# INT-02E — Unreal Engine 5.8 Build-host Readiness

**Date:** 2026-09-25  
**Repository:** `node63labs/morabezza-game`  
**Candidate branch:** `verification/int-02e-build-host-readiness`  
**Exact review parent:** draft PR #10 head `3c223c4b84681e3d7fed2aa759ad8ab4da446b43`.  
**Gate classification:** **HANDOFF PREPARED; HOST BLOCKED; INT-02 ENGINE BUILD NOT ACCEPTED**.  
**Authority:** separate stacked **draft** PR, no changes to `main`, no deployment, no paid host ordered and no GitHub Actions executed.

## 1. Repository-derived facts, not build-success claims

At the reviewed PR #10 baseline:

- `MORABEZA.uproject` declares Unreal `EngineAssociation: 5.8`; its enabled project plugins are `EnhancedInput` and `MoverExamples`.
- Editor, Game and Server target definitions exist and use `EngineIncludeOrderVersion.Unreal5_8`. The game module declares `EnhancedInput`, `UMG`, `Slate`, `SlateCore` and `Landscape`.
- The project has **293** tracked `.uasset`/`.umap` paths; all **293** are LFS *pointer placeholders* in the uploaded GitHub ZIP, which is not a Git checkout.
- The GitHub tree contains no `Plugins/` directory or vendored `MoverExamples` plugin, so the selected engine distribution must supply it, or an approved project plugin must be separately provisioned.
- The two tracked `Content/Untitled.umap` and `Content/dskjnfkljsdnf.umap` files are **not** authoritative test maps; INT-02E does not cook them, resolve RM-01, or create a disposable development zone.

**External requirements verified from Epic's UE 5.8 documentation:** [dedicated server setup](https://dev.epicgames.com/documentation/unreal-engine/setting-up-dedicated-servers-in-unreal-engine) uses a **source build** and a C++ project; [Mover Examples](https://dev.epicgames.com/documentation/unreal-engine/mover-examples-in-unreal-engine) instructs developers to enable **Mover and Mover Examples**, calls the feature experimental and says example content is not intended directly for shipping. The current `.uproject` explicitly enables only `MoverExamples`, not `Mover`. **Do not silently change this file:** inspect the actual `MoverExamples.uplugin` dependency graph on the selected UE 5.8 host and obtain an owner-reviewed follow-up if an explicit Mover enablement change is required. A plugin descriptor's existence does not prove that plugin source or sample assets compile.

## 2. Proposed host profile, with no procurement authorization

The build-host owner must supply an authorized **UE 5.8 source-engine checkout/build** that supports the Server target, its UnrealBuildTool and UHT, a platform C++ compiler/SDK and the three required plugin descriptors (`EnhancedInput`, `Mover`, `MoverExamples`). Do not upload or commit licensed engine source/binaries or Epic account credentials into the public game repository.

- **Windows:** select the compiler/SDK supported by the exact UE 5.8 engine build. Epic's [UE 5.8 release notes](https://dev.epicgames.com/documentation/unreal-engine/unreal-engine-5-8-release-notes) recommend Visual Studio 2026, list Visual Studio 2022 v17.14 as a minimum, MSVC 14.38 minimum and Windows SDK 10.0.22621.0 minimum; confirm the exact installed versions on the host. Windows source-engine prerequisites and UnrealBuildTool must be verified before attempting the server target.
- **Linux:** Epic's [UE 5.8 Linux requirements](https://dev.epicgames.com/documentation/unreal-engine/linux-development-requirements-for-unreal-engine) recommend Ubuntu 22.04 for development and the Clang 20.1.8 toolchain; `Setup.sh` can supply the matching native engine toolchain. Verify actual bundled compiler/SDK versions rather than assuming whichever `clang++` is on `PATH` is the one UBT uses.
- **macOS:** supported only with the exact compatible UE 5.8 source build, Xcode/SDK and toolchain. Do not assume the user's former Mac or external SSD is available.

Epic's [UE 5.8 hardware requirements](https://dev.epicgames.com/documentation/unreal-engine/hardware-and-software-specifications-for-unreal-engine) recommend 32 GB RAM for Windows development. Available SSD capacity, engine installation size and actual Git LFS transfer volume **have not been measured**; check them before selecting or paying for a build host. A headless compile host is not automatically suitable for later two-graphical-client gameplay tests.

**The current available sandbox is not an Unreal build host.** It has no UE 5.8 source engine; the uploaded source ZIP is neither an actual `.git` checkout nor a hydrated LFS working tree. No remote machine, cloud subscription or GitHub Actions usage was started.

## 3. Git and Git LFS recovery — future, on an authorized host

Do not use GitHub's **Download ZIP** as the build source. It contains pointer text for LFS assets and cannot attest the exact `git rev-parse HEAD` commit. The host owner must approve storage/network expenditure before these transfer steps.

**Linux/macOS Bash example** (replace the reviewed SHA and engine path with verified values):

```bash
git clone --branch verification/int-02e-build-host-readiness \
  https://github.com/node63labs/morabezza-game.git
cd morabezza-game

# Freeze the exact, owner-reviewed PR #11 head; a branch name alone can move.
git checkout --detach <PINNED_PR11_COMMIT_SHA>
git lfs install --local
git lfs pull origin
git lfs fsck --objects --dry-run

git rev-parse HEAD
git status --porcelain
git lfs version
```

**Windows PowerShell:** use the same `git clone`, detached SHA checkout, `git lfs install --local`, `git lfs pull origin` and `git lfs fsck --objects --dry-run` commands from a PowerShell terminal. Configure Epic-authorized UE 5.8 source and supported MSVC/Windows SDK on the host; there is no repository-supplied engine.

The project's `.gitattributes` also marks image, audio and 3D source extensions for LFS. Thus the 293-package check is a **minimum frozen Unreal-package scope**, not a proof that every other tracked LFS object is healthy. A successful `git lfs fsck --objects --dry-run` checks local LFS object consistency without moving corrupted objects; record its exit code and redact any sensitive local paths before sharing evidence. Git LFS reference: [git-lfs-fsck](https://github.com/git-lfs/git-lfs/blob/main/docs/man/git-lfs-fsck.adoc).

If the LFS objects cannot be downloaded or verified, STOP and return to the separate RM-01 recovery/SSD authority decision. Do not fabricate assets, replace the two unresolved maps, or promote the GitHub ZIP as a complete world.

## 4. Safe host-readiness preflight — no Unreal execution

This draft adds `scripts/mmo/int02e_host_readiness.py`, a **read-only** inventory that records an exact Git HEAD, clean working tree including untracked files, available Git LFS, 293 asset paths and pointer counts, UE 5.8 `Build.version`, build-script entry point, source-tree markers, the three plugin descriptors and compiler availability. It optionally runs `git lfs fsck --objects --dry-run`. It **does not** call `git clone`, `git lfs pull`, UnrealBuildTool, `Build.sh`, game clients or any paid service. It writes local JSON under ignored `Saved/INT02E/` by default.

On the actual checkout (Bash example):

```bash
PINNED_SHA="$(git rev-parse HEAD)"
UE_ROOT="/ABSOLUTE/PATH/TO/UE_5.8_SOURCE"

python3 scripts/mmo/verify_s0_integration.py --run-tests

python3 scripts/mmo/int02e_host_readiness.py \
  --engine-root "$UE_ROOT" \
  --candidate-sha "$PINNED_SHA" \
  --verify-lfs-objects
```

On Windows, use `py -3 scripts/mmo/int02e_host_readiness.py --engine-root "D:\UE_5.8_SOURCE" --candidate-sha (git rev-parse HEAD) --verify-lfs-objects`; ensure that Python, Git LFS and the correct source engine are available. The printed engine directory is an example, not a host path that has been verified.

**Exit codes:** `2` = blocking prerequisites; `0` = `HOST_PREFLIGHT_ELIGIBLE_REVIEW_REQUIRED` only. Neither result is a UE build, and exit 0 is **not** permission to compile or merge. Source-build marker directories cannot prove that a source engine is usable for the specific server target; UBT and compiler execution must validate it.

On the uploaded ZIP, the actual script returned `BLOCKED` / exit 2 with `EXACT_GIT_CHECKOUT_REQUIRED`, `LFS_PAYLOADS_NOT_HYDRATED:293` and `UE58_ENGINE_ROOT_MISSING`; it executed **zero** builds and zero network actions.

## 5. INT-02 actual build handoff (separate owner authorization)

Only when host preflight is eligible, plugin dependency review is resolved and an owner approves the cost/scope, run the **existing** opt-in INT-02 build gate on the same pinned SHA and host:

```bash
python3 scripts/mmo/int02_unreal_build_gate.py \
  --engine-root "$UE_ROOT" \
  --candidate-sha "$PINNED_SHA"

# Additional owner approval required; deliberately NOT run here:
python3 scripts/mmo/int02_unreal_build_gate.py \
  --engine-root "$UE_ROOT" \
  --candidate-sha "$PINNED_SHA" \
  --execute
```

The second command targets `MORABEZAEditor`, `MORABEZA` and `MORABEZAServer` in Development configuration and captures separate build logs. A zero exit still requires review of UHT diagnostics, full link output, compiler/SDK and engine provenance, actual binary paths and warnings. Do not confuse a built Game target with a validated Client target; no dedicated `MORABEZAClient.Target.cs` is in the reviewed repository. Cooking, maps and multiplayer execution remain later gates.

Do **not** put Epic credentials, engine archives, entire engine source or sensitive local paths into a public PR, Actions artifact or this readiness JSON. The JSON includes local filesystem paths and must be reviewed/redacted before publication.

## 6. Verification performed in INT-02E

The new Python host-readiness script and seven fixture-based tests were executed against the previously uploaded source tree (the GitHub ZIP augmented by **exact GitHub blob-matched** PR #9/#10 files from INT-02D). The new script and test files were independently compared with their GitHub Git blob IDs. The frozen INT-01 source manifest and the INT-02C pinned overlay remain **unchanged**.

| Gate item | Current result |
| --- | --- |
| New INT-02E host-readiness fixture tests | **7/7 PASS** (including blocked ZIP, successful synthetic host, wrong SHA/engine, missing plugin/server target and LFS fsck rejection) |
| Complete offline Python suites: S0-W0 + W1b + W1c + INT-02 + INT-02C + INT-02E | **42/42 PASS**, no failures |
| Effective INT-02C source manifest | **24/24 PASS** |
| Bash syntax, Python syntax | **PASS** |
| Local uploaded-ZIP host inventory | **BLOCKED** as expected; 293 LFS pointers, no `.git`, no UE root |
| Full Git+LFS checkout and source engine on a designated host | **NOT AVAILABLE / NOT PROVEN** |
| Real UE 5.8 Editor/Game/Server UHT and C++ build | **NOT EXECUTED** |
| Game-session and two-client runtime | **NOT EXECUTED** |
| Promotion or merge to `main` | **NOT AUTHORIZED** |

The full offline regression log SHA-256 is `205310a8872ed9cfed374110df14b72e93ad6862864f8fde1a57cb6425effa9b`. It is **not** a UE build log.

**INT-02E decision:** handoff and fail-closed inventory prepared; genuine build-host readiness remains conditional on approved hardware and recovered assets. INT-02 engine compilation and INT-03/04 runtime remain open; this PR stays draft and stacked on PR #10.
