# INT-02 — Unreal Engine 5.8 Build Verification

**Date:** 2026-09-24  
**Repository:** `node63labs/morabezza-game`  
**Verification branch:** `verification/int-02-ue58-build-gate`  
**Direct parent:** draft integration PR #8, head at branch creation `25721eb6dd040584c0bff5f9f7c00be7fd67e8a6`  
**Source scope:** S0-W0 through S0-W1c integrated candidate; no change to the 21 frozen source paths.  
**Status: BLOCKED — Unreal build NOT executed; no INT-02 acceptance.**  
**Actions:** Not used. **Main:** Untouched and not authorized for merge.

## 1. Actual inspection of the user-supplied source archive

The source archive supplied for INT-01 was inspected in an isolated Linux sandbox. Its GitHub archive comment identified integration commit `2aa532fef43140f50af615c6b42266508cac3a79`, an earlier candidate snapshot preceding the final INT-01 documentation commit. The frozen 21-source-file manifest is unchanged in the subsequent branch. A GitHub source ZIP is not a Git checkout and cannot itself prove `git rev-parse HEAD`.

Actual project facts read from this archive:

| Check | Observed |
| --- | --- |
| `MORABEZA.uproject` `EngineAssociation` | `5.8` |
| Module/targets | `MORABEZA` runtime module; `MORABEZA`, `MORABEZAEditor`, `MORABEZAServer` targets present |
| Required enabled plugins | `EnhancedInput`; `MoverExamples` |
| Unreal `.uasset`/`.umap` tracked source archive files | 293 |
| Git LFS pointer placeholders in those files | **293/293**; no hydrated asset payloads in the downloaded ZIP |
| Unreal Engine 5.8 Build.sh/UnrealBuildTool in sandbox | **Not installed** |
| `dotnet` in sandbox | **Not installed** |
| Git commit metadata in ZIP | **Absent** |

The sandbox does provide Python, Bash and generic C++ compilers, but generic `clang++`/`g++` cannot compile Unreal generated reflection code without the matching engine/UHT, build rules and plugins. An apparent `*.Target.cs` file is build intent, not evidence that UnrealBuildTool accepted it.

**Therefore INT-02 could not execute an Unreal Editor, Game or Server compilation.** The blocked state is a missing-execution-environment finding, not a demonstrated compilation failure.

## 2. New fail-closed build gate (this PR)

`scripts/mmo/int02_unreal_build_gate.py` provides an explicit, evidence-producing gate:

1. Verify the frozen 21-file source manifest through the existing offline verifier.
2. Require an actual `.git` checkout and an exact 40-character `--candidate-sha` matching `git rev-parse HEAD`.
3. Verify `MORABEZA.uproject` requests engine 5.8; the three Editor/Game/Server targets exist.
4. Require exactly 293 frozen candidate binary package paths and **zero Git LFS pointer placeholders**.
5. Validate the supplied engine's `Engine/Build/Build.version` is 5.8, the host-specific `Build.sh`/`Build.bat` exists, and the engine or project contains both enabled `.uplugin` descriptors.
6. Default to **preflight only**. Do not launch Unreal without an explicit `--execute`.
7. On `--execute`, call each of `MORABEZAEditor`, `MORABEZA`, `MORABEZAServer` in Development configuration through the host's Unreal `Build.sh`/`Build.bat`, using argv (no shell interpolation). Capture separate target logs, exit codes and log SHA-256 digests in a local JSON evidence file. Stop at the first failed build.
8. Even if all three build commands exit zero, report `TARGET_BUILDS_ZERO_EXIT_REVIEW_REQUIRED` rather than claiming the INT-02 gate is approved. Reviewers must inspect UHT/compilation logs, expected binaries, engine/compiler provenance and warnings.

The JSON evidence output is written under `Saved/INT02/int02-build-evidence.json` by default. It is local, must be reviewed/redacted before publication and is not automatically uploaded to GitHub or an external service.

### Verified blocked preflight

The **exact script now committed to this PR** was first executed against the extracted user ZIP. Its Git blob ID is `9edcc1c00de5a369bfcbf481314a2a21c7e6ae24`, verified against GitHub after upload. The preflight was run with the archive's advertised SHA and no engine root, intentionally without `--execute`. Actual result:

```text
INT-02 BLOCKED: GIT_COMMIT_NOT_PROVABLE_FROM_SOURCE_ARCHIVE, LFS_PAYLOADS_MISSING:293, UE_ENGINE_ROOT_UNAVAILABLE
EXIT_CODE=2
REPORT_STATUS=BLOCKED_NO_UNREAL_BUILD
MANIFEST_PASS=True
ASSET_PATHS=293 LFS_PLACEHOLDERS=293
BUILD_LOGS=[]
```

This is a **successful fail-closed preflight**, not a failed UE build. A ZIP cannot satisfy the pinned-Git-checkout requirement; an actual Git clone is required on the eventual Unreal build host.

### Test execution

`tests/mmo/test_int02_unreal_build_gate.py` contains five offline tests for build-command argv, missing Git metadata, plugin discovery, missing engine/LFS refusing execution, and wrong UE minor version refusing execution. The exact file committed here has Git blob ID `a06327c12a844f4876d936dc54288652e33380b5`, matched against the locally tested file.

- New INT-02 offline suite: **5/5 PASS**.
- All offline suites on the extracted candidate plus the new two INT-02 files: **28/28 PASS**, exit 0.
- Python syntax compilation of both new files: PASS.
- UnrealBuildTool, UHT, C++ target compilation, executable launch and network playtest: **NOT EXECUTED**.

No GitHub Actions workflow was added or invoked. The five tests use temporary fixtures and synthetic engine descriptors to test *rejection behavior*; they are not a real-engine proof.

## 3. Reproducible build-host procedure (future)

Use a properly provisioned Linux, macOS or Windows build host with Epic-authorized UE 5.8 and its appropriate platform compiler/toolchain. A dedicated-server build may require a source-built engine or another engine distribution that actually supports the server target. Do not assume an installed launcher binary can build it.

From an exact checkout of this review branch, hydrate Git LFS **before** the build gate:

```bash
git clone --branch verification/int-02-ue58-build-gate https://github.com/node63labs/morabezza-game.git
cd morabezza-game
git lfs install --local
git lfs pull

# Source-only regression gate (does not require Unreal):
python3 scripts/mmo/verify_s0_integration.py --run-tests

# Default mode: check all prerequisites, produce an evidence JSON, NO BUILD.
python3 scripts/mmo/int02_unreal_build_gate.py \
  --engine-root "/ABSOLUTE/PATH/TO/UE_5.8" \
  --candidate-sha "$(git rev-parse HEAD)"

# Only after preflight is READY and the build host is explicitly authorized:
python3 scripts/mmo/int02_unreal_build_gate.py \
  --engine-root "/ABSOLUTE/PATH/TO/UE_5.8" \
  --candidate-sha "$(git rev-parse HEAD)" \
  --execute
```

The displayed shell snippet is appropriate for Bash-compatible Linux/macOS hosts; use the equivalent Windows PowerShell quoting, paths and SHA retrieval on Windows. The Python script itself selects the correct platform build entrypoint. The engine path is a placeholder, not an installed path confirmed in this conversation.

Build acceptance requires a reviewed evidence set containing the exact Git revision, engine build version, platform compiler/SDK versions, installed plugin versions, three build-command logs, UHT-generated-code diagnostics, target exit codes, and verified Editor/Game/Server output paths. The script intentionally does not infer actual binary locations or claim acceptance from exit code alone.

## 4. Formal gate status and exit conditions

| Gate | Current outcome |
| --- | --- |
| INT-01 offline integration source test | **PASS**, previously accepted |
| INT-02a target/source preflight script and its negative tests | **PASS**, offline only |
| INT-02b Git checkout + 293 hydrated assets | **BLOCKED** on uploaded ZIP (not a clone; all packages are LFS pointers) |
| INT-02c UE 5.8 engine, toolchain and `EnhancedInput`/`MoverExamples` plugin availability | **BLOCKED** in current sandbox |
| INT-02d Editor/Game/Server UHT + C++ builds with logs | **NOT EXECUTED** |
| INT-02 approval | **NOT GRANTED** |
| INT-03/04 dedicated server + two-client runtime | **NOT EXECUTED** |
| Promotion/merge to `main` | **NOT AUTHORIZED** |

**Next action:** obtain an authorized UE 5.8-capable build host, provide a complete Git+LFS checkout of the pinned candidate, run preflight, then explicitly perform the three builds and review captured logs. Do not generate a fake build-success artifact or promote this branch based on static checks. Neither GitHub Actions nor the user's former local computer is required for offline source verification; a real Unreal host **is** required for INT-02 compilation.
