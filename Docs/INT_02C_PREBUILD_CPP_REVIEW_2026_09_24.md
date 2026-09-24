# INT-02C — Pre-build Unreal C++ and UHT Source Review

**Date:** 2026-09-24  
**Repository:** `node63labs/morabezza-game`  
**Review branch:** `review/int-02c-prebuild-cpp`  
**Base at branch creation:** draft INT-02 PR #9, commit `7f92c7a120026c0cb4a3cfaff34c6cbece068e12`  
**Gate status:** **SOURCE REMEDIATION PREPARED; UE 5.8 COMPILATION STILL BLOCKED / UNVERIFIED**  
**Integration policy:** draft PR only; no GitHub Actions, no `main` merge, no game content/map/asset/secret changes.

## 1. Evidence basis and limitations

The source was reviewed from the user-supplied S0 integration ZIP and live GitHub PR #9 branch. The ZIP is a source archive (not a Git checkout) with 293 LFS pointer placeholders and no UE 5.8 engine. The three amended source files, the new regression suite, the S0 verifier and the INT-02C overlay were reconstructed locally and independently matched against their live GitHub Git blob IDs before offline tests.

The review inspected build targets, module dependencies, reflected headers, `LandscapeDiagnosticCommandlet`, character input, contact/RPC, HUD and dialogue source. It also checked relevant **Epic Unreal Engine 5.8 API documentation** for API boundaries. These checks are not equivalent to UHT, UnrealBuildTool or C++ compilation.

## 2. Findings and changes

| Ref | Source-derived issue / observation | Resolution |
| --- | --- | --- |
| C-01 | `LandscapeDiagnosticCommandlet.cpp` called `Proxy->GetActorLabel()` directly from the `MORABEZA` runtime module compiled by all three Game/Editor/Server targets. Actor labels are not consistently available in non-editor/package builds. | Replaced the diagnostic display name with `Proxy->GetName()`, a cross-target UObject API. This intentionally uses an object name rather than the friendly editor label. See [Epic actor API](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/Engine/AActor) and [Epic packaging discussion](https://forums.unrealengine.com/t/no-member-named-getactorlabel-in-aactor-when-packaging-for-android/389241). **Portability remediation is source-reviewed; no actual compile evidence yet.** |
| C-02 | `LandscapeDiagnosticCommandlet.h` pulled `LandscapeProxy.h` and `LandscapeComponent.h` into a reflected runtime header even though only pointer parameters are declared. | Kept `LandscapeDiagnosticCommandlet.generated.h` as its last include and forward-declared `UWorld`, `ALandscapeStreamingProxy` and `ULandscapeComponent`. Implementation retains explicit complete-type includes. |
| C-03 | `MORABEZACharacter.cpp` used `ULocalPlayer::GetSubsystem` via transitive includes. | Added explicit `Engine/LocalPlayer.h` (IWYU) without changing input behavior. |
| C-04 | Existing target files for `MORABEZA`, `MORABEZAEditor` and `MORABEZAServer` target the same runtime module and declare UE 5.8 include order. `MORABEZA.Build.cs` includes `EnhancedInput`, `UMG`, `Slate`, `SlateCore` and `Landscape`. | Source-level cross-target consistency observed. **Target and plugin compilation remain unverified** on a real UE 5.8 installation. |
| C-05 | Existing reflected `UCLASS`/`UINTERFACE` headers place their matching `*.generated.h` as the final `#include`. Server RPC, pawn authority and contact owner guard are still declared in the reviewed source. | Added regression assertions; these are structural observations, not UHT or networking acceptance. |
| C-06 | The original S0 frozen manifest correctly records 21 files from PRs #4–#7, but this review necessarily changes `MORABEZACharacter.cpp`. Running its old byte-for-byte verifier unmodified would correctly fail. | **Preserved the INT-01 manifest unchanged.** Added a strictly pinned INT-02C source overlay with three reviewed `before`/`after` Git blob hashes and one new test file. Updated the existing verifier to accept *only that explicit overlay*, verify the original manifest's Git blob identity and reject unexpected changed paths. |

### Remaining compile-sensitive questions (not inferred as failures)

The actual UE 5.8 build must check generated RPC declarations, UHT reflection, full module-link compatibility, editor/game/server linkage, the enabled `MoverExamples` and `EnhancedInput` plugins, and compiler/SDK-specific diagnostics. The diagnostic commandlet's hardcoded `/Game/dskjnfkljsdnf` path remains historical and **must not** establish canonical world authority. The source-only review does not prove the replicated private-contact dialogue or local input works with two clients.

No broad refactor of input assets, UI construction, GameMode spawning, RPC authorization, Landscape worlds or production economy was performed.

## 3. Immutable source provenance

INT-01's baseline manifest remains byte-for-byte intact at Git blob SHA `0c94f2584425469d5acbef7c212196037f8b1754`. The new `Docs/INT_02C_SOURCE_OVERRIDES.json` records the exact changed blobs:

| Path | Reviewed new Git blob ID |
| --- | --- |
| `MORABEZACharacter.cpp` | `5ad9fc1d2809ea2506ec441da094f2611765f785` |
| `LandscapeDiagnosticCommandlet.cpp` | `3091a76fded46c39ab0d5177ca3a724bb232f0fd` |
| `LandscapeDiagnosticCommandlet.h` | `4b1e29afeb3087ca7e5a6a28fb62abf17914e8c7` |
| `tests/mmo/test_int02c_prebuild_source.py` (added) | `9dc6e479951d48fe744ae796c33fbba7426bdf3f` |

The updated verifier is at blob `d70e72a50fc9546d282caf8f22b7992afb6b5b77`; the overlay is `27992690bef79ab73a5470736c47c5144edcd913`. The original 21-path manifest remains the INT-01 record; the INT-02C verification contract checks **24** source paths (the original 21 with the reviewed character amendment, two commandlet files and one new test file). This avoids misrepresenting amended source as the original PR #7 snapshot.

## 4. Actual offline results

The review ran against locally reconstructed files whose six modified/new Git blob IDs match the corresponding branch files. The local archive does **not** include the two new Python files from PR #9, so its test discovery consists of the S0-W0 (6), S0-W1b (8), S0-W1c (9) and INT-02C (7) suites; the PR #9 INT-02 preflight suite (5) must be rerun against a full checkout.

| Check | Recorded result |
| --- | --- |
| Pinned overlay + immutable S0 manifest byte identity | **PASS** |
| File content hashes for reviewed amended source | **PASS — 24/24** |
| INT-02C source regression suite | **PASS — 7/7** |
| Locally available complete source-test discovery | **PASS — 30/30**, no errors/failures |
| `bash -n` two-client harness | **PASS** |
| `py_compile` new source test and revised verifier | **PASS** |
| Negative fixture: modified `MORABEZACharacter.cpp` | **REJECTED**, verifier exit 1 |
| Negative fixture: altered overlay base SHA | **REJECTED**, verifier exit 1 |
| Restored exact source/overlay | **PASS**, verifier exit 0 |
| Full PR #9 + INT-02C suite on actual Git checkout (35 expected methods) | **NOT EXECUTED HERE** |
| UE 5.8 UHT, C++ Editor/Game/Server builds | **NOT EXECUTED** |

All tested code is standard-library Python or source-only C++ inspection; **none** of these checks substitute for compilation.

## 5. Gate and handoff

This review branch is **stacked on draft PR #9**, not `main`, with narrow changes: three C++ files, one offline source-test file, one pinned overlay JSON, the source verifier update and this evidence document. PR #9's fail-closed build gate can invoke the revised verifier unchanged; its optional `--execute` remains explicitly opt-in.

On a fresh full Git checkout of this branch, with Git LFS hydrated and UE 5.8 available, run `python3 scripts/mmo/verify_s0_integration.py --run-tests`, then the existing INT-02 preflight with an exact `--candidate-sha` before any authorized build execution. Record real UHT, compiler, target/link and binary evidence. **INT-02 remains blocked until a real UE host compiles all targets.** PR #8, PR #9 and this review candidate must remain draft; no direct `main` push or integration acceptance is authorized by this source review.
