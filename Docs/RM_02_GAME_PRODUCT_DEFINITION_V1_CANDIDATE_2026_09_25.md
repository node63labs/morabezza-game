# MORABEZZA — Game Product Definition V1 (Founder Review Candidate)

**Document ID:** MORABEZZA-PD-01  
**Date:** 2026-09-25  
**Status:** CANDIDATE / AD-01 OPTION B FOUNDER DECISION RECORDED; AD-02 HISTORICAL-SCOPE DISPOSITION PENDING  
**Product boundary:** MORABEZZA, independent NODE63 game product; PC-01 founder decision recorded. Governance charter v1.1 PC-01 candidate is under draft review, not yet accepted into governance `main`.  
**Source authority:** historical `README.md` and `Docs/GAME_DESIGN.md` on `morabezza-game/main@af8f3d0e5f9291b1358232c7055190e4bd57b9f6`; original RM-02 candidate PR #3; architecture-review PR #12; latest later source candidate PR #11. These sources have different ancestry and approval states.  
**Change class:** product definition / documentation only. No historical document is overwritten, and no technical, budget, staffing or implementation authority is granted.

## 1. Product identity and intended experience

**MORABEZZA** is a Cape Verde-inspired original game with a founder-directed **shared-world, elevated/isometric MMO gameplay target**. It combines exploration and story adventure with player-to-player presence and interactive world systems rather than reducing the MMO goal solely because it is hard to engineer. The working genre term **MMORPG** describes the target experience, not a claim that a deployed massively multiplayer runtime or a particular player capacity already exists.

The founder's previously selected presentation direction is an elevated/isometric, readable viewpoint with smaller characters in a shared world, visible other players, resources and enemies, and an MMO-style hotbar/social UI. The original creative brief also draws on **adventure/exploration associated with Tomb Raider** and **colorful, approachable Albion-like stylization**. These are comparisons for experience/readability, **not** permission to reproduce third-party characters, art, stories, systems or copyrighted assets.

**Setting:** a fictionalized Cape Verde-inspired world, with the earlier Praia/Santiago brief as source material. Portuguese and Cape Verdean Creole, architecture, landscapes, transport and local social detail are important identity inputs. Geographic accuracy and cultural authenticity require deliberate review; neither of the two tracked map packages is currently accepted as the authoritative production world.

**Product/legal identity:** `MORABEZZA` is the game product, while `Morabeza` remains the separate hospitality product. The UE project/module/package spelling remains `MORABEZA` until an independent Unreal migration review. NODE63 LABS owns portfolio governance. MORABEZZA owns its game-domain policy and data. No identity, permission, economy, secret, release or roadmap status is inherited from hospitality or Lariba.

## 2. AD-01 — Player-specific characters; Morabezza remains the story heroine

**Founder decision recorded 2026-09-25: OPTION B.** Players create or select **their own distinct player characters** in the shared world; they do not each play as an instance of the named heroine. **Morabezza remains the central authored story heroine**, preserving the original character's narrative importance and previously approved visual reference. [AD-01 decision record](https://github.com/node63labs/morabezza-game/blob/review/rm-02-architecture-decision-docket-20260925/Docs/AD_01_PLAYER_CHARACTERS_AND_STORY_HEROINE_DECISION_2026_09_25.md).

The founder previously accepted the **female main-character visual concept named Morabezza**, with dark curly hair/navy headband, warm brown skin, cream T-shirt, blue shorts, sandals, blue pendant and bracelets. Character design and asset-handling approval remain separate from player-account authorization and asset licensing. Visual materials must preserve the accepted orientation of collar/necklace/accessories.

The product identity contract now distinguishes **player characters** from **Morabezza as an authored heroine**. The exact way players create/select and customize a character, available cosmetics, naming uniqueness, number of character slots per account, narrative/quest presentation and heroine's possible bounded playable/non-player role require separately scoped product and implementation decisions. **Option B does not silently reclassify Morabezza as an NPC or approve special playable chapters.** Production account authorization, character schemas and cross-player isolation require AD-04/AD-05 and Unreal runtime evidence.

## 3. Source-preserving transition from the historical action/life sandbox

`Docs/GAME_DESIGN.md` historically defines an open-world action/life sandbox: Praia vertical slice, the **Volta** introductory mission, movement, a prototype vehicle, local save/load, money/reputation, relationships and later vehicles, businesses and island expansion. The existing `README.md` still calls M0.2 the Praia prototype and references UE 5.6; the current `.uproject` instead requests UE 5.8. Those historical artifacts **remain evidence of the original product plan**. This document proposes their disposition; it does not replace them as an accepted roadmap.

| Historical feature | Product-definition candidate for MMO direction | Decision / implementation status |
| --- | --- | --- |
| Cape Verde / fictionalized Praia identity and cultural detail | Retain as the game's original setting and story foundation. | Direction retained; canonical map and detailed scope need RM-01/AD-06 review. |
| Volta arrival/introduction mission | Preserve the concept as a candidate onboarding/story quest adaptable to the approved MMO character model. | Exact quest script, instancing, rewards and character role **TBD**; no mission migration authorized. |
| Vehicle entry/driving and property/business progression | Retain as potential later game content instead of silently deleting it; not part of the first two-player verification slice. | Founder must choose later MMO integration, standalone legacy mode or explicit feature retirement under AD-02. |
| Local SaveGame money/reputation/location/mission | Keep historical prototype behavior isolated. Multiplayer inventory, currency, progression and character ownership must be product-server authoritative with a separately approved durable store. | AD-04 required; no client-save-to-MMORPG migration or accepted economy. |
| Original third-person follow camera | Historical prototype and optional future contextual view. The previously selected MMO view is elevated/isometric. | Camera implementation/transition and storytelling perspective need runtime validation; no camera package mutation here. |
| NPCs, interaction, combat, resources and quests | Target small, testable shared-world interactions with authoritative outcomes. | PR #11 contains **dev-contact UI interaction source candidate only**; rewards, resource loop and combat not accepted. |
| Broad open world / whole islands | Keep long-term fiction ambition. First engineering slice uses one disposable small zone and two clients. | Shard count, supported population, world authority and release milestones remain TBD. |

No `main` README, `Docs/GAME_DESIGN.md`, M0 milestone or issue is silently renamed or closed. A separately accepted AD-02 delta will specify which historical obligations are retained, deferred, replaced or moved to another mode.

## 4. Product loop and milestone separation

**Long-term candidate loop:** enter a living shared zone → explore and meet players/NPCs → accept a story activity, combat or resource interaction → the MORABEZZA server validates and commits the consequential outcome → the owning character sees authorized progression → the player continues or returns on a later session.

**MMO-S0.1 first playable target, not current feature status:** a disposable /Game/Dev zone, two simultaneous distinct character sessions, server-owned movement and one owner-isolated NPC/contact interaction, one limited enemy encounter and one resource/reward operation that survives reconnect, plus elevated/isometric presentation. These are independent acceptance criteria, not a statement that all systems are present. Initial source-only W0/W1a/W1b/W1c development-contact and HUD branches remain draft; the reward, combat, authenticated session and durable store are not implemented/accepted.

**Runtime gate separation:** INT-01 and INT-02D/E offline preflight evidence does not establish UE 5.8 compilation, map load, two client connections or multiplayer security. INT-02 Editor/Game/Server UHT+C++ and INT-03/04 actual two-client evidence require an authorized engine host and hydrated Git LFS checkout. RM-01 recovery/canonical world and third-party asset permissions remain separate.

**Explicit first-slice exclusions:** production player population promise, monetization model, guild/trading/PvP economy, cross-zone handoff, production Lariba identity, whole-island asset rewrite, canonical map substitution and release commitments.

## 5. Architectural product ownership

- MORABEZZA must own account-to-character authorization, game sessions, game-domain consequences, economy, inventory, quests and player data. The client may present intent/predicted feedback but cannot choose final reward truth.
- One authoritative dedicated Unreal zone server and an optional product-owned session/durable-data service are **candidate architecture**, subject to AD-03/04/05. No deployment or account store has been authorized by this product definition.
- Lariba Cloud may supply accepted versioned infrastructure primitives **later**; it does not own a game's characters, money, ban rules or authorization. There is no first-slice hard dependency on unaccepted shared-platform contracts.
- The game and hospitality product do not share mutable domain tables, cross-product secrets or release authority.

## 6. Founder decisions remaining to close this version

**AD-01 — DECIDED, OPTION B:** each player has their own character and Morabezza remains the central story heroine. Her exact interactive/playable-versus-non-player treatment, character-creation rules and technical identity contract remain separate design/implementation decisions. The product-definition candidate preserves the founder-approved direction; it does not authorize gameplay code or character schema.

**AD-02:** affirm the previously selected shared-world isometric MMORPG as the accepted game definition and explicitly disposition the historical M0 obligations. In particular choose whether **Volta and vehicle gameplay** become later MMO features, remain in a separate legacy/single-player mode, or are retired in a separately documented scope decision. The broader vision may retain adventure/story elements without claiming the original third-person M0 was already achieved.

**Independent gates:** AD-03–AD-07 still govern technical topology, persistence, identity, dev-zone/map authority and license. PC-01 charter integration into approved governance `main` remains a separate merge decision. No feature implementation, engine build, map, PR merge or public roadmap rewrite is approved merely by writing this candidate.

## 7. Review outcome

**Completed now:** a versioned, source-grounded product-definition candidate that recognizes the founder's MMO/isometric direction, keeps the accepted protagonist visual concept, preserves and disposition-tracks historical M0 material, and defines a bounded future first slice. **Not completed:** formal AD-02 legacy-feature disposition, heroine's precise gameplay role, character creation/identity schema, portfolio charter admission on `main`, Unreal runtime or multiplayer acceptance. **AD-01 Option B founder selection is recorded** without treating the entire product-definition candidate as approved.
