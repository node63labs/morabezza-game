# INT-02D — Full Offline Regression Evidence (PR #10)

**Date:** 2026-09-25  
**Repository:** `node63labs/morabezza-game`  
**Reviewed branch:** `review/int-02c-prebuild-cpp`  
**PR:** #10 (draft, stacked on draft PR #9)  
**Source branch head before this evidence-only commit:** `1e8a15f43648e1d9009e9d3b92171b3fba8d2136`  
**Gate:** **INT-02D OFFLINE REGRESSION PASS** — scoped to reconstructed, blob-verified test-relevant source; NOT Unreal build acceptance.  
**Execution:** isolated Linux sandbox, system Python 3.13.5, Bash 5.2.37. No GitHub Actions.

## 1. Exact source provenance

The user-supplied S0 integration ZIP was extracted into an isolated local working directory. Its SHA-256:

```text
c97f227414c0d51b08b8730edb4d55adbc7cc72c5defa66959d4340e1463a442
```

The original frozen S0 source manifest independently verified **21/21** file contents from the ZIP. Because the ZIP predates draft PRs #9 and #10, the offline test fixture was completed using their actual live GitHub source content and the three documented INT-02C source amendments. Each reconstructed file was independently checked against its **exact Git blob ID from PR #10** before running tests.

- Original INT-01 manifest: unchanged, blob `0c94f2584425469d5acbef7c212196037f8b1754`.
- INT-02C pinned overlay: blob `27992690bef79ab73a5470736c47c5144edcd913`, identifying the three reviewed pre-build C++ amendments and one new regression test.
- Amended verifier: blob `d70e72a50fc9546d282caf8f22b7992afb6b5b77`.
- PR #9's actual INT-02 preflight script and five-test suite: blobs `9edcc1c00de5a369bfcbf481314a2a21c7e6ae24` and `a06327c12a844f4876d936dc54288652e33380b5`.
- INT-02C C++ amendments and new source regression suite: individual actual GitHub blobs checked; the pinned overlay verifier validated **24/24** effective source paths. Four additional PR #9/support blob identities independently passed **4/4**.

**Identity limitation:** This is a *blob-verified reconstruction of the executable source/test subset*, **not** a Git checkout of PR #10. The ZIP does not contain a `.git` directory, Git LFS payloads, or all PR #9/#10 evidence documents. No claim is made that the archive itself is an exact PR #10 commit checkout.

## 2. Executed regression command

From the reconstructed project root, using system Python:

```bash
/usr/bin/python3 -m py_compile \
  scripts/mmo/verify_s0_integration.py \
  scripts/mmo/int02_unreal_build_gate.py \
  tests/mmo/test_int02_unreal_build_gate.py \
  tests/mmo/test_int02c_prebuild_source.py

/usr/bin/python3 scripts/mmo/verify_s0_integration.py --run-tests
```

The committed verifier independently checked the 24-path pinned source overlay, performed `bash -n scripts/mmo/s0_two_client_smoke.sh`, and invoked Python standard-library unittest discovery for all five `tests/mmo/test_*.py` modules.

| Suite | Executed tests | Result |
| --- | ---: | --- |
| S0-W0 server harness source preflight | 6 | PASS |
| S0-W1b server interaction source contracts | 8 | PASS |
| S0-W1c possession and HUD source contracts | 9 | PASS |
| INT-02 UE5.8 build-gate negative fixtures | 5 | PASS |
| INT-02C pre-build cross-target source regression | 7 | PASS |
| **Combined unittest discovery** | **35** | **PASS, 0 failures/errors** |

Additional results: original frozen ZIP **21/21** hashes PASS; effective INT-02C overlay **24/24** hashes PASS; independently pinned PR #9/support files **4/4** PASS; Bash syntax PASS; Python syntax PASS; committed verifier **exit 0**.

Recorded terminal excerpt:

```text
INT-02C REVIEW OVERLAY ACTIVE: 3 reviewed changes and 1 new regression test
S0 SOURCE SNAPSHOT PASS: 24 exact Git blob IDs
S0 OFFLINE TEST: bash -n scripts/mmo/s0_two_client_smoke.sh
S0 OFFLINE TEST: /usr/bin/python3 -m unittest discover -s tests/mmo -p test_*.py -v
Ran 35 tests in 0.052s
OK
S0 OFFLINE TEST PASS: syntax and Python source-contract suites only
UNREAL COMPILE: NOT EXECUTED; TWO-CLIENT NETWORK TEST: NOT EXECUTED
```

Full offline regression log SHA-256:

```text
7ac76175d023c6828bb785d162e9ba37ffe6070c3f15b6e44262644e3f388956
```

Two independent **negative mutation checks** were also executed on temporary copies: (1) modifying `MORABEZACharacter.cpp` and (2) changing the review overlay's pinned base SHA. The verifier rejected both with exit code 1. After restoration, the 24-file verifier returned exit 0. These temporary changes were **not** committed.

## 3. Decision and boundaries

**INT-02D full offline regression: PASS.** This closes the prior PR #10 verification gap: PR #9's five tests and INT-02C's seven tests have now been executed together with S0-W0/W1b/W1c against the same locally reconstructed, blob-verified test-relevant source tree.

**INT-02 Unreal build gate: still BLOCKED / NOT ACCEPTED.** The supplied ZIP contains Git LFS pointer placeholders and no Git commit metadata; the sandbox does not have a compatible UE 5.8 engine, UHT, plugins and build environment. No Editor, Game or dedicated-server compilation or two-client gameplay test was attempted. Source tests do not establish UHT/C++ compatibility, RPC runtime security or replication correctness.

PR #10 remains draft; PRs #4–#9 remain under their existing review dependencies. No GitHub Actions were used and no merge into `main` or production deployment was performed.

**Next execution requirement:** an authorized UE 5.8-capable host with the pinned branch's actual Git+LFS checkout, its required plugins/compiler and recorded build logs. The current `scripts/mmo/int02_unreal_build_gate.py` remains fail-closed until those prerequisites are satisfied.
