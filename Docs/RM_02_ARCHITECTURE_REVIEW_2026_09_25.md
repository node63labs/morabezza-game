# RM-02 — MORABEZZA MMO Architecture Review (2026-09-25)

**Disposition: SOURCE REVIEW COMPLETE; ARCHITECTURE APPROVAL ON HOLD.**
**PC-01 subsequent disposition (2026-09-25): FOUNDER DECISION RECORDED — MORABEZZA is an independent NODE63 game product. The approved portfolio charter has NOT yet been amended; AD-01–AD-07 and the remaining architecture acceptance gates are still pending.**
**Scope:** Documentation and source review only. No game code, map, LFS object, repository setting or production service changed.
**Review baseline:** draft PR #3 at bf50e288f7be71b772b90826a25d75a96587d44c; its audited game main at af8f3d0e5f9291b1358232c7055190e4bd57b9f6.
**Later source inspected independently:** draft PR #11 at 155168e7b29e521f59cc64bfd0e2b5274c3790ed. PR #11 is NOT an ancestor of PR #3.
**Governing source inspected:** approved NODE63 Portfolio Charter v1.0, node63-governance main df746d72927f6da37fdebe6e2a34abf1a035e2f0, charter blob 9a06b1a32fa729a4db76ff19295068fca97edb35.

## 1. Governance finding: hospitality Morabeza is not automatically this game

The approved Portfolio Charter v1.0 defines **Morabeza as a hospitality-oriented product in incubation**. The reviewed repository is a Cape Verde-inspired game named MORABEZA in Unreal and MORABEZZA in the proposed player-facing identity. The approved charter does not identify the MORABEZZA game. PR #3 assumes the game's inherited Morabeza P5/incubation position, but that assignment is **not established by the approved charter**.

The project-provided NODE63 Master Roadmap v1.4 and Ecosystem/Product Boundaries v1.1 are explicitly **draft revisions requiring founder approval**. The roadmap discusses Morabeza P5, not a separately registered MORABEZZA game. Do not silently treat those drafts as an approved portfolio amendment or silently transfer hospitality product authority to game character data.

**PC-01 — founder decision recorded 2026-09-25:** MORABEZZA is a separate NODE63 game product, **not** a subproject of the Morabeza hospitality product. The [founder decision record](https://github.com/node63labs/node63-governance/blob/governance/pc-01-morabezza-independent-game-20260925/portfolio/decisions/PC_01_MORABEZZA_INDEPENDENT_GAME_2026_09_25.md) is on a separate governance branch. The existing approved charter remains unchanged. [Draft governance PR #15](https://github.com/node63labs/node63-governance/pull/15) stages a controlled v1.1 PC-01 amendment; its initial CI result is failure and governance approval/merge remain pending. No game roadmap priority, staffing or implementation authority has been assigned.

## 2. What the RM-02 candidate proposes, without accepting implementation

PR #3's architecture candidate proposes a Cape Verde-inspired elevated-isometric MMORPG; a dedicated authoritative Unreal zone server; game-owned account, character, inventory, rewards and persistence; versioned, optional Lariba platform adapters; and a small disposable-zone multiplayer slice. The original migration register distinguished a local single-player baseline from that proposed design. It explicitly left avatar identity, genre change, topology, persistence, authentication, map authority and third-party rights open.

The approach is suitable for technical discussion but remains a **candidate**. Its source audit of the original main commit is historical evidence and should not be rewritten to suggest later draft PRs have been accepted.

## 3. Current-source delta from that historical baseline

| Source finding in PR #3 | Later draft PR #11 source observation | Disposition |
| --- | --- | --- |
| SRC-01: GameMode first-controller test contact | GameMode RestartPlayer uses the supplied controller and pawn; maps one owner-specific dev contact, with respawn and logout cleanup. | Addresses the dev-contact code shape only; does NOT establish account admission, distinct authenticated characters or a runtime proof. |
| SRC-02: wrong-player contact dialogue fallback | ContactActor requires the interacting pawn controller, tests contact ownership and sends cosmetic owner-client acknowledgement. | Source-level mitigation only; owner-only replication and HUD isolation need actual two-client evidence. |
| SRC-03: client-direct interaction | Possessed character routes ServerTryInteract to server-side controller/pawn, dev-contact owner, distance, sweep, line-of-sight and cooldown checks. | Limited to TEST_INTERACTION, with no durable reward; not an accepted general authoritative action system. |
| SRC-06: one-shot input/HUD | Draft W1c adds PawnClientRestart and OnRep_Controller input reconciliation and HUD pawn/delegate rebinding. | No engine compilation, delayed-possession runtime or two-client verification. |
| SRC-07: no dedicated server target on audited main | Draft W0 adds MORABEZAServer.Target.cs; PRs #8–#11 add integration/source/build-host checks. | Server target source exists in drafts, but Unreal 5.8 UHT/C++ compilation has NOT occurred. |
| SRC-04/05: local Money, Reputation and SaveGame | PlayerState still exposes BlueprintCallable money/reputation mutators; SaveGame still stores local money, reputation, location and mission fields. | No approved server-side durable economy, atomic reward, session-fenced persistence or owner-only inventory. |
| SRC-08: unresolved canonical map | DefaultEngine.ini still points to the old map. The two candidate map packages remain; no disposable /Game/Dev map is tracked. | Do not change or declare either production candidate authoritative. The launch script validates a future path string, not a real asset. |
| SRC-09: historical product definition | README and GAME_DESIGN.md describe an action/life sandbox with driving, a Volta mission and local saves; README also says UE 5.6 while the project requests UE 5.8. | Explicit product-definition delta, milestone reconciliation and README correction only after owner review. |
| SRC-11: tracked public configuration | An Android File Server token field is present in tracked configuration. | Separate owner-led configuration/secret validity assessment; value deliberately omitted here. |

The original PR #3 docs and later PR #11 code have different Git ancestry. PRs #4–#11 are **draft source candidates** and none of their prior source-only passing tests establishes a working two-player MMO.

## 4. Architecture acceptance findings

| ID | Review item | Required resolution |
| --- | --- | --- |
| GOV-01 | Approved hospitality-product charter does not yet contain the game. | PC-01 founder choice resolved as **independent game product**; incorporate it into a controlled approved portfolio revision before asserting registered product status, roadmap priority or implementation authorization. |
| PROD-01 | Old single-player/life-sandbox definition versus proposed MMORPG and named protagonist. | Approve AD-01 and AD-02, preserving historical requirements and explicit deferrals. |
| NET-01 | One-zone two-player experiment versus unspecified single-/multi-shard production topology. | AD-03 must distinguish the first disposable test-zone topology from any later shard transfer; no invented population or performance targets. |
| ID-01 | Unreal controller ownership is not account-to-character authorization. No ticket issuer/verifier, revocation, session fencing or reconnect contract evidenced in inspected game source. | AD-05: product-owned account and character binding, audience-bound short-lived ticket, exact zone/session revision, nonce/replay rejection, revocation, reconnect and mismatched-owner negative tests. Production authentication remains a separate gate. |
| DATA-01 | PlayerState local mutators and client SaveGame are not shared-game economic truth. | AD-04: game-owned persistence and transaction boundary, atomic idempotent rewards, concurrent-spend rejection, reconnect consistency and private inventory views. PostgreSQL remains only a storage candidate. |
| WORLD-01 | No verified disposable dev-map asset; canonical-world SSD reconciliation remains open. | AD-06, recovery-safe map approval and independent RM-01 world authority. |
| RIGHTS-01 | Public repository, unfinalized game license, artist assets and experimental engine MoverExamples dependency. | AD-07 source/asset rights, distribution and supported plugin provenance; no licensed Epic engine source committed. |
| PLATFORM-01 | Conditional Lariba adapters must not confer game-domain authority or create hidden synchronous dependencies. | Explicit versioned consumer contract and separate cross-product approval. Do not use Lariba as first-slice required runtime. |
| VERIFY-01 | No available UE 5.8 build host or actual two-client gameplay. | INT-02 Editor/Game/Server compilation, then INT-03/04 session/owner/range/LOS/respawn and negative tests on a disposable map. |

**Threat review inputs, not claims of current production exploitation:** forged account or target, non-owning RPC, expired/replayed ticket, wrong-zone entry, stale reconnect, cross-player interaction, reward duplication, concurrent spend, missing persistence commit, untrusted client SaveGame and credential/configuration exposure. A new product identity or authorization boundary requires the appropriate NODE63 change classification, ADR and explicit trust/security review.

## 5. Plan reconciliation, not an implicit implementation authorization

The candidate implementation order predates W1a/W1b/W1c. The later draft branches already contain **development-contact** server RPC and local HUD lifecycle candidates. Do not relabel them as authenticated MMO interaction or duplicate that scope in S0-W2. Following incorporation of the recorded PC-01 decision into approved governance and approval of the affected AD decisions, propose S0-W2 as **character/session identity and single-zone admission**, reserving authoritative persistence and rewards for a separately approved S0-W3.

The first complete MMORPG slice in PR #3 additionally proposes one resource, one NPC, one enemy and one durable reward. The current draft source only establishes a test-contact dialogue candidate, not that vertical slice. Preserve both descriptions with their evidence scope.

A versioned product-definition delta must reconcile the proposed camera/avatar design with the historical driving, Volta mission, local saves, source-module MORABEZA naming and proposed MORABEZZA game identity. Do not rename Unreal packages or edit either map in this review.

### AD-01/AD-02 product definition follow-up — 2026-09-25

A [versioned MORABEZZA game product-definition candidate](https://github.com/node63labs/morabezza-game/blob/review/rm-02-architecture-decision-docket-20260925/Docs/RM_02_GAME_PRODUCT_DEFINITION_V1_CANDIDATE_2026_09_25.md) now records the founder's previously expressed MMO/isometric direction, the accepted named heroine visual reference and the historical life-sandbox/M0 feature disposition questions. **AD-01 MMO avatar model remains undecided; AD-02 genre direction is recorded, but formal scope and legacy-feature disposition are pending.** The historical README and game-design foundation remain intact. No technical implementation acceptance follows from the candidate.

## 6. Gate statement and source register

**Review performed:** read both PR #3 candidate documents and the selected code/config/README in draft PR #11; verified PR #3 and PR #11 remain drafts and game main was unchanged at review time; checked the current approved charter in node63-governance/main and distinguished project-supplied draft revision documents.

**Not performed:** founder/portfolio decision, canonical map selection, identity service, database creation, security remediation, engine build, LFS hydration on a new machine, game launch, network test, or GitHub Actions.

**Review result (updated 2026-09-25):** PC-01 founder classification is recorded as an independent game product; its incorporation into the approved portfolio charter and AD-01–AD-07 remain pending. Technical architecture approval, Unreal compilation and gameplay acceptance remain on hold. The original PR #3 documents, gameplay source and `main` remain unchanged.

- [Original architecture candidate in PR #3](https://github.com/node63labs/morabezza-game/blob/design/rm-02-mmo-architecture-candidate/Docs/RM_02_MMO_RUNTIME_ARCHITECTURE_V1_CANDIDATE.md)
- [Original source-migration register in PR #3](https://github.com/node63labs/morabezza-game/blob/design/rm-02-mmo-architecture-candidate/Docs/RM_02_SOURCE_MIGRATION_AND_REMOTE_WORK_PLAN.md)
- [Later draft integration/host work in PR #11](https://github.com/node63labs/morabezza-game/pull/11)
- [Approved NODE63 Portfolio Charter v1.0](https://github.com/node63labs/node63-governance/blob/df746d72927f6da37fdebe6e2a34abf1a035e2f0/portfolio/NODE63_PORTFOLIO_CHARTER.md)
