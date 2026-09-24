# INT-01 — Controlled Integration Verification Evidence

**Date:** 2026-09-24  
**Repository:** `node63labs/morabezza-game`  
**Integration branch:** `integration/s0-w0-w1c-candidate-20260924`  
**Frozen candidate commit verified:** `35f4c8a66c0919f1c25b4d8c6f00876b8820b9c4`  
**Frozen PR #7 base:** `42d6307299c4123bd56689b333d99db4dc6a2f21`  
**Frozen PR #4 import:** `115ddba8c43377656e1a7e70c2374607e4347355`  
**Authority main at preflight:** `af8f3d0e5f9291b1358232c7055190e4bd57b9f6`  
**Status:** **PARTIAL PASS — INT-01 ACCEPTANCE NOT YET MET**  
**Actions policy:** No GitHub Actions workflow was manually or automatically run for this verification.

## 1. What was actually executed

### INT-01a — Remote exact-source identity: PASS

The verifier read `Docs/S0_INTEGRATION_SOURCE_MANIFEST.json` from the candidate branch, then independently fetched **each of its 21 listed files** from the same branch through the live GitHub repository connector. All 21 returned Git blob SHA IDs matched their frozen manifest values:

- Five S0-W0 files imported byte-for-byte from PR #4.
- Sixteen files from the PR #5 → #6 → #7 source stack.
- Zero manifest-vs-GitHub blob mismatches in three bounded read batches (8 + 8 + 5).
- No source-file content was modified during verification.

Git blob IDs are returned by GitHub for those files; this is a remote, independent **source identity** check. It is **not** a successful local run of `scripts/mmo/verify_s0_integration.py`.

### INT-01b — Remote source-contract assertions: PASS (limited scope)

Eight candidate files were fetched from GitHub and evaluated by an isolated JavaScript source checker. **18/18 bounded source-contract assertions PASS** for server-target declaration, harness plan/guard structure, candidate-to-server RPC routing, owner/range/LOS guard presence, idempotent input mapping, HUD possession reconciliation and delegate teardown. The three committed Python suites contain **23 test methods** in total (S0-W0: 6, S0-W1b: 8, S0-W1c: 9).

These are **string/source structure checks**, not execution of the committed Python tests, a Bash parser, Unreal Header Tool, C++ compilation, real RPC traffic or actual two-player behavior. A source guard may be present yet still be incorrect at runtime.

## 2. Why INT-01 is not accepted

The current execution sandbox has Python and Git but **cannot resolve `github.com`** for a fresh checkout. Its attempted `git ls-remote https://github.com/node63labs/morabezza-game.git HEAD` returned: `Could not resolve host: github.com` (exit 128). GitHub-connector read calls succeed, but the connector does not materialize a full working tree in the sandbox. No exact local integration checkout was available for the committed verifier or the three Python suites. A GitHub-hosted Actions run was intentionally **not** substituted for offline testing.

Accordingly the following required checks remain **NOT EXECUTED**:

```bash
python3 scripts/mmo/verify_s0_integration.py
python3 scripts/mmo/verify_s0_integration.py --run-tests
```

The second command would perform the Bash syntax test plus Python standard-library `unittest` discovery in the same exact integration checkout. A 21/21 remote GitHub blob readback does **not** claim either command passed locally.

## 3. Formal gate matrix

| Gate item | Result | Evidence / limitation |
| --- | --- | --- |
| Frozen manifest vs live candidate source file identities | **PASS — 21/21** | Independently retrieved GitHub blob IDs match the frozen 21-path manifest. |
| Source-level integration guard presence | **PASS — 18/18** | JavaScript source checker; no Python test invocation. |
| Local verifier SHA-1 checks against a checked-out tree | **PENDING** | Sandbox lacks the repository checkout. |
| Bash `-n` syntax check of integrated launch script | **PENDING** | Not executed against the checked-out candidate in INT-01. |
| S0-W0 Python suite (6 methods) | **PENDING** | Not executed. |
| S0-W1b Python suite (8 methods) | **PENDING** | Not executed. |
| S0-W1c Python suite (9 methods) | **PENDING** | Not executed. |
| Unreal Engine 5.8 UHT / C++ compilation | **OUT OF SCOPE / PENDING INT-02** | Requires compatible Unreal host. |
| Dedicated server and two-client gameplay | **OUT OF SCOPE / PENDING INT-03/04** | No Unreal runtime evidence. |
| `main` merge | **NOT AUTHORIZED** | Candidate remains isolated and draft. |

**INT-01 disposition:** identity and remote source checks are verified, but INT-01 remains **PARTIAL / NOT ACCEPTED** until the local verifier and all three offline Python suites complete against an exact checkout.

## 4. Completion path without GitHub Actions

Provide the exact GitHub integration branch source tree to an offline Python/Bash runner (for example an uploaded ZIP made from candidate commit `35f4c8a66c0919f1c25b4d8c6f00876b8820b9c4`, or an independently reachable clone). The archive must include the manifest, all 21 listed files, verifier, existing game build target and offline test suites. Do not infer a clean original working tree from an archive if there are untracked local changes.

On that exact candidate, execute:

```bash
python3 scripts/mmo/verify_s0_integration.py --run-tests
```

Record interpreter version, OS/Bash version, exact candidate commit, 21 blob results, Bash syntax result, Python test count, failures and exit code. If any file is intentionally changed, create a new reviewed candidate snapshot and update the manifest rather than weakening hash validation. No full Unreal LFS payload download is required for these source-only tests.

If the source archive is uploaded into the conversation, the assistant can use the sandbox to run the Python and Bash checks and attach their actual output to this record. An Unreal-compatible host is a separate requirement for INT-02 and beyond.

**No `main` promotion, PR merge, runtime claim or gameplay acceptance is implied by this evidence.**
