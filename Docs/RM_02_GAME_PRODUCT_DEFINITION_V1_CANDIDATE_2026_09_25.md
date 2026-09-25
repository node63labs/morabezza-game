# MORABEZZA — Game Product Definition V1 (Founder Review Candidate)

**Document ID:** MORABEZZA-PD-01  
**Date:** 2026-09-25  
**Status:** PRODUCT-DEFINITION REVIEW CANDIDATE / AD-01 OPTION B AND AD-02 MMO + LEGACY SCOPE DECISIONS RECORDED; NOT MERGED TO `main`  
**Product boundary:** MORABEZZA, independent NODE63 game product; PC-01 founder decision recorded. Governance charter v1.1 PC-01 candidate is under draft review, not yet accepted into governance `main`.  
**Source authority:** historical `README.md` and `Docs/GAME_DESIGN.md` on `morabezza-game/main@af8f3d0e5f9291b1358232c7055190e4bd57b9f6`; original RM-02 candidate PR #3; architecture-review PR #12; latest later source candidate PR #11. These sources have different ancestry and approval states.  
**Change class:** product definition / documentation only. No historical document is overwritten, and no technical, budget, staffing or implementation authority is granted.

## 1. Product identity and intended experience

**MORABEZZA** is a Cape Verde-inspired original game with a founder-recorded AD-02 **shared-world, elevated/isometric action-adventure MMORPG product direction**. It combines exploration and story adventure with player-to-player presence and interactive world systems rather than reducing the MMO goal solely because it is hard to engineer. The working genre term **MMORPG** describes the target experience, not a claim that a deployed massively multiplayer runtime or a particular player capacity already exists.

The founder's previously selected presentation direction is an elevated/isometric, readable viewpoint with smaller characters in a shared world, visible other players, resources and enemies, and an MMO-style hotbar/social UI. The original creative brief also draws on **adventure/exploration associated with Tomb Raider** and **colorful, approachable Albion-like stylization**. These are comparisons for experience/readability, **not** permission to reproduce third-party characters, art, stories, systems or copyrighted assets.

**Setting:** a fictionalized Cape Verde-inspired world, with the earlier Praia/Santiago brief as source material. Portuguese and Cape Verdean Creole, architecture, landscapes, transport and local social detail are important identity inputs. Geographic accuracy and cultural authenticity require deliberate review; neither of the two tracked map packages is currently accepted as the authoritative production world.

**Product/legal identity:** `MORABEZZA` is the game product, while `Morabeza` remains the separate hospitality product. The UE project/module/package spelling remains `MORABEZA` until an independent Unreal migration review. NODE63 LABS owns portfolio governance. MORABEZZA owns its game-domain policy and data. No identity, permission, economy, secret, release or roadmap status is inherited from hospitality or Lariba.

## 2. AD-01 — Player-specific characters; Morabezza remains the story heroine

**Founder decision recorded 2026-09-25: OPTION B.** Players create or select **their own distinct player characters** in the shared world; they do not each play as an instance of the named heroine. **Morabezza remains the central authored story heroine**, preserving the original character's narrative importance and previously approved visual reference. [AD-01 decision record](https://github.com/node63labs/morabezza-game/blob/review/rm-02-architecture-decision-docket-20260925/Docs/AD_01_PLAYER_CHARACTERS_AND_STORY_HEROINE_DECISION_2026_09_25.md).

The founder previously accepted the **female main-character visual concept named Morabezza**, with dark curly hair/navy headband, warm brown skin, cream T-shirt, blue shorts, sandals, blue pendant and bracelets. Character design and asset-handling approval remain separate from player-account authorization and asset licensing. Visual materials must preserve the accepted orientation of collar/necklace/accessories.

The product identity contract now distinguishes **player characters** from **Morabezza as an authored heroine**. The exact way players create/select and customize a character, available cosmetics, naming uniqueness, number of character slots per account, narrative/quest presentation and heroine's possible bounded playable/non-player role require separately scoped product and implementation decisions. **Option B does not silently reclassify Morabezza as an NPC or approve special playable chapters.** Production account authorization, character schemas and cross-player isolation require AD-04/AD-05 and Unreal runtime evidence.

## 3. AD-02 — Source-preserving MMO transition from the historical action/life sandbox

**AD-02 decision recorded 2026-09-25:** the product direction is Cape Verde-inspired shared-world elevated/isometric action-adventure MMORPG. **Volta and vehicles are retained as later MMO features, outside the initial two-player proof and full MMO-S0.1 vertical-slice criteria**; no separate legacy single-player release is authorized. See the [formal AD-02 decision record](https://github.com/node63labs/morabezza-game/blob/review/rm-02-architecture-decision-docket-20260925/Docs/AD_02_MMO_PRODUCT_DEFINITION_AND_LEGACY_SCOPE_DECISION_2026_09_25.md).

The historic `Docs/GAME_DESIGN.md` defines an open-world action/life sandbox: fictionalized Praia, **Volta** arrival/introduction, movement, prototype driving, local save/load and money/reputation. The `README.md` still names M0.2 Praia prototype and UE 5.6; current `.uproject` requests UE 5.8. These historical documents remain **evidence of original scope**, not proof it was completed. Prospective MMO acceptance supersedes the historical single-player M0.1/M0.2 targets **only after this controlled decision is incorporated**. Neither historical file nor GitHub Issue #1 is edited or closed in this draft.

| Historical feature | AD-02 product disposition | Technical/acceptance boundary |
| --- | --- | --- |
| Cape Verde / fictionalized Praia identity and culture | **Retain** as setting and narrative direction. | RM-01 world authority and AD-06 dev-map approval are independent. |
| `Volta` arrival/introduction mission | **Retain as a later player-specific MMO onboarding/story quest.** Adapt to each player's own character; Morabezza remains the central story heroine. | **Excluded from initial two-client and MMO-S0.1 gates.** Exact script, heroine role, quest progression and any reward require later narrative and AD-04/05 review. Dev-contact dialogue is not completed Volta. |
| Vehicle entry/driving, possible ownership and transport | **Retain as later MMO gameplay**; no separate legacy mode. | **Excluded from first slice.** Driver/seat ownership, server movement, replication and disconnect behavior need a separately approved feature spec. |
| Local `SaveGame` money/reputation/location/mission | **Preserve as prototype-only historical data**, not network player truth. | MORABEZZA-owned durable economy and safe migration require AD-04/05. |
| Original third-person camera | Preserve prototype source; elevated/isometric is the intended shared-world presentation. | Camera implementation and possible contextual story views need verified Unreal evidence. |
| NPC/contact, enemy, resource and progression | **Retain** as bounded shared-world systems. | Draft PR #11 has development-only cosmetic interaction; no accepted reward/combat yet. |
| Whole-island/open-world expansion | Long-term setting ambition, not first deployment capacity. | Start with disposable zone and two distinct clients; shards/population uncommitted. |
| Historical M0.1/M0.2 and separate single-player release | Keep legacy milestone definitions as traceable history; MMO acceptance becomes prospective target. | No original-M0 completion claim; no separate legacy shipping stream; README/issue migration needs review. |

**Milestone separation:** two-client connectivity and owner-isolated interaction precede the full **MMO-S0.1** target (one disposable zone, two players, one NPC/contact, one resource, one enemy, one durable owner-only reward and elevated presentation). **Volta and drivable vehicles are post-S0.1 features.** Neither milestone is accepted merely because a design file or source-only regression exists.

## 4. Product loop and milestone separation

**Long-term candidate loop:** enter a living shared zone → explore and meet players/NPCs → accept a story activity, combat or resource interaction → the MORABEZZA server validates and commits the consequential outcome → the owning character sees authorized progression → the player continues or returns on a later session.

**MMO-S0.1 first playable target, not current feature status:** a disposable /Game/Dev zone, two simultaneous distinct character sessions, server-owned movement and one owner-isolated NPC/contact interaction, one limited enemy encounter and one resource/reward operation that survives reconnect, plus elevated/isometric presentation. These are independent acceptance criteria, not a statement that all systems are present. Initial source-only W0/W1a/W1b/W1c development-contact and HUD branches remain draft; the reward, combat, authenticated session and durable store are not implemented/accepted.

**Runtime gate separation:** INT-01 and INT-02D/E offline preflight evidence does not establish UE 5.8 compilation, map load, two client connections or multiplayer security. INT-02 Editor/Game/Server UHT+C++ and INT-03/04 actual two-client evidence require an authorized engine host and hydrated Git LFS checkout. RM-01 recovery/canonical world and third-party asset permissions remain separate.

**Explicit first-slice exclusions:** the full Volta mission and persistent quest/reward logic; drivable vehicles and ownership; production player population promise, monetization, guild/trading/PvP economy, cross-zone handoff, production Lariba identity, whole-island asset rewrite, canonical map substitution and release commitments.

## 5. Architectural product ownership

- MORABEZZA must own account-to-character authorization, game sessions, game-domain consequences, economy, inventory, quests and player data. The client may present intent/predicted feedback but cannot choose final reward truth.
- One authoritative dedicated Unreal zone server and an optional product-owned session/durable-data service are **candidate architecture**, subject to AD-03/04/05. No deployment or account store has been authorized by this product definition.
- Lariba Cloud may supply accepted versioned infrastructure primitives **later**; it does not own a game's characters, money, ban rules or authorization. There is no first-slice hard dependency on unaccepted shared-platform contracts.
- The game and hospitality product do not share mutable domain tables, cross-product secrets or release authority.

## 6. Recorded product decisions and remaining gates

**PC-01 — DECIDED:** MORABEZZA is independent from Morabeza hospitality; charter incorporation remains draft in [governance PR #15](https://github.com/node63labs/node63-governance/pull/15). No inherited hospitality priority or extra engineering capacity.

**AD-01 — DECIDED, OPTION B:** each player has their own character, while Morabezza remains central to the authored story. Heroine gameplay treatment and technical account-to-character contract remain separate.

**AD-02 — PRODUCT DIRECTION AND HISTORICAL SCOPE RECORDED:** Cape Verde-inspired elevated/isometric shared-world MMORPG. Volta becomes a **later MMO onboarding/story quest**, while vehicle entry/driving becomes **later MMO travel/gameplay**. Both are outside the first two-client and full MMO-S0.1 acceptance. Original single-player M0 documents remain historical evidence; no separately authorized legacy shipping stream. [AD-02 decision](https://github.com/node63labs/morabezza-game/blob/review/rm-02-architecture-decision-docket-20260925/Docs/AD_02_MMO_PRODUCT_DEFINITION_AND_LEGACY_SCOPE_DECISION_2026_09_25.md).

**Still open:** AD-03–AD-07 technical decisions, heroine's exact gameplay role, character customization, Volta script/rewards, vehicle feature design, roadmap/README/Issue #1 migration, UE 5.8 Editor/Game/Server compilation, two-player runtime, world authority and asset rights. No game implementation, engine build, map mutation, release or merge is approved by this product decision.

## 7. Review outcome

**Completed at the product-decision level:** PC-01 independent-game classification, AD-01 Option B player characters/story heroine and AD-02 elevated/isometric MMORPG direction with Volta and vehicles retained **after** the MMO-S0.1 slice. This candidate now reflects those decisions and preserves the original M0 design as traceable history.

**Not completed:** approval/integration of governance charter into `main`, review/merge of this product-definition document, historical README/roadmap/issue migration, AD-03–AD-07, heroine gameplay/quest details, actual Unreal compilation or two-client runtime. This draft does not establish accepted deployed MMO functionality or additional capacity.
