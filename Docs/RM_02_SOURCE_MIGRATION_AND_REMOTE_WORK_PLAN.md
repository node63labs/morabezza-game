# RM-02 — Remote Source Migration Register and Verification Plan

**Date:** 2026-09-24  
**Status:** CANDIDATE / SOURCE REVIEW COMPLETE; IMPLEMENTATION NOT AUTHORIZED  
**Audited source:** `node63labs/morabezza-game@af8f3d0e5f9291b1358232c7055190e4bd57b9f6`  
**Companion:** `Docs/RM_02_MMO_RUNTIME_ARCHITECTURE_V1_CANDIDATE.md`

This register distinguishes code actually found in GitHub from changes proposed for the MMORPG. Findings are not claims of exploitable production vulnerabilities; the repository has no accepted MMO production runtime. Do not silently edit or delete existing gameplay while RM-01/architecture gates remain open.

## 1. Current source-level migration findings

| ID | Source evidence | MMO implication | Proposed change | Verification gate |
| --- | --- | --- | --- | --- |
| SRC-01 | `MORABEZAGameMode.cpp::SpawnTestContact` uses `GetFirstPlayerController()` and the first pawn. | World startup assumes a single local player; not correct for per-connection spawn/admission. | Extract test-contact setup from world startup; introduce explicit server-side per-login admission/spawn with a deterministic owner association. | Two simultaneous clients have distinct pawns and neither session receives another player's contact/dialogue. |
| SRC-02 | `MORABEZAContactActor.cpp::Interact_Implementation` falls back to `GetFirstPlayerController()`. | Interaction may resolve the wrong player/HUD in multiplayer. | Require valid interactor ownership; handle UI on owning client only; fail closed when no owner can be resolved. | Player A's action never triggers B's HUD/dialogue, including when A disconnects. |
| SRC-03 | `MORABEZAInteractionComponent.cpp::TryInteract` directly executes an interaction after a local trace. | Client-side interaction cannot grant authoritative resource, item, mission or currency outcomes. | Separate local affordance/target presentation from owner-initiated request and server-side target/distance/LOS/authorization validation. | Invalid range, forged target, replayed request and wrong owner cannot change game state. |
| SRC-04 | `MORABEZAPlayerState.h/.cpp` allows local `AddMoney`, `SpendMoney`, `AddReputation` against mutable `SaveGame`-marked properties. | No source-evidenced replicated/transactional multiplayer currency authority. | Introduce MORABEZZA-owned authoritative progression/economy contract; use PlayerState for approved replicated view, not authoritative client writes. | Concurrent spend, duplicate reward, disconnect/reconnect and owner-only visibility are tested. |
| SRC-05 | `MORABEZASaveGame.h` stores money/reputation/location/mission locally. | Client saves must not become MMO character, currency or inventory truth. | Preserve local-only test compatibility as needed; design server-side persistence and safe migration separately. | Restart/reconnect rehydrates owner-only durable state from the game service. |
| SRC-06 | `MORABEZACharacter.cpp` has spring-arm camera, Enhanced Input, movement, jump and sprint. | Existing client experience can be reused but no authoritative multiplayer acceptance exists. | Extract local camera/input ownership into a controller/camera contract and test server-owned movement. | UE dedicated-server + two-client runtime proves movement and separation. |
| SRC-07 | Only `MORABEZA.Target.cs` and `MORABEZAEditor.Target.cs` tracked at the audited SHA. | No source-evidenced dedicated-server build target. | Propose a server target only after build environment/version are validated. | Server target compiles against the pinned engine and a client connects. |
| SRC-08 | `Config/DefaultEngine.ini` uses `/Game/dskjnfkljsdnf` as game/editor map. | Startup selection does not resolve candidate world authority. | Use a separate, clearly disposable test zone for MMO integration; do not rewrite candidate maps. | RM-01 map decision remains separate; test zone recovers from a clean checkout. |
| SRC-09 | README and `Docs/GAME_DESIGN.md` describe prior life-sandbox / single-player gameplay. | Existing scope and proposed MMO target conflict; repository documentation is not yet a product-approved migration. | Review and explicitly approve a versioned product-definition delta before replacing public documentation. | Owner accepts the revised game definition and documents deferred/retained prior features. |
| SRC-10 | No `.github/workflows/` files on audited main; RM-01 LFS workflow exists only on an unmerged draft PR. | Source-level verification and Unreal compilation are separate gaps. | Integrate or adapt approved read-only CI; add Unreal build only on a suitable runner/host. | CI checks report actual source checks vs engine compilation separately. |
| SRC-11 | `Config/DefaultEngine.ini` tracks an Android File Server token field in the public repository. | Potential credential/configuration exposure requires owner-led assessment, not copying a value into logs or docs. | Determine whether value is active and rotate/reconfigure if needed; implement separate security remediation without expanding this PR. | No active embedded secret in the approved public distribution baseline. |
| SRC-12 | Tracked historical `MORABEZADialogueWidget.cpp.*` backup variants. | Historic local snapshots increase source ambiguity; not an immediate MMO blocker. | Reconcile against SSD history; remove only in a separately reviewed cleanup if obsolete. | Final build uses only intended authoritative source and no backup is accidentally compiled. |

## 2. What can be executed remotely today

**Available now without Unreal Editor:**
- Code inspection, versioned architecture, feature acceptance contracts, threat model, schema and API candidate design.
- Independent pure-service logic and tests in a separately approved implementation PR after domain-ownership/schema review.
- Read-only GitHub CI for LFS/source checks and code-quality checks that do not claim Unreal compilation.
- Branch/PR review and test-evidence collection.

**Requires a suitable Unreal build/runtime:** changes to UE RPC declarations, replicated properties, GameMode/GameState/controller behavior, actor spawning, client prediction, native compilation, PIE multiplayer and network packet tests. Writing these files remotely is possible, but promoting them to compile-verified or accepted without that runtime is prohibited.

**Requires SSD reconciliation and map authority:** canonical map freeze, Landscape/World Partition repairs, production art integration and M0 gameplay closure.

## 3. Sequenced implementation PRs (not yet authorized)

### PR-A — Dev multiplayer harness

- New server target and two local clients.
- Disposable, small development zone only.
- Unique per-connection controller/pawn, no first-controller assumptions.
- No persistence, currency or real Lariba credentials.
- Evidence: Unreal 5.8 compile, server launch, two distinct player identities/pawns.

### PR-B — Interaction authority

- Client only displays nearby interactables.
- Owning controller sends bounded interaction intent.
- Server validates actor, world, distance/LOS, rate limits, ownership and actor state.
- Product-server-owned effect and owning-client feedback.
- Evidence: positive and negative network tests, including simultaneous requests.

### PR-C — Durable character and inventory

- Product-local datastore, explicit account-character binding and session revision.
- Server-authoritative item grants with idempotency.
- Safe persistence/reconnect, no client SaveGame currency authority.
- Evidence: concurrent transaction, replay, reconnect and owner-isolation tests.

### PR-D — First bounded gameplay slice

- One enemy/dummy, one resource, one NPC and minimal HUD.
- Elevated camera for the client only; do not infer gameplay authority from HUD.
- Evidence: two-player playtest and captured server logs.

## 4. Acceptance and evidence rules

- Do not change `main`, map/asset packages or the existing saved game format in the candidate-architecture PR.
- Do not use a successful Git LFS checkout as proof of valid Unreal asset references, landscape behavior or gameplay.
- Separate source-only checks, Unreal compilation, networked gameplay and persistence tests in reporting.
- Record exact commit SHA, engine build, test mode, number of clients, test environment, input data class and failing cases.
- Do not enable real Lariba integration until a versioned consumer contract and per-product/environment secret/identity isolation are accepted.
- No productive MMO implementation priority change is implied; the current NODE63 roadmap still places Morabeza in incubation.

**Current gate:** RM-02 design review; PR-A must wait for architecture/portfolio authorization and a suitable Unreal test execution path.
