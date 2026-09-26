# MORABEZZA

**MORABEZZA** is an original Cabo Verde-inspired game in early development at NODE63 LABS. Its founder-recorded product direction is an **elevated/isometric, shared-world action-adventure MMORPG**. That describes the game we intend to design; it does **not** describe a currently verified multiplayer build, player capacity, production service or release commitment.

> **Documentation status (2026-09-26):** This README is a proposed public-documentation migration, reviewed separately from the stacked RM-02 architecture decisions. The decisions cited below are recorded on draft game PRs [#3](https://github.com/node63labs/morabezza-game/pull/3) and [#12](https://github.com/node63labs/morabezza-game/pull/12); they are not yet merged into `main`. This README does not authorize implementation, hosting, spending or a change to NODE63 portfolio priority.

## Product identity and creative direction

- **Product:** MORABEZZA, a NODE63 game product distinct from the separately governed **Morabeza hospitality** product. The founder-recorded PC-01 classification still requires incorporation into the approved NODE63 governance charter ([governance PR #15](https://github.com/node63labs/node63-governance/pull/15)).
- **Setting:** a fictionalized world inspired by Cabo Verde's landscapes, communities, architecture, languages and stories. Cultural and geographic details require deliberate review.
- **Player identity:** each player has their own distinct character. **Morabezza** is the central authored story heroine, not an automatically shared player avatar. Her exact playable or non-player treatment is not yet decided.
- **Presentation target:** readable elevated/isometric exploration and action in a shared world. Existing third-person prototype code is preserved pending an independently reviewed migration.
- **Legacy scope:** the original **Volta** introduction and vehicle gameplay are retained as *later* MMO concepts. They are not requirements of the first two-client proof or the proposed MMO-S0.1 slice.

See the [game product-definition candidate](Docs/RM_02_GAME_PRODUCT_DEFINITION_V1_CANDIDATE_2026_09_25.md), [AD-01 player-character decision](Docs/AD_01_PLAYER_CHARACTERS_AND_STORY_HEROINE_DECISION_2026_09_25.md) and [AD-02 MMO/legacy-scope decision](Docs/AD_02_MMO_PRODUCT_DEFINITION_AND_LEGACY_SCOPE_DECISION_2026_09_25.md). These documents remain subject to their draft review and merge gates.

## What the integrated repository actually contains

The `main` baseline at `af8f3d0e5f9291b1358232c7055190e4bd57b9f6` contains an **Unreal Engine 5.8 C++ prototype**, not an accepted MMORPG runtime. Source-level foundations include:

- third-person character, camera and movement code;
- interaction tracing, the `E`-key interaction path, a development contact actor and dialogue/UI code;
- prototype player-state and local save-game classes, **not** an authoritative MMO economy or account store;
- a Landscape diagnostic commandlet for investigating world and actor ownership.

The tracked `MORABEZA.uproject` declares `EngineAssociation: "5.8"`. The Unreal project/module and `Source/MORABEZA/` paths retain the legacy **MORABEZA** spelling; changing those paths requires a separate engine/source migration review. Do not rename them as part of a branding update.

Source presence, a past local test or offline checks on unmerged branches do **not** prove a current clean Unreal build, dedicated-server operation, authenticated player sessions or two-client gameplay. The asset-recovery evidence is recorded separately in draft [RM-01 PR #2](https://github.com/node63labs/morabezza-game/pull/2).

## World and multiplayer boundaries

The existing `Untitled.umap` and `dskjnfkljsdnf.umap` remain **unresolved canonical-world candidates**. Landscape/World Partition integrity and a production-world freeze require separate RM-01 evidence. Do not treat either candidate as an approved production world or overwrite it for MMO testing.

The founder-approved **AD-03 design, not runtime acceptance**, scopes the first multiplayer proof to **one UE 5.8 dedicated server, one disposable development zone and two separate clients**. `/Game/Dev/MMO_S0_TestZone` is a planned path, **not a verified map asset**. Session/admission authority, durable persistence, dev-map creation and asset rights remain separate decisions. See [AD-03](Docs/AD_03_DEDICATED_SERVER_AND_FIRST_DEV_ZONE_DECISION_2026_09_25.md).

The proposed **MMO-S0.1 vertical slice** is a later acceptance target; it is not satisfied by a launch harness, source-only checks or the historical single-player M0.1/M0.2 definition. [Milestone transition and outstanding gates](Docs/MORABEZZA_MILESTONE_TRANSITION_2026_09_26_CANDIDATE.md).

## Historical design and repository organization

[Docs/GAME_DESIGN.md](Docs/GAME_DESIGN.md) preserves the original action/life-sandbox and Praia prototype brief **as historical design evidence**. Its original M0.1/M0.2 vehicle, mission, save/load and economy criteria are not current MMO-S0.1 acceptance criteria. [Issue #1](https://github.com/node63labs/morabezza-game/issues/1) still tracks the old UE 5.6 Praia scene and needs a separate, traceable disposition; this documentation PR does not close it.

- `Source/MORABEZA/` — C++ gameplay and diagnostics.
- `Content/` — Unreal maps/assets; binary content is tracked using Git LFS.
- `Config/` — Unreal project configuration.
- `Docs/` — historical design, candidate product decisions and engineering review evidence.

MORABEZZA's eventual game accounts, characters, quests, inventory, progression and rewards must be governed by its **own** accepted contracts. No automatic dependence on Lariba Cloud or the hospitality product is approved for the first development zone.

## License and art provenance

The repository is public, but **no redistribution license has been granted by that fact alone**. Code licensing, character-art rights, third-party Unreal plugins/assets and release permissions require a separate AD-07 review. The published concept art and Sara de Melo Cruz's character development should not be mistaken for integrated or redistributable game assets.
