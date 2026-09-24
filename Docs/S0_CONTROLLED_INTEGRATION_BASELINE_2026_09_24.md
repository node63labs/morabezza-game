# S0 — Controlled Integration Baseline, PRs #4–#7

**Date:** 2026-09-24  
**Repository:** `node63labs/morabezza-game`  
**Candidate branch:** `integration/s0-w0-w1c-candidate-20260924`  
**Status:** SOURCE CONTENT INTEGRATED IN AN ISOLATED CANDIDATE; **NO MAIN MERGE AUTHORIZED**  
**Governance:** Evidence-first, bounded branch/PR review; main is the authoritative integrated repository baseline, not this candidate.  
**Scope:** Unreal source, offline preflight and development-only test contacts; no production world or game-domain authority accepted.

## 1. Frozen source authority and exact composition

| Component | PR | Frozen source SHA | Materialization into candidate |
| --- | --- | --- | --- |
| S0-W0 dedicated-server source/harness | #4 | `115ddba8c43377656e1a7e70c2374607e4347355` | Five PR #4 text files copied with unchanged Git blob IDs |
| S0-W1a per-player test contact | #5 | `6962de04e7403af8fa8dbb2624fad504e64b7276` | Included through PR #7 ancestor chain |
| S0-W1b server interaction validation | #6 | `e7f9ed86b61a8a8f4ee22de2903ef1d68a7f18c2` | Included through PR #7 ancestor chain |
| S0-W1c input/HUD possession lifecycle | #7 | `42d6307299c4123bd56689b333d99db4dc6a2f21` | **Exact base commit** of integration branch |
| Original authoritative main at creation | — | `af8f3d0e5f9291b1358232c7055190e4bd57b9f6` | Unchanged; only an ancestor, not a merge target |

**Important provenance distinction:** GitHub's connected editing interface provided file-level creation but no reviewed branch-merge/cherry-pick operation. Therefore the candidate was created from **PR #7's exact commit** and PR #4's five files were materialized *byte-for-byte*. Their Git blob IDs match PR #4; **PR #4's commits are not integrated into the candidate's Git ancestry**. The PR #5 → #6 → #7 ancestry *is* retained.

GitHub recursive-tree inspection, before adding integration-only artifacts, compared the candidate with the expected union of PR #7's complete source tree and PR #4's five files: **zero extra paths, zero missing paths, zero blob mismatches**. The frozen source-path manifest is `Docs/S0_INTEGRATION_SOURCE_MANIFEST.json`, covering **21 paths** (five from PR #4 and 16 modified/added paths from the #5–#7 stack).

The integration branch adds **only** the source manifest, the offline verifier and this control record beyond those frozen source snapshots. It does not alter `main`, the original PR branches, `MORABEZA.uproject`, tracked config, LFS assets, `.umap` or `.uasset` packages.

## 2. Controlled change sets and dependencies

```text
main (untouched)
  |
  +-- PR #4 S0-W0 (separate draft: server target, harness, source-only workflow)
  |
  +-- PR #5 S0-W1a (draft)
        |
        +-- PR #6 S0-W1b (draft)
              |
              +-- PR #7 S0-W1c (draft)
                    |
                    +-- THIS INTEGRATION CANDIDATE
                         exact PR #4 file content imported
                         + frozen manifest
                         + offline verifier
                         + integration decision/evidence record
```

The integration-review PR targets **PR #7's branch**, not `main`. Its diff shows only the import of PR #4 content and integration-only evidence/verification files. The branch itself contains the complete candidate tree for later local testing. This is **not** an instruction to merge the integration PR into PR #7 or retarget it to `main`.

## 3. GitHub Actions policy for this candidate

PR #4's source-only workflow is preserved byte-for-byte. Its automatic triggers are restricted to pushes on `feat/s0-w0-server-harness` and pull requests targeting `main`; it also offers a **manual** `workflow_dispatch` trigger.

- Creating/updating this integration branch does **not** match that workflow's push branch filter.
- The integration-review PR targets `feat/s0-w1c-possession-safe-input-hud`, not `main`, so it does **not** match the workflow's PR base filter.
- No workflow is dispatched as part of this baseline preparation. Do not use an Actions run as a substitute for UE compilation.
- If the integration PR is later retargeted to `main`, recheck workflow triggers and repository usage allowance **before** doing so.

## 4. Offline verifier and intended evidence

`scripts/mmo/verify_s0_integration.py` uses only the Python standard library and has two explicit modes:

```bash
# Snapshot check (cross-platform, no engine, no GitHub API calls):
python3 scripts/mmo/verify_s0_integration.py

# Optional on a Linux/macOS/WSL checkout with Bash:
python3 scripts/mmo/verify_s0_integration.py --run-tests
```

The default mode checks all 21 frozen Git blob identities against local raw file contents using the SHA-1 Git blob format. It refuses missing or changed paths and invalid manifest structure. `--run-tests` additionally performs Bash syntax validation and Python `unittest` discovery of S0-W0, S0-W1b and S0-W1c source-contract tests. Those tests are **not** a game integration test.

**Execution status at baseline creation:** source-tree and exact Git blob comparison through the connected GitHub read API: PASS. Offline verifier and the three Python test suites on a local checkout: **NOT EXECUTED**. The current assistant sandbox could not resolve `github.com` to obtain a local repository checkout; do not treat the committed script as having run. Unreal Header Tool/C++ compilation, the server target, a two-client session and negative interaction gameplay tests: **NOT EXECUTED**.

## 5. Acceptance gates and block conditions

| Gate | Required evidence | Current state |
| --- | --- | --- |
| INT-00 — branch isolation | Candidate derived from frozen PR #7; PR #4 file blobs verified; `main` unchanged | PASS — source snapshot |
| INT-01 — offline source preflight | Verifier confirms 21 blob hashes; Bash syntax and Python suites complete on checked-out candidate | PENDING |
| INT-02 — engine compatibility | Pinned UE 5.8, required plugins, Unreal Header Tool and Editor/dedicated-server compile | PENDING |
| INT-03 — network lifecycle | Dedicated server + two clients in approved disposable test zone; distinct pawns/owners, post-possession input/HUD, respawn and teardown | PENDING |
| INT-04 — authorization | Server-side interaction T01–T11 and lifecycle L01–L11, including forged owner/target, distance/LOS, throttling and cross-player HUD isolation | PENDING |
| INT-05 — merge governance | Architecture owner/founder review; upstream draft PR sequencing, known issues, rollback and `main` protection/approved compensating controls | PENDING |
| RM-01 production world | SSD reconciliation and canonical Landscape/World Partition decision | BLOCKED — separate gate |

`main` was **not protected** when the candidate was prepared; the absence of branch protection is **not** permission to push directly. All candidate changes remain isolated and draft. The baseline itself is not accepted gameplay, an MMO release candidate or production authority.

## 6. Known limitations to review before UE execution

- S0-W0's script verifies process liveness, **not** actual client connection, player identity or replication.
- The test-zone map under `/Game/Dev/<Name>` does not yet exist as an approved tracked Unreal package. Do not use `Untitled` or `dskjnfkljsdnf` as an implicit canonical map.
- S0-W1a contact spawning and owner relevancy are source-only; exact dedicated-server behavior is untested.
- S0-W1b restricts network interactions to an owner-scoped `TEST_INTERACTION` contact; the rate limit is not durable idempotency, and no game rewards are authorized.
- S0-W1c uses a lightweight HUD tick to reconcile delayed possession; an actual two-client runtime must verify widget and delegate lifecycle. A widget creation failure after a valid local controller is logged but is not automatically retried.
- PR #3 MMO architecture and portfolio-priority decisions remain candidate/reviewed separately. No production Lariba consumer contract is implied.
- The separate public-configuration token review remains open; this branch does not reconfigure credentials or erase history.

## 7. Controlled promotion, rollback and exit

1. Keep PRs #4–#7 and the integration-review PR draft; record reviewers/decisions separately. **Do not merge or force-push `main`.**
2. Run the committed offline verifier from a fresh checkout, capturing its output and exact candidate SHA. Fix any mismatch on a new reviewed candidate revision, then update the manifest deliberately.
3. Obtain a compatible UE 5.8 build environment and execute the runtime acceptance matrix on an approved disposable map. Track failures before any promotion.
4. Only after owner approval and all applicable gates pass, decide an explicit upstream merge order. Recheck the latest PR head SHAs and rebase/reconcile any changed branches; do not silently replace the frozen snapshots.
5. If the candidate fails, close the integration-review PR or rebuild a new candidate branch from verified source commits. Original PRs remain independent; **no production rollback is needed because this baseline is not deployed or merged into `main`.**

**Current decision:** Integration **candidate prepared**; source content matches frozen PR #4–#7 snapshots. All runtime and promotion gates remain open.
