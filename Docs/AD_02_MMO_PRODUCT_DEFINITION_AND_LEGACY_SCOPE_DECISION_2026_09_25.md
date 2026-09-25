# AD-02 — MORABEZZA MMO Product Definition and Legacy Scope

**Decision ID:** MORABEZZA-AD-02  
**Decision date:** 2026-09-25  
**Decision authority:** NODE63 LABS founder, MORABEZZA product  
**Status:** PRODUCT-DIRECTION / LEGACY-SCOPE DECISION RECORDED; SOURCE BRANCH CANDIDATE — NOT MERGED TO GAME `main`  
**Decision basis:** The founder directed formal AD-02 establishment after choosing AD-01 Option B. The previously proposed treatment was to retain **Volta and vehicle gameplay as later MMO features, outside the first two-player slice**. This record adopts that scoped treatment; it does not invent a complete mission script, vehicle design, runtime acceptance or shipping date.  
**Authority boundaries:** PC-01 founder-classifies MORABEZZA as independent from the Morabeza hospitality product; governance charter incorporation is still pending in draft PR #15. AD-01 Option B establishes distinct player characters and Morabezza as the central story heroine. AD-03–AD-07 remain open.

## 1. Binding product direction — what game are we designing?

**MORABEZZA is a Cape Verde-inspired, shared-world elevated/isometric action-adventure MMORPG design target.** Players control their **own distinct characters** (AD-01 Option B), encounter other players and a living world, explore fictionalized Cape Verde-inspired locations, interact with story characters, and later engage with progression, activities, combat and resources under MORABEZZA game-domain authority.

An elevated/isometric, readable camera and shared-world presentation are the **intended product experience**, not a statement that the current third-person C++ prototype or two-player network runtime already implements them. The **MMORPG** term describes the product direction, not a measured concurrency claim, committed shard topology, service-level guarantee or promised release.

The named heroine **Morabezza** remains the central authored story heroine. AD-02 does not convert her into every player's avatar or determine whether she is exclusively non-player versus playable in separate bounded story chapters.

**First engineering principle:** prove a small, observable, owner-isolated multiplayer slice before whole-island scope, large-population promises or an expensive server topology. The game remains its own product; the hospitality Morabeza product and Lariba Cloud do not own game character, quest, money or inventory truth.

## 2. Volta — RETAIN as a later MMO story/onboarding mission

The historical `Docs/GAME_DESIGN.md` defines **Volta** as an introductory mission in which a player arrives in Praia, meets an acquaintance, and learns movement, interaction, navigation and the game's basic economy. That source concept is **retained**, not discarded.

**AD-02 disposition:** adapt **Volta** into a **later MORABEZZA MMO onboarding/story quest** for each player's own character. It should convey the Cape Verde-inspired setting and basic world interaction in the shared-world narrative, with Morabezza the heroine remaining important to the wider story. The exact role of Morabezza in Volta, the acquaintance's identity, dialogue, quest stages, level layout, party handling and mission scripting remain a separate narrative/design review. Do not silently turn the heroine into the acquaintance or a permanent NPC.

Volta is **not a prerequisite of the first two-client connectivity / interaction gate or MMO-S0.1 verification slice**. Its future authoritative quest progress, reward eligibility and money changes require AD-04/AD-05 game-owned persistence, player-character ownership, idempotency and negative multiplayer tests. The current `VOLTA_INTRO` and development-only contact source must not be represented as an accepted persistent MMO quest or as completed Volta.

**Acceptance for the eventual Volta implementation:** a reviewed mission spec; a verified player-specific quest/trigger contract; owner-only progress and dialogue where appropriate; authorized and idempotent reward if one is designed; reconnect consistency; a playable Unreal zone; and actual runtime evidence. No script, economy reward or release date is authorized by this decision.

## 3. Vehicles — RETAIN as later MMO travel/gameplay

The historical M0.1 design expected enterable and drivable prototype vehicles and later driving/property progression. **Vehicles remain part of the long-term MORABEZZA game direction**, but **not part of the first two-player slice or the initial MMO-S0.1 technical acceptance**. There is no separate single-player/legacy game release authorized by AD-02.

Future vehicle integration must be defined for an MMO context: driver/passenger access, actor/seat ownership, server-authoritative possession and movement, replication/relevancy, safe dismount, collision and disconnect/reconnect semantics, and any inventory/economy consequences. Island travel or cross-zone transfer must **not** be inferred from having a drivable vehicle. Vehicle physics, art, exact fleet, PvP/combat use, commercial systems and scheduling are deferred until a bounded feature design is accepted.

The existing prototype vehicle idea and related source/design work are preserved as historical engineering/creative material; their presence does not establish an accepted network vehicle system.

## 4. Historical M0.1/M0.2 versus the MMO milestones

The existing `README.md` and `Docs/GAME_DESIGN.md` were written for an original third-person, action/life-sandbox plan. They remain **historical source material**. The fact that earlier M0.1 required vehicle driving, Volta, local save/load and money does **not** mean those requirements have already passed, nor that they must all block the newly scoped first networked engineering gate.

**AD-02 supersedes the historical single-player M0.1/M0.2 definitions as prospective acceptance criteria for the MORABEZZA MMORPG direction**, subject to controlled incorporation of this design decision. Do **not** rewrite or delete those historical files on `main` in this review; disposition of GitHub Issue #1 and the public README must be handled in a separate traceable product-definition/roadmap change.

| Historical element | AD-02 disposition | First MMO slice? |
| --- | --- | --- |
| Cape Verde-inspired Praia/Santiago identity and local culture | Retain as setting/narrative direction; world-map authority remains RM-01/AD-06. | Disposable representative test zone only; not the canonical Praia map. |
| Named heroine Morabezza | Retain as central story heroine; players use their own characters (AD-01). | Distinct dev character connections; heroine content not required. |
| Volta introductory mission | Retain and adapt as a later MMO story/onboarding quest; authoritative quest/reward design later. | **No.** Dev-contact dialogue is not accepted Volta. |
| Vehicle entry, driving and later ownership | Retain for future MMO travel/gameplay with server/owner authority; details deferred. | **No.** |
| Third-person follow-camera M0 acceptance | Historical prototype behavior; elevated/isometric viewpoint is the intended MMO presentation. | Bounded elevated camera proof at the appropriate UI milestone; no unreviewed Unreal migration. |
| Local SaveGame money, reputation, location, mission | Historical standalone/prototype data only; never authoritative MMO character/economy truth. | No client-save authority. Durable reward only after AD-04/05. |
| NPC/contact interaction, resource, enemy and progression | Retain as bounded shared-world MMO systems subject to server-side validation. | First full MMO-S0.1 target includes one of each, gated by runtime and persistence acceptance. |
| Standalone/single-player release | No separate production mode or maintenance stream established by AD-02. | **No.** Old prototype preserved for evidence. |

**Milestone distinction:** initial S0-W0/W1 offline source/harness checks and the first **two-client connectivity proof** are narrower than the proposed **MMO-S0.1 full vertical slice** in RM-02 (one disposable zone, two distinct characters, NPC interaction, one resource, one enemy, one durable owner-specific reward and an elevated camera). Nothing here marks that full slice complete. Volta and vehicles remain **later than MMO-S0.1** and must not be shoehorned into its acceptance criteria.

## 5. Product acceptance and exclusions

**Decision-level acceptance:** product target and old M0 feature disposition defined above. This is an AD-02 **product-definition decision** documented on a **draft review branch**, not approval to merge historical docs, release a game or implement server/domain authority.

**Excluded from AD-02 acceptance:** playable/AI role of Morabezza the heroine; exact avatar customization and account-to-character cardinality; detailed Volta story; vehicle game design; single/multi-shard production topology; player capacity; payment/monetization; economy schema; production authentication; canonical map selection; art license; Lariba integration; purchased hardware; CI runs; Unreal 5.8 UHT/C++ compilation; two-client network acceptance; or game `main` merge.

**Required follow-up:** record AD-02 in the RM-02 register and product-definition document; resolve AD-03–AD-07 independently; approve the game product/portfolio priority and any actual implementation scope separately. Draft PR #12 remains draft. Governance PR #15 remains draft with its previously observed CI failure; AD-02 does not clear that failure or incorporate PC-01 into governance `main`.
