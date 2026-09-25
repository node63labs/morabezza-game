# RM-02 — Architecture and Portfolio Decision Register

**Date:** 2026-09-25
**Status:** PC-01 FOUNDER DECISION RECORDED (2026-09-25); AD-01–AD-07 PENDING; APPROVED PORTFOLIO CHARTER INCORPORATION PENDING.
**Companion:** RM_02_ARCHITECTURE_REVIEW_2026_09_25.md, separate draft review PR stacked on original architecture PR #3.
**Owner of decision record:** NODE63 Labs founder, with product-domain and security/architecture review as applicable.
**Scope control:** PC-01 records the founder's explicit independent-game product classification. The approved portfolio charter remains unchanged until a separate controlled governance update; this register does not assign game priority, approve implementation, merge a PR or approve production.

## PC-01 — Independent MORABEZZA game product

**Founder decision — 2026-09-25:** The founder explicitly chose **alternative A: MORABEZZA is an independent NODE63 game product under NODE63 LABS**, not a game subproject of the existing **Morabeza hospitality product**. Product independence does not create a separate legal company.

**Identity and authority:** `MORABEZZA` is the game product; `Morabeza` remains the separate hospitality product. The existing game repository is `node63labs/morabezza-game`, while the Unreal project/module stays `MORABEZA` until a separately reviewed rename. MORABEZZA requires its own game-domain authority, character/session/data boundaries and release approval; it inherits no hospitality database, secrets, permissions or release acceptance.

**Recorded governance decision:** [NODE63 PC-01 independent-game founder decision](https://github.com/node63labs/node63-governance/blob/governance/pc-01-morabezza-independent-game-20260925/portfolio/decisions/PC_01_MORABEZZA_INDEPENDENT_GAME_2026_09_25.md), recorded on a separate governance branch from approved charter v1.0. It has **not** amended governance `main` or the approved charter. Its incorporation into an approved portfolio/charter revision is still required; that update must separately record accountable product owner, roadmap priority, resourcing, lifecycle and repository mapping. No existing Morabeza P5 priority transfers automatically.

**Decision status:** **FOUNDER APPROVED / RECORDED — APPROVED PORTFOLIO BASELINE UPDATE PENDING.** PC-01 is no longer a founder-choice question. The architectural AD-01–AD-07 decisions and Unreal build/runtime gates remain separately pending. PC-01 does not authorize game implementation, production login, a paid build host, GitHub Actions or a merge to `main`.

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
