# RM-01 — Repository Baseline and Recovery Audit

**Project:** MORABEZA / MORABEZZA  
**Repository:** node63labs/morabezza-game  
**Audit date:** 2026-09-24  
**Audit class:** Read-only repository and recovery preflight  
**Status:** SOURCE INVENTORY COMPLETE — CLEAN RECOVERY NOT YET VERIFIED  
**Audited branch:** main  
**Audited commit:** af8f3d0e5f9291b1358232c7055190e4bd57b9f6  
**Audited tree:** 934b017735844baf6108f7971658438c95c57c26  

This document records what the remote repository demonstrably contains. It does not assert that the local SSD is synchronized, that Git LFS objects can be fetched, that Unreal builds, or that either candidate map is the authoritative production world.

## 1. Evidence scope and method

- Read-only inspection of GitHub repository metadata, main branch and the recursive Git tree at the audited commit.
- Inspection of the project descriptor, README, game-design document, Git attributes, Git ignore rules, engine configuration and selected C++ source.
- Inspection of Git LFS pointer text for both maps and the interaction-prompt asset.
- No Unreal Editor execution, clean clone, Git LFS binary download, compilation, project generation, gameplay execution, local SSD inspection or production mutation.
- No repository configuration, visibility, branch protection, map, asset or source-code change is authorized by this audit.

## 2. Repository authority and governance

| Item | Observed state |
| --- | --- |
| Repository | node63labs/morabezza-game |
| Visibility | PUBLIC |
| Default branch | main |
| Main commit | af8f3d0e5f9291b1358232c7055190e4bd57b9f6 |
| Main commit date | 2026-08-30 |
| Main commit subject | Add landscape diagnostic commandlet |
| Main branch protection | Not enabled at audit preflight |
| Open PRs at audit preflight | 0 |
| GitHub Actions workflows in audited tree | None identified |
| Audit branch | docs/rm-01-repository-recovery-audit |

The repository's public visibility is an observed fact, not approval to publish additional assets, unpublished designs or confidential configuration. This audit does not change visibility or merge into main.

## 3. Complete remote-tree inventory

The Git tree response was not truncated.

| Inventory | Count |
| --- | ---: |
| Git tree entries, including directories | 677 |
| Tracked file blobs | 335 |
| Unreal binary-file paths (.uasset and .umap) | 293 |
| .uasset paths | 291 |
| .umap paths | 2 |
| Source/ tracked files | 34 |
| Config/ tracked files | 3 |
| Docs/ tracked files | 1 |

The 293 Unreal binary-file paths have Git blob sizes of 129–131 bytes, consistent with Git LFS pointers, not actual playable Unreal asset payloads. Both map pointers and one UI asset pointer were inspected directly. **Remote LFS payload availability and integrity have NOT been proven.**

Representative LFS evidence:

| Tracked path | LFS object SHA-256 | Pointer-declared payload size |
| --- | --- | ---: |
| Content/Untitled.umap | 10674d9001fdade5925e0a42ea91bb6bf7dcb4bd325a6251ce5d1bb044fce4ed | 12,968 bytes |
| Content/dskjnfkljsdnf.umap | 78785976c239438638a52fdb6ebd9570caeb0c733cb708df58eb07fa448d0406 | 163,122 bytes |
| Content/Content_UI/WBP_InteractionPrompt.uasset | 74edf426db32cc45408da4db80f9d84ce7d1d1898bbb462d99b30a06913f05eb | 26,101 bytes |

The Git attributes file declares Git LFS filters for Unreal maps/assets and several other binary formats. The Git ignore file excludes common Unreal generated directories (Binaries, DerivedDataCache, Intermediate and Saved).

### 3.1 Candidate maps and associated packages

- Candidate maps tracked: Content/Untitled.umap and Content/dskjnfkljsdnf.umap.
- Content/__ExternalActors__/Untitled/: 139 tracked files.
- Content/__ExternalActors__/dskjnfkljsdnf/: 139 tracked files.
- Content/__ExternalObjects__/Untitled/: 5 tracked files.
- Content/__ExternalObjects__/dskjnfkljsdnf/: 5 tracked files.
- Config/DefaultEngine.ini currently selects /Game/dskjnfkljsdnf for game and Editor startup.

The selected startup map is **not** proof of canonical world authority. Equal actor-file counts do not prove package equivalence, Landscape GUID consistency, complete cell coverage, or correct World Partition ownership. Do not rename, delete, regenerate or consolidate either map's binary packages based on this inventory.

### 3.2 Source foundation

The audited tree includes:

- MORABEZACharacter and gameplay movement/input definitions.
- MORABEZAInteractionComponent and MORABEZAInteractable.
- MORABEZAGameMode, MORABEZAHUD and MORABEZAPlayerState.
- Dialogue, interaction widgets, a contact actor and a SaveGame header.
- MORABEZAProceduralCharacterComponent.
- LandscapeDiagnosticCommandlet.
- Five tracked historical DialogueWidget source backup/snapshot files despite ignore patterns for backups.

Presence in the tree is source inventory evidence, not a successful compile, multiplayer qualification or gameplay acceptance result.

## 4. Engine and dependency preflight

The authoritative tracked MORABEZA.uproject declares **EngineAssociation 5.8** and enables **EnhancedInput** and **MoverExamples**. The README still refers to Unreal Engine 5.6 and an earlier gameplay milestone. The README and Docs/GAME_DESIGN.md are not reliable evidence of the latest proposed product scope or of a tested current runtime.

Before clean recovery, verify availability and compatible installation of the exact Unreal version and required plugins; inspect any additional local plugins or assets that exist only on the SSD. Do not modify the .uproject simply to make an incompatible installation launch.

## 5. Recovery blockers and evidence classifications

| ID | Requirement | Status |
| --- | --- | --- |
| RM01-01 | GitHub main commit and complete file-tree inventory | PASS — SOURCE |
| RM01-02 | Git LFS tracking rules and representative pointers | PASS — SOURCE |
| RM01-03 | All remote LFS payloads downloadable | NOT VERIFIED |
| RM01-04 | Fresh-clone LFS object integrity | NOT EXECUTED |
| RM01-05 | Local SSD branch, commits, uncommitted files and additional assets reconciled | NOT EXECUTED |
| RM01-06 | Unreal 5.8 and required plugins available on recovery host | NOT EXECUTED |
| RM01-07 | Clean project generation and C++ compile | NOT EXECUTED |
| RM01-08 | Editor opens both candidate maps without missing assets | NOT EXECUTED |
| RM01-09 | Landscape / World Partition / external-actor authority established | BLOCKED |
| RM01-10 | Player spawn, traversal and interaction proven in canonical world | BLOCKED |
| RM01-11 | CI/build pipeline and repository protection policy review | OPEN |
| RM01-12 | Tracked configuration and public-distribution security review | OPEN — OWNER REVIEW |

**Recovery gate: NOT ACCEPTED. M0 world authority remains unresolved.**

## 6. Next machine: safe recovery sequence

### Phase A — Preserve the SSD before modifying anything

1. Mount the SSD and create a second copy of the entire project, including hidden Git metadata, Unreal assets, configuration and any external project asset folders.
2. Do not run hard reset, force push, bulk cleanup, map conversion, plugin upgrade or Unreal auto-save on the only copy.
3. Record the local project path, current Git branch, commit SHA, working-tree state, remotes, Git LFS status and untracked/ignored asset inventory. Keep these observations in an owner-controlled location.
4. Compare the preserved SSD state to the audited remote main commit. Local-only commits and asset changes must be reconciled explicitly, not overwritten.

### Phase B — Test an independent GitHub recovery

On the new machine, create a *separate, empty* recovery directory; do not clone over the SSD working tree.

    git lfs version
    git clone https://github.com/node63labs/morabezza-game.git morabezza-recovery
    cd morabezza-recovery
    git rev-parse HEAD
    git lfs ls-files --long
    git lfs pull
    git lfs fsck
    git status --short

Record the actual command outputs and failures. On a recovery checkout of audited main, compare HEAD against af8f3d0e5f9291b1358232c7055190e4bd57b9f6 and compare the LFS file inventory with the 293 tracked Unreal asset paths. Git LFS successful download and integrity verification are distinct from merely seeing pointer files in GitHub.

### Phase C — Prove the Unreal project and world

1. Install/identify Unreal Engine 5.8, platform SDKs and the required project plugins.
2. Generate project files and compile the MORABEZA Editor target; capture engine, compiler and exact source SHA.
3. Open the cloned project in the Editor without modifying either candidate production map.
4. Run the existing Landscape diagnostic commandlet against both candidate worlds; capture Landscape actors/proxies/components, GUIDs, coordinates, ownership and external actor observations.
5. Compare the candidate worlds against the preserved SSD source; select canonical authority through a separate reviewed decision, not from filenames or the startup-map setting alone.
6. Once authority is established, validate player spawning, terrain traversal, interaction and World Partition runtime streaming.
7. Record test evidence before updating M0 acceptance status.

## 7. Follow-up change boundaries

- No production-world or binary-asset mutation is part of RM-01.
- No README, gameplay scope or engine-version change is part of this evidence-only PR.
- Resolve tracked source backups, CI, visibility/protection and configuration-security findings in separately reviewed changes.
- Do not promote a proposed MMO architecture or any local SSD-only feature to implemented status from repository presence alone.

## 8. RM-01 closure criteria

RM-01 may close only when **both** the remote recovery and the SSD reconciliation are documented, or an explicit reviewed decision records that local-only data is irrecoverable and defines the accepted loss. Required final evidence: exact authoritative SHA, complete LFS fetch/integrity results, local/remote delta register, clean Unreal build and Editor launch, candidate-map diagnostics, and a bounded recovery procedure.

**Current disposition:** source inventory available for remote architecture and code review; full Unreal recovery and canonical-world acceptance remain pending a suitable machine and SSD access.
