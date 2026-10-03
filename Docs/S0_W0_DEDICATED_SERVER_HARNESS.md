# S0-W0 — Dedicated Server Target and Two-Client Preflight

**Document ID:** MORABEZZA-S0-W0-SERVER-HARNESS  
**Date:** 2026-09-24  
**Status:** IMPLEMENTATION CANDIDATE — SOURCE-ONLY PRECHECK; UNREAL RUNTIME NOT VERIFIED  
**Baseline:** `node63labs/morabezza-game@af8f3d0e5f9291b1358232c7055190e4bd57b9f6`  
**Branch:** `feat/s0-w0-server-harness`  
**Dependencies:** RM-02 architecture candidate (draft PR #3, not yet approved/merged); RM-01 recovery draft PR #2; an Unreal 5.8 host and SSD reconciliation remain pending.

## 1. Exact change scope

This PR adds a dedicated-server build target that mirrors the existing game and Editor target settings, a guarded two-client launch script, source-only regression tests, and GitHub Actions static preflight. No existing gameplay C++, engine configuration, `.uproject`, Unreal map, asset package, authoritative world state, repository visibility or Lariba Cloud service was changed.

| Path | Change |
| --- | --- |
| `Source/MORABEZAServer.Target.cs` | Unreal `TargetType.Server` using the existing module and Unreal 5.8 build setting. |
| `scripts/mmo/s0_two_client_smoke.sh` | Isolated game-server + two-client local launch plan; optional launch with process-liveness and retained logs. |
| `tests/mmo/test_s0_w0_preflight.py` | Standard-library tests for target shape, dry-run and negative input cases. |
| `.github/workflows/s0-w0-preflight.yml` | Read-only CI that runs Bash syntax and source-only tests; no Unreal installation or binary assets downloaded. |
| `Docs/S0_W0_DEDICATED_SERVER_HARNESS.md` | This preflight/acceptance record. |

## 2. Why this is not a finished multiplayer feature

The current `MORABEZAGameMode.cpp` startup test-contact code uses `GetFirstPlayerController()`, and `MORABEZAContactActor.cpp` can fall back to the first player controller. Those behaviors are not appropriate for per-connection dialogue/interaction. `MORABEZAInteractionComponent.cpp` currently performs a local target lookup and direct interaction execution. These remain deliberately unchanged in S0-W0; dedicated-server target presence does **not** establish secure replicated movement, authenticated player sessions, cross-client ownership or two-player gameplay.

The current GitHub maps `Untitled` and `dskjnfkljsdnf` remain candidate worlds with unresolved authoritative Landscape/World Partition state. This harness refuses any map outside the disposable `/Game/Dev/<Name>` namespace. An actual test map must be created and verified in Unreal separately; this PR does not generate one.

## 3. Static preflight, safe from GitHub

On GitHub Actions, the workflow executes:

    bash -n scripts/mmo/s0_two_client_smoke.sh
    python3 -m unittest discover -s tests/mmo -p 'test_*.py' -v

Tests use `/bin/echo` only as a **non-executing, dry-run path placeholder**. No Unreal binary, game server or client process runs in CI. No engine compilation or LFS fetch is claimed by the workflow.

Success is limited to syntax, target-source shape and guarded-script input validation. Source-only CI passing does not authorize PR merge or qualify S0-W0 as a runtime acceptance gate.

## 4. Future actual Unreal execution prerequisites

- A known-good Unreal Engine **5.8** build compatible with the repository's required `EnhancedInput` and `MoverExamples` plugins and the platform compiler/SDK.
- A fresh source checkout with Git LFS assets available, plus a separate, verified `/Game/Dev/MMO_S0_TestZone` map or another approved disposable map.
- Compiled `MORABEZAServer` dedicated-server and `MORABEZA` game-client binaries from the same reviewed SHA/engine build.
- Explicit resolution of whether the current game mode is safe for two clients, or a bounded per-connection spawn/interaction remediation PR with accompanying tests.
- A test environment with available loopback TCP/UDP connections, local ports, logs and permission to launch two graphical clients.

For a source-build Unreal installation, a prospective target-build invocation is:

    <UE_5.8>/Engine/Build/BatchFiles/Mac/Build.sh MORABEZAServer Mac Development -Project="<repo>/MORABEZA.uproject" -WaitMutex

That command is a **candidate** for a compatible Mac source-engine installation, not a tested build instruction. A launcher-installed engine may not provide a usable dedicated-server build path. Use the engine's actual supported target/build configuration and capture its output.

## 5. Plan-only check

With executable paths from a compatible test host:

    export MORABEZA_SERVER_BIN="/absolute/path/to/MORABEZAServer"
    export MORABEZA_CLIENT_BIN="/absolute/path/to/MORABEZA"
    export MORABEZA_TEST_MAP="/Game/Dev/MMO_S0_TestZone"

    bash scripts/mmo/s0_two_client_smoke.sh --plan

A plan prints the server and two client commands but **does not start any process**.

## 6. Controlled launch (future)

Only after the prerequisites and game-mode safety checks are met:

    bash scripts/mmo/s0_two_client_smoke.sh --run

The script starts the server, then two client processes connecting to the same loopback endpoint, verifies basic process liveness and cleans up only the processes it started. It retains stdout/stderr logs in a temporary directory. It does **not** parse Unreal networking logs, prove that clients joined, identify players, exercise server RPCs or validate replication; capture those separately.

Required manual/automated runtime evidence for S0-W0 acceptance:

| Gate | Required result |
| --- | --- |
| Unreal Server target compile | PASS with engine version, compiler, binary provenance and source SHA |
| Disposable dev-map load | PASS without touching the unresolved production maps |
| Server launch and listen | PASS with actual Unreal startup/connection logs |
| Two independent client connections | PASS with evidence of distinct owning controller, pawn and session |
| Cross-client presence | PASS; each client observes the correct other pawn |
| Ownership isolation | PASS; A's action/UI does not control B; invalid RPC ownership rejected |
| Exit/restart behavior | PASS with no orphaned runtime process or corrupted local save |
| RM-01 canonical production world | Remains a **separate** gate; S0-W0 is not M0 approval |

**S0-W0 current classification:** server-target source added; CI preflight eligible; engine compilation and two-client gameplay **NOT EXECUTED**. PR remains draft pending RM-02 review and an Unreal-compatible execution environment.
