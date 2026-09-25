# RM-02 — Architecture and Portfolio Decision Register

**Date:** 2026-09-25
**Status:** PENDING FOUNDER / OWNER DECISIONS. No decision below is presumed approved.
**Companion:** RM_02_ARCHITECTURE_REVIEW_2026_09_25.md, separate draft review PR stacked on original architecture PR #3.
**Owner of decision record:** NODE63 Labs founder, with product-domain and security/architecture review as applicable.
**Scope control:** this register records questions, bounded alternatives, required evidence and explicit acceptance; it does not create product identity, modify a portfolio charter, authorize game implementation, merge a PR or approve a production release.

## PC-01 — What product is this game?

**Why blocking:** Approved NODE63 Portfolio Charter v1.0 identifies Morabeza as a **hospitality-oriented incubation product**. The MORABEZZA game is not named in that approved charter. The supplied Master Roadmap v1.4 is a draft revision. Similar product names cannot independently establish shared domain authority, release governance, investment priority or a shared database.

**Owner must choose and record one governed relationship:**
- A. MORABEZZA is a new independent game product with its own product charter, repository mapping, accountable owner and separately approved roadmap placement.
- B. MORABEZZA is an explicitly scoped game subproject of existing Morabeza, requiring an approved change to the hospitality product charter and a separation of game/hospitality data and authority.
- C. Another product name and governance placement, explicitly recorded in the approved portfolio source.

**Required evidence:** changed and accepted portfolio document; exact product code/name, domain owner, roadmap position, permitted resource budget and relationship to the existing hospitality product. Existing technical Unreal module/package MORABEZA need not be renamed as part of this governance decision.

**Status:** DECISION REQUIRED / NOT APPROVED. Do not copy Morabeza's P5 classification to the game by inference.

## AD-01 — Player avatar and the named protagonist

**Question:** does every player control a customized avatar, an individual character with a unique name, or a shared narrative protagonist rendered independently for each account? Define the canonical relationship of the historical named Morabeza hero to individual account-owned characters.

**Required evidence:** chosen product experience, character ID uniqueness, visual-rights/cosmetic limits, account-to-character cardinality, how two simultaneous players remain distinguishable, and the retention or deferral of historical narrative assets.

**Status:** DECISION REQUIRED / NO CHARACTER SCHEMA AUTHORIZED.

## AD-02 — Approved game genre, scope and historical M0

**Question:** is the target a multiplayer expansion of the Cape Verdean life-sandbox, a new isometric MMORPG product direction, or a separately scoped experimental MMO mode? Which prior M0 features (vehicle/Volta mission, local saves, economy and third-person camera) are retained, deferred or explicitly superseded?

**Required evidence:** versioned product-definition delta, approved vertical-slice acceptance, updated public README only after acceptance and a mapping from historical M0 to proposed MMO-S0.1. Fix the README's UE 5.6 statement only with the verified UE 5.8 project/toolchain baseline.

**Status:** DECISION REQUIRED / NO SILENT PRODUCT-DESIGN REPLACEMENT.

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

1. Resolve PC-01 and the portfolio capacity/scope decision in the **approved** governance record. This cannot be replaced by approving a code PR.
2. Resolve AD-01/02 before definitive character/camera/product changes. Retain historical single-player docs until the signed-off migration delta exists.
3. Resolve AD-03/05 to scope **S0-W2 character/session identity and single-zone admission**. S0-W1a/b/c already contain draft dev-contact server RPC and HUD changes; no duplicate implementation or acceptance by renaming.
4. Resolve AD-04 before durable rewards, inventory or database work. Resolve AD-06 before map/Unreal runtime tests. Resolve AD-07 before redistributing unverified engine/artist assets or shipping.
5. Review the separate public configuration token field under the appropriate security/change-control procedure; do not quote the value in PRs, logs or prompts. Its validity has not been proven.
6. Maintain PRs #3–#11 as **draft** under their existing branch dependencies. Actual INT-02 Unreal compilation and INT-03/04 two-client tests remain missing; no GitHub Actions or paid build host is requested by this register.

## Approval record template — not yet signed

- Decision ID: [PC-01 / AD-01 … AD-07]
- Approved alternative and explicit exclusions: [owner to provide]
- Effective approved source document/version and commit: [owner to provide]
- Accountable product owner / independent reviewer: [owner to provide]
- Security/authority classification and ADR link where required: [reviewer to provide]
- Preconditions and deferred work: [owner to provide]
- Approval date / reviewer identity: [owner to provide]
- Resulting allowed implementation PR and acceptance criteria: [owner to provide]

**Until populated and explicitly accepted, every entry remains PENDING.**
