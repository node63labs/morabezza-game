# INT-01 — Offline Integration Verification Evidence

**Date:** 2026-09-24  
**Repository:** `node63labs/morabezza-game`  
**Integration branch:** `integration/s0-w0-w1c-candidate-20260924`  
**Branch head when archive was obtained:** `2aa532fef43140f50af615c6b42266508cac3a79`  
**Frozen PR #7 integration base:** `42d6307299c4123bd56689b333d99db4dc6a2f21`  
**Frozen PR #4 file import:** `115ddba8c43377656e1a7e70c2374607e4347355`  
**Authority `main` at execution check:** `af8f3d0e5f9291b1358232c7055190e4bd57b9f6`  
**Status:** **INT-01 PASS — OFFLINE SOURCE INTEGRATION ACCEPTED ONLY**  
**GitHub Actions:** Not used for this gate.  
**Unreal/runtime:** Not executed and not accepted.

## 1. Verified archive provenance and execution context

The user supplied the ZIP for the integration branch as a conversation attachment. The exact uploaded ZIP was inspected before extraction: 699 ZIP entries, 371,244 declared uncompressed bytes, one expected top-level project folder, no unsafe traversal paths, and no ZIP symlinks. The archive's SHA-256 was:

```text
c97f227414c0d51b08b8730edb4d55adbc7cc72c5defa66959d4340e1463a442
```

A GitHub source archive contains no `.git` commit metadata; archive name alone does not prove its Git commit. The accepted source identity comes from the **21 frozen Git blob hashes** in `Docs/S0_INTEGRATION_SOURCE_MANIFEST.json`, checked against the actual extracted file bytes. The earlier remote GitHub readback separately confirmed 21/21 blob identities against the integration branch.

Execution environment: isolated Linux sandbox, system Python **3.13.5** (`/usr/bin/python3`), Bash **5.2.37**, no GitHub Actions, no engine installation or game runtime. The final clean execution used system Python to avoid an unrelated virtual-environment startup warning encountered during a preceding successful run.

## 2. Command and actual results

Executed from the extracted archive's project root:

```bash
/usr/bin/python3 scripts/mmo/verify_s0_integration.py --run-tests
```

The committed verifier ran its local Git blob-hash check and then invoked:

```bash
bash -n scripts/mmo/s0_two_client_smoke.sh
/usr/bin/python3 -m unittest discover -s tests/mmo -p 'test_*.py' -v
```

| INT-01 check | Observed result |
| --- | --- |
| Exact source manifest checks against extracted file bytes | **PASS, 21/21 frozen Git blob IDs** |
| Bash syntax validation of the two-client launch harness | **PASS, exit 0** |
| S0-W0 source-only preflight | **PASS, 6 tests** |
| S0-W1b source-contract checks | **PASS, 8 tests** |
| S0-W1c possession/HUD source-contract checks | **PASS, 9 tests** |
| Combined Python unittest discovery | **PASS, 23 tests, 0 failures/errors** |
| Committed verifier process | **PASS, exit 0** |
| Unreal Header Tool/C++ compilation | **NOT EXECUTED — INT-02** |
| Dedicated server, client connection, replication, gameplay | **NOT EXECUTED — INT-03/04** |
| Merge into `main` | **NOT AUTHORIZED / NOT PERFORMED** |

Recorded terminal summary:

```text
S0 SOURCE SNAPSHOT PASS: 21 exact Git blob IDs
S0 STATUS: candidate only; Unreal compile and runtime NOT VERIFIED
S0 OFFLINE TEST: bash -n scripts/mmo/s0_two_client_smoke.sh
S0 OFFLINE TEST: /usr/bin/python3 -m unittest discover -s tests/mmo -p test_*.py -v
S0 OFFLINE TEST PASS: syntax and Python source-contract suites only
UNREAL COMPILE: NOT EXECUTED; TWO-CLIENT NETWORK TEST: NOT EXECUTED

----------------------------------------------------------------------
Ran 23 tests in 0.209s

OK
```

No actual game server or client was launched. S0-W0 tests only exercise the script's nonexecuting `--plan` mode using a harmless executable path placeholder. Source-contract tests inspect C++ text; they are **not** C++ execution, network authorization proof or UE5.8 build evidence.

## 3. Prior remote verification and resolved blocker

Earlier in this gate, the connected GitHub source review passed **21/21 remote blob identities** and **18/18 source-string checks**, but the Python/Bash suite remained pending because the sandbox could not clone GitHub by DNS. The user's uploaded ZIP resolved that exact-checkout blocker: all 21 frozen file hashes passed in the extracted tree and the full offline Bash/Python suite executed successfully. The previous **PARTIAL PASS** classification is superseded by this record.

## 4. Acceptance boundary and next gate

**INT-01 is ACCEPTED for the defined offline source-integration scope only.** The integration branch and draft PR #8 remain isolated. PRs #4–#7 still require reviewed integration sequencing, and no new production, canonical-world, security or multiplayer-runtime claim follows from these tests.

**INT-02** requires a compatible Unreal Engine 5.8 development environment, the actual required plugins/compiler, generated-header validation, game/client and dedicated-server compilation, and captured build logs at a pinned candidate source revision. The `/Game/Dev/<Name>` disposable test-zone requirement and RM-01 canonical-world/SSD authority remain separate. INT-03/04 require an actual server and two-client playtest, ownership/range/LOS negative cases and possession/respawn evidence.

`main` was unchanged at verification time; its branch-protection review remains open. Do not mark the integrated runtime complete or merge this candidate into `main` because INT-01 passed.
