# RM-02 — Architecture and Portfolio Decision Register

**Date:** 2026-09-25
**Status:** PC-01, AD-01 AND AD-02 PRODUCT DECISIONS RECORDED (2026-09-25); AD-03–AD-07 PENDING; PC-01 APPROVED PORTFOLIO CHARTER INCORPORATION PENDING.
**Companion:** RM_02_ARCHITECTURE_REVIEW_2026_09_25.md, separate draft review PR stacked on original architecture PR #3.
**Owner of decision record:** NODE63 Labs founder, with product-domain and security/architecture review as applicable.
**Scope control:** PC-01 records the founder's explicit independent-game product classification. The approved portfolio charter remains unchanged until a separate controlled governance update; this register does not assign game priority, approve implementation, merge a PR or approve production.

## PC-01 — Independent MORABEZZA game product

**Founder decision — 2026-09-25:** The founder explicitly chose **alternative A: MORABEZZA is an independent NODE63 game product under NODE63 LABS**, not a game subproject of the existing **Morabeza hospitality product**. Product independence does not create a separate legal company.

**Identity and authority:** `MORABEZZA` is the game product; `Morabeza` remains the separate hospitality product. The existing game repository is `node63labs/morabezza-game`, while the Unreal project/module stays `MORABEZA` until a separately reviewed rename. MORABEZZA requires its own game-domain authority, character/session/data boundaries and release approval; it inherits no hospitality database, secrets, permissions or release acceptance.

**Recorded governance decision:** [NODE63 PC-01 founder decision](https://github.com/node63labs/node63-governance/blob/governance/pc-01-morabezza-independent-game-20260925/portfolio/decisions/PC_01_MORABEZZA_INDEPENDENT_GAME_2026_09_25.md). A controlled [draft governance PR #15](https://github.com/node63labs/node63-governance/pull/15) now proposes its v1.1 charter incorporation. **Governance `main` and its approved v1.0 charter remain unchanged** until separately approved/merged; the first PR CI attempt failed and does not constitute acceptance. No Morabeza hospitality P5 priority transfers to the game, and no staffing/budget is assigned.

**Decision status:** **FOUNDER APPROVED / RECORDED — APPROVED PORTFOLIO BASELINE UPDATE PENDING.** PC-01 is no longer a founder-choice question. AD-01 Option B and AD-02 product scope are now founder-directed/recorded; AD-03–AD-07 architecture decisions and Unreal build/runtime gates remain separately pending. PC-01 does not authorize game implementation, production login, a paid build host, GitHub Actions or a merge to `main`.

## AD-01 — Player-specific characters and Morabezza story heroine

**Founder decision — 2026-09-25: OPTION B.** Each player controls their **own distinct player character** in the MORABEZZA shared world; players do **not** each control the named heroine. **Morabezza remains the central authored story heroine** and retains her established visual concept. This resolves the AD-01 avatar-versus-heroine product-design choice.

**Decision authority and record:** [AD-01 founder decision — Option B](https://github.com/node63labs/morabezza-game/blob/review/rm-02-architecture-decision-docket-20260925/Docs/AD_01_PLAYER_CHARACTERS_AND_STORY_HEROINE_DECISION_2026_09_25.md), on this draft review branch. The [game product-definition candidate](https://github.com/node63labs/morabezza-game/blob/review/rm-02-architecture-decision-docket-20260925/Docs/RM_02_GAME_PRODUCT_DEFINITION_V1_CANDIDATE_2026_09_25.md) reflects the founder's choice without replacing the historical game-design document on `main`.

**Explicitly undecided:** the heroine's exact interactive/playable-versus-non-player role, account-to-character cardinality, avatar customization scope, character naming rules, identity schema, storage, quests and asset rights. AD-01 does not by itself authorize character/account implementation, production authentication, Unreal builds, game release or merge of this draft PR.

**Status:** FOUNDER DECISION APPROVED / RECORDED — OPTION B; technical contract and implementation acceptance pending AD-03–AD-07 and the corresponding engineering gates.

## AD-02 — MMO product definition and historical M0 disposition

**Product decision recorded — 2026-09-25:** MORABEZZA's intended product is a **Cape Verde-inspired shared-world elevated/isometric action-adventure MMORPG**. Each player uses a distinct character and Morabezza remains the central story heroine (AD-01 Option B). This is a product direction, **not** an assertion of an operating large-population MMO, accepted engine runtime or final topology.

**Volta — RETAIN / LATER MMO QUEST:** adapt the original Praia-arrival introduction into a **later shared-world player-specific story/onboarding quest**, not part of the initial two-client connectivity gate or MMO-S0.1 full vertical slice. Preserve the historic mission concept; its script, Morabezza's direct role and any persistent rewards require separate narrative, AD-04/05 and runtime review. The current development test contact is not a completed Volta quest.

**Vehicle gameplay — RETAIN / LATER MMO FEATURE:** preserve vehicle entry/driving and possible travel/property progression as later MMO content, with a separately accepted server-authoritative vehicle/ownership/replication contract. No first-slice vehicle requirement and no separately authorized legacy single-player release.

**Original M0.1/M0.2:** those third-person/life-sandbox milestone requirements remain historical source/evidence. The prospective MMORPG acceptance path is the bounded MMO slice and later approved features. Preserve the original `README.md`, `Docs/GAME_DESIGN.md`, historical issue and binary assets until an explicit, separately reviewed milestone/public-documentation migration; do not claim original M0 completion or delete its scope silently.

**Decision evidence:** [AD-02 formal product-definition and legacy-scope record](https://github.com/node63labs/morabezza-game/blob/review/rm-02-architecture-decision-docket-20260925/Docs/AD_02_MMO_PRODUCT_DEFINITION_AND_LEGACY_SCOPE_DECISION_2026_09_25.md); [MORABEZZA game product-definition candidate](https://github.com/node63labs/morabezza-game/blob/review/rm-02-architecture-decision-docket-20260925/Docs/RM_02_GAME_PRODUCT_DEFINITION_V1_CANDIDATE_2026_09_25.md). Both remain on the draft review branch, without approved governance `main` incorporation or implementation approval.

**Status:** AD-02 PRODUCT-DIRECTION / LEGACY-SCOPE DECISION RECORDED; downstream AD-03–AD-07, roadmap integration and gameplay acceptance PENDING.

## AD-03 — Dedicated server topology and first-zone boundary

**Question:** approve a one-zone, two-distinct-player disposable development slice without yet deciding single- versus multi-shard production. Is the future directory/session-admission authority MORABEZZA-owned, and what identity/simulation boundary is controlled by the Unreal zone server?

**Required evidence:** server authority, connection/admission state transitions, controller/pawn ownership, logout cleanup, no cross-zone transition in S0, source-engine availability/cost approval and reproducible target build. World Partition is local world streaming, not automatic multi-server handoff.

**Status:** DECISION REQUIRED / NO HOST, ENGINE BUILD OR CAPACITY CLAIM.

## AD-04 — Durable game authority and persistence

**Question:** define MORABEZZA ownership of account, character, inventory, reward, progression and economy truth. PostgreSQL is a candidate technology, not an approved or provisioned database.

**Required evidence:** authoritative writer, transaction/idempotency keys, character-session revision fencing, owner-only replicated state, conflict/failure recovery, duplicate grant/concurrent spend negative tests and restrictions on legacy client SaveGame/PlayerState mutators.

**Status:** DECISION REQUIRED / NO ECONOMY DB OR MONEY MIGRATION AUTHORIZED.

## AD-05 — Authentication, product authorization and reconnect

**Question:** select the short-lived development session authority and the path to future production identity without requiring unaccepted Lariba contracts. Define the game-controlled authorization independent of any upstream authentication service.

**Required evidence:** trusted issuer and verifier, ticket signature/key rotation and storage, intended audience, AccountId, CharacterId, ZoneId, expiry, nonce/replay strategy, session revision, revocation/fencing, disconnect/reconnect rules, zone mismatch rejection and error/degraded modes. Accept only synthetic or explicitly approved development identity before production contracts exist.

**Additional cross-product gate:** optional future Lariba consumer must specify approved versioned identity/secret/event contract, product/environment isolation, failure mode, data owner and explicit NODE63 approval. No Lariba internal-database access or simulation-tick dependency.

**Status:** DECISION REQUIRED / NO PRODUCTION AUTH SESSION AUTHORIZED.

## AD-06 — Disposable test world versus RM-01 canonical-world decision

**Question:** approve a separate, verifiable /Game/Dev test-zone asset without replacing or changing either unresolved candidate world. Specify who creates/owns the asset and how a clean Git+LFS checkout recovers it.

**Required evidence:** actual UE 5.8 test-map creation and provenance, default-map isolation, absence of unintended canonical map mutations, recovery/evidence procedure and a future two-client load test.

**Status:** DECISION REQUIRED / NO TEST MAP PRESENT OR VERIFIED.

## AD-07 — License, artist deliverables and public repository boundary

**Question:** determine distribution/license for game code, owned character/environment art and third-party plugins/assets. Repository visibility is currently public; a public URL is not permission to redistribute third-party content.

**Required evidence:** code license or explicit no-distribution stance, artist rights/asset assignments, provenance and redistribution rights, MoverExamples and Mover source/plugin compatibility, and review of any value stored in publicly tracked configuration without reproducing secrets in the review.

**Status:** DECISION REQUIRED / NO SHIPPING OR THIRD-PARTY RIGHTS CLAIM.

## Implementation sequence and review handoff

1. Incorporate the founder-recorded PC-01 independent-game decision into the **approved** governance baseline through its separate controlled review; game priority/capacity remain undecided. This cannot be replaced by approving a code PR.
2. Apply AD-01 Option B and the recorded AD-02 MMO/Volta/vehicle disposition to a separately reviewed product-documentation and milestone migration. Retain historical single-player docs until the migration is reviewed; do not treat the new camera/character/gameplay code as accepted.
3. Resolve AD-03/05 to scope **S0-W2 character/session identity and single-zone admission**. S0-W1a/b/c already contain draft dev-contact server RPC and HUD changes; no duplicate implementation or acceptance by renaming.
4. Resolve AD-04 before durable rewards, inventory or database work. Resolve AD-06 before map/Unreal runtime tests. Resolve AD-07 before redistributing unverified engine/artist assets or shipping.
5. Review the separate public configuration token field under the appropriate security/change-control procedure; do not quote the value in PRs, logs or prompts. Its validity has not been proven.
6. Maintain PRs #3–#11 as **draft** under their existing branch dependencies. Actual INT-02 Unreal compilation and INT-03/04 two-client tests remain missing; no GitHub Actions or paid build host is requested by this register.

## Approval record template — not yet signed

- Decision ID: [PC-01 / AD-01 … AD-07; PC-01, AD-01 and AD-02 product choices recorded]
- Approved alternative and explicit exclusions: [owner to provide]
- Effective approved source document/version and commit: [owner to provide]
- Accountable product owner / independent reviewer: [owner to provide]
- Security/authority classification and ADR link where required: [reviewer to provide]
- Preconditions and deferred work: [owner to provide]
- Approval date / reviewer identity: [owner to provide]
- Resulting allowed implementation PR and acceptance criteria: [owner to provide]

**PC-01, AD-01 and AD-02 product decisions are recorded. PC-01 approved portfolio-baseline incorporation, AD-03–AD-07 architecture decisions, historical public-documentation migration and all implementation-specific acceptance remain PENDING.**
