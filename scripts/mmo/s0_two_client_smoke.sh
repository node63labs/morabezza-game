#!/usr/bin/env bash
# S0-W0: local two-client launch harness. No Unreal runtime is bundled here.
# This checks launch/liveness only; it does NOT prove network replication.
set -euo pipefail

usage() {
  cat <<'USAGE'
Usage: scripts/mmo/s0_two_client_smoke.sh --plan|--run

Required environment:
  MORABEZA_SERVER_BIN   Absolute path to a compiled, executable dedicated server.
  MORABEZA_CLIENT_BIN   Absolute path to a compiled, executable game client.
  MORABEZA_TEST_MAP     Disposable map asset path: /Game/Dev/<Name>.

Optional:
  MORABEZA_TEST_PORT    Local server port (1024..65535; default 7777).
  MORABEZA_SMOKE_WAIT   Seconds to observe client liveness (1..60; default 8).

--plan validates inputs and prints shell-escaped launch commands.
--run starts one server and two clients, checks process liveness, then stops
only the launched processes. Logs remain in a temporary directory.

The two existing candidate production maps are NOT accepted as test maps.
This harness does not create a map, build Unreal, validate session ownership,
or verify that either client has actually connected to the server.
USAGE
}

fail() {
  printf 'S0-W0 PRECHECK FAIL: %s\n' "$*" >&2
  exit 2
}

if [[ $# -ne 1 ]]; then
  usage >&2
  exit 2
fi

mode="$1"
if [[ "$mode" == "--help" || "$mode" == "-h" ]]; then
  usage
  exit 0
fi
if [[ "$mode" != "--plan" && "$mode" != "--run" ]]; then
  fail "expected --plan or --run"
fi

server_bin="${MORABEZA_SERVER_BIN:-}"
client_bin="${MORABEZA_CLIENT_BIN:-}"
test_map="${MORABEZA_TEST_MAP:-}"
port="${MORABEZA_TEST_PORT:-7777}"
wait_seconds="${MORABEZA_SMOKE_WAIT:-8}"

[[ "$server_bin" == /* && -f "$server_bin" && -x "$server_bin" ]] ||
  fail "MORABEZA_SERVER_BIN must name an existing executable at an absolute path"
[[ "$client_bin" == /* && -f "$client_bin" && -x "$client_bin" ]] ||
  fail "MORABEZA_CLIENT_BIN must name an existing executable at an absolute path"

# A narrow test-zone path avoids silently booting unresolved production maps.
[[ "$test_map" =~ ^/Game/Dev/[A-Za-z0-9_]+$ ]] ||
  fail "MORABEZA_TEST_MAP must be /Game/Dev/<Name>; never a candidate production map"

[[ "$port" =~ ^[1-9][0-9]{3,4}$ ]] ||
  fail "MORABEZA_TEST_PORT must be a decimal integer 1024..65535"
(( port >= 1024 && port <= 65535 )) ||
  fail "MORABEZA_TEST_PORT outside 1024..65535"

[[ "$wait_seconds" =~ ^[1-9][0-9]?$ ]] ||
  fail "MORABEZA_SMOKE_WAIT must be an integer 1..60"
(( wait_seconds <= 60 )) ||
  fail "MORABEZA_SMOKE_WAIT outside 1..60"

server_cmd=("$server_bin" "$test_map" "-port=$port" -log -unattended -nosplash)
client_cmd=("$client_bin" "127.0.0.1:$port" -windowed -ResX=960 -ResY=540 -log -unattended -nosplash -NoSound)

printf 'S0-W0: proposed local launch plan (not executed)\n'
printf 'server:'
printf ' %q' "${server_cmd[@]}"
printf '\nclient A:'
printf ' %q' "${client_cmd[@]}"
printf '\nclient B:'
printf ' %q' "${client_cmd[@]}"
printf '\n'

if [[ "$mode" == "--plan" ]]; then
  printf 'S0-W0 PLAN PASS: configuration validated; Unreal was not started.\n'
  exit 0
fi

log_dir="$(mktemp -d "${TMPDIR:-/tmp}/morabeza-s0-w0.XXXXXX")"
server_pid=''
client_a_pid=''
client_b_pid=''

cleanup() {
  local original_status="$1"
  trap - EXIT INT TERM
  for pid in "$client_b_pid" "$client_a_pid" "$server_pid"; do
    if [[ -n "$pid" ]]; then
      kill "$pid" 2>/dev/null || true
    fi
  done
  for pid in "$client_b_pid" "$client_a_pid" "$server_pid"; do
    if [[ -n "$pid" ]]; then
      wait "$pid" 2>/dev/null || true
    fi
  done
  printf 'S0-W0: launch logs retained at %s\n' "$log_dir"
  exit "$original_status"
}
trap 'cleanup $?' EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

printf 'S0-W0: starting dedicated server\n'
"${server_cmd[@]}" >"$log_dir/server.log" 2>&1 &
server_pid=$!
sleep 3
kill -0 "$server_pid" 2>/dev/null ||
  fail "server exited during startup; inspect $log_dir/server.log"

printf 'S0-W0: starting two independent client processes\n'
"${client_cmd[@]}" >"$log_dir/client-a.log" 2>&1 &
client_a_pid=$!
"${client_cmd[@]}" >"$log_dir/client-b.log" 2>&1 &
client_b_pid=$!
sleep "$wait_seconds"

kill -0 "$server_pid" 2>/dev/null ||
  fail "server exited; inspect $log_dir/server.log"
kill -0 "$client_a_pid" 2>/dev/null ||
  fail "client A exited; inspect $log_dir/client-a.log"
kill -0 "$client_b_pid" 2>/dev/null ||
  fail "client B exited; inspect $log_dir/client-b.log"

printf 'S0-W0 LAUNCH-LIVENESS PASS: three processes remained alive.\n'
printf 'S0-W0 NETWORK/OWNERSHIP/REPLICATION: NOT VERIFIED; capture Unreal logs and two-client evidence separately.\n'
