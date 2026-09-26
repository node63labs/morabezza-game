# MORABEZZA — Milestone and Public-Documentation Transition

**Document ID:** MORABEZZA-RM-03  
**Date:** 2026-09-26  
**Status:** DRAFT / FOUNDER REVIEW CANDIDATE — NOT M0 OR MMO-S0.1 ACCEPTANCE  
**Change class:** Documentation and milestone reconciliation only  
**Change boundary:** Public README, historical design labeling and acceptance vocabulary. No Unreal code, assets, maps, deployment, costs, portfolio allocation or game `main` merge.

## 1. Why this transition exists

The original `README.md`, `Docs/GAME_DESIGN.md` and open [Issue #1](https://github.com/node63labs/morabezza-game/issues/1) described a third-person action/life-sandbox with an Unreal Engine **5.6** Praia scene, early `Volta` mission, vehicles, local money/save-game state and M0.1/M0.2 acceptance. The tracked `MORABEZA.uproject` now declares **5.8**, and the founder-recorded AD-01/AD-02 decisions establish an elevated/isometric shared-world **MMORPG design target** with distinct player characters and Morabezza as central story heroine.

These are different *product and evidence stages*. Updating terminology must not retroactively accept the old milestones, erase their source history or claim the proposed MMO runtime exists.

**Dependency:** this document is a separate draft stacked on [RM-02 review PR #12](https://github.com/node63labs/morabezza-game/pull/12), itself stacked on [RM-02 architecture PR #3](https://github.com/node63labs/morabezza-game/pull/3). If those decisions change during review, reconcile this draft before any merge.

## 2. Source and decision hierarchy

| Item | Role in this transition | Current authority limit |
| --- | --- | --- |
| `main@af8f3d0e5f9291b1358232c7055190e4bd57b9f6` | Last inspected integrated C++ and UE 5.8 source baseline | Not proof of multiplayer, clean build or world integrity |
| [RM-01 draft PR #2](https://github.com/node63labs/morabezza-game/pull/2) | Independently checked remote Git LFS recovery and source audit | Does not verify the former SSD workspace, Unreal compilation or canonical map |
| [AD-01](AD_01_PLAYER_CHARACTERS_AND_STORY_HEROINE_DECISION_2026_09_25.md) | Founder-recorded separate player characters and central story heroine | No approved account schema or heroine gameplay role |
| [AD-02](AD_02_MMO_PRODUCT_DEFINITION_AND_LEGACY_SCOPE_DECISION_2026_09_25.md) | Founder-recorded MMO direction; Volta/vehicles retained for later MMO gameplay | Not gameplay implementation or an approved production release |
| [AD-03](AD_03_DEDICATED_SERVER_AND_FIRST_DEV_ZONE_DECISION_2026_09_25.md) | Founder-approved *bounded design*: one server, one disposable dev zone, two clients | No actual map, compilation, hosting, admission or two-client acceptance |
| PC-01 / [governance draft PR #15](https://github.com/node63labs/node63-governance/pull/15) | Founder classification of MORABEZZA as a game product distinct from hospitality Morabeza | Charter integration and resource/priority authorization are separate |
| Existing `Docs/GAME_DESIGN.md` and Issue #1 | Original design/prototype evidence; retained for traceability | Historical M0.1/M0.2 requirements do not automatically become MMO-S0.1 acceptance |

Do not promote source-only test results, a design approval or a public announcement to integrated runtime acceptance.

## 3. Historical milestones and prospective MMO gates

**Historical M0.1/M0.2:** retain the original definition and issue history. The original vehicle, Volta, money and local save/load requirements are *not* a mandatory first multiplayer-slice checklist. Do not mark them passed, destroy the supporting prototype or start a separately maintained legacy game by implication.

**RM-01 world integrity:** a parallel, still-open engineering authority question. The tracked `Content/Untitled.umap` and `Content/dskjnfkljsdnf.umap` are unresolved canonical-world candidates. Landscape, streaming-proxy, World Partition and external-actor integrity, clean reconstruction and production-world freeze need actual engine evidence. Neither map is authorized as an MMO test map merely because it exists.

**S0-W0/W1 and INT-01/02:** independent draft source/harness work and limited offline checks. Existing draft PRs must retain their exact acceptance scope. A server-target source file or liveness script is not a successful server compile, connected two-client game, authentication or owner isolation in runtime.

**Initial two-client runtime proof:** the first bounded AD-03 design proposes one UE 5.8 dedicated-server process, two independently connected clients and the *planned* disposable map `/Game/Dev/MMO_S0_TestZone`. The map package is not yet verified. AD-05 dev identity/admission, AD-06 test-world authorization, a compatible build host and executable client/server evidence remain prerequisites at their respective gates.

**MMO-S0.1 — proposed future full vertical slice:** a bounded development zone with two distinct player characters, owner-isolated NPC/contact interaction, one resource, one enemy, a durable owner-only reward and elevated/isometric presentation. It requires AD-04 durable game authority, AD-05 session/character authority, actual UE runtime and negative multiplayer tests. This is a *target*, not the current repository state. Volta and drivable vehicles come later, under separate design and verification.

Production-zone expansion, whole-island content, cross-zone handoff, massive concurrency, paid hosting and release decisions are **outside** this migration.

## 4. Current follow-up gates

1. **Review and integrate product authority separately:** PC-01 governance charter review, RM-02 architecture PR #3 and decision review PR #12. No independent branch should merge ahead of the contracts it references.
2. **Resolve AD-04–AD-07:** durable game-state authority, development/production session and reconnect model, separate disposable dev-map authority, and code/art/third-party redistribution rights. AD-03 design acceptance does not resolve those gates.
3. **Establish actual build/runtime evidence:** verified UE 5.8 source build and hydrated Git LFS checkout, then disposable map creation and two-client runtime/ownership tests. The current draft code and offline verification do not establish this evidence.
4. **Close RM-01 canonical-world authority independently:** identify and freeze the intended production world only with Unreal-aware Landscape and World Partition proof.
5. **Disposition [Issue #1](https://github.com/node63labs/morabezza-game/issues/1) in a separate, traceable change:** link its historical scope to AD-02, decide whether to archive/relabel/supersede the issue, and preserve original acceptance criteria. This document intentionally leaves it open.

## 5. Approval and scope controls

This document proposes only **repository-documentation alignment**. It does not merge PRs #2/#3/#12 or governance PR #15, grant new founder capacity, authorize a new code or gameplay feature, select a canonical map, alter `MORABEZA.uproject`, change the legacy C++ module spelling, or assert ownership of Sara de Melo Cruz's assets. All such actions remain under their own reviews.

**Proposed documentation exit:** the README states the intended product versus verified source correctly; the historical design is explicitly labeled without deleting it; milestone and gate distinctions are unambiguous; links resolve after the stacked architecture branches are reviewed and merged in order. That is a documentation review result, **not** M0, MMO-S0.1 or release acceptance.
