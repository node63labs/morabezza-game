# RM-02 — MORABEZZA MMO Runtime Architecture V1 (Candidate)

**Document ID:** MORABEZZA-RM-02-MMO-RUNTIME-ARCHITECTURE  
**Date:** 2026-09-24  
**Status:** CANDIDATE — REVIEW REQUIRED; NO RUNTIME IMPLEMENTATION AUTHORIZATION  
**Repository:** `node63labs/morabezza-game`  
**Source authority inspected:** `main@af8f3d0e5f9291b1358232c7055190e4bd57b9f6`  
**Prerequisite evidence:** RM-01 GitHub-hosted Git LFS recovery PASS (293/293 assets); SSD reconciliation, Unreal build and canonical world authority PENDING.  
**Change class:** Product-definition / architecture proposal; no world, binary-asset, service or production mutation.  
**Portfolio disposition:** Morabeza remains P5/incubation in NODE63 Master Roadmap v1.4 until a separate portfolio approval changes its implementation priority.

## 1. Purpose and authority

Define a reviewable path from the existing MORABEZA Unreal C++ prototype to the *proposed* MORABEZZA isometric MMORPG without mistaking concept art, local development history or source-file presence for accepted implementation. This candidate does not amend NODE63 portfolio gates, approve deployment, declare the existing maps authoritative or promote any MMO subsystem to PASS.

The repository currently uses the engine/module spelling `MORABEZA`. The proposed player-facing game identity is `MORABEZZA`. Preserve module, asset and package names until a separately reviewed naming/migration decision; Unreal asset reference redirects can be fragile.

## 2. Observed repository baseline (not the proposed design)

| Source | Audited observation |
| --- | --- |
| `MORABEZA.uproject` | Unreal EngineAssociation 5.8; EnhancedInput and MoverExamples enabled. |
| `MORABEZACharacter.cpp/.h` | Character, spring-arm camera, Enhanced Input, movement, jump, sprint, interaction and procedural-character component. |
| `MORABEZAInteractionComponent.cpp` | Character-forward sphere sweep and direct `IMORABEZAInteractable::Execute_Interact` call. This is a local implementation, not authoritative MMO validation. |
| `MORABEZAGameMode.cpp` | Spawns a test contact by looking up the world's first player controller and its pawn; this assumption is not appropriate for multiple independent sessions. |
| `MORABEZAContactActor.cpp` | Uses the interactor's instigator controller, then falls back to first player controller; the fallback must not route one player's dialogue to another. |
| `MORABEZAPlayerState.h/.cpp` | Mutable local Money/Reputation methods, with no source-evidenced server authority or network-replication acceptance. |
| `MORABEZASaveGame.h` | Local SaveGame fields for money, reputation, location, rotation and mission; not an MMO persistence authority. |
| `Config/DefaultEngine.ini` | Startup map `/Game/dskjnfkljsdnf`, which is a setting, not accepted canonical-world proof. |
| `Docs/GAME_DESIGN.md` and README | Older action/life-sandbox / single-player assumptions. They are historical source material and must not be silently replaced. |

No dedicated server target, proven cross-player replication, authenticated MMO session, server-owned inventory or independent player persistence was accepted in this audit. A complete Unreal build has **not** been executed.

## 3. Proposed player experience

**Candidate genre:** Single-shard-or-multi-shard MMORPG (topology TBD), elevated isometric/open-zone action adventure. The game world and art direction are Cape Verde-inspired; the implementation must not assume a generic medieval MMORPG or reproduce another game's proprietary assets.

**Candidate visual/camera contract:** elevated ~45–55 degree camera, bounded zoom and optional controlled rotation; sufficiently small character scale for group combat/readability, with contextual close views for dialogue and story. This is a design target, not a validated Unreal camera setting.

**Candidate gameplay loop:** explore zone → encounter players/NPCs → combat, gather, interact or discover → product-server-authoritative outcome → inventory/progression → further world exploration. The first slice proves one traversal, one NPC interaction, one enemy, one resource and persistent ownership.

**Unresolved avatar decision:** the existing named Morabezza protagonist is the canonical reference for visual design. Whether every player controls a personalized hero, a character with a unique name, or a specific narrative protagonist must be resolved before approving account/character schema or cosmetics.

## 4. Runtime and authority topology (candidate)

```text
Unreal Client (rendering, input, HUD, prediction)
   |
   | versioned connection/session ticket (validated on entry)
   v
MORABEZZA Session/World Directory  ---> optional approved Lariba identity adapter
   |
   +-- Dedicated Unreal Zone Server A (authoritative simulation)
   |      movement / combat / NPC / interactive resource authority
   |
   +-- Dedicated Unreal Zone Server B (later; explicit handoff protocol)
   |
   +-- MORABEZZA Product Services
          character ownership / inventory / progression / durable economy
          |
          +-- MORABEZZA-owned datastore (candidate: PostgreSQL)
          |
          +-- optional Lariba adapters: service identity, secrets,
              domain-event delivery, telemetry, release provenance
```

**Authority invariants:**
1. The client sends *intent*, never final position, damage, currency, loot or inventory truth.
2. A game server validates each consequential action against authenticated session, current zone, game state, distance/line of sight, cooldown, action version and ownership.
3. An operation that changes durable character state has an idempotent transaction boundary and uniquely attributable server authority.
4. Client rendering/prediction may lead UI feedback but must reconcile to authoritative simulation.
5. A zone transition transfers ownership through an explicit protocol (session fencing, state checkpoint, destination admission, acknowledgement and rollback/retry); Unreal World Partition streaming alone is **not** multi-server sharding.
6. Player/character authorization and disciplinary policy belong to MORABEZZA, even when upstream authentication is supplied by Lariba.
7. MORABEZZA does not read/write Lariba's internal databases; shared capability use is via approved versioned contracts.

### 4.1 Unreal class ownership (candidate; not implemented)

| Unreal type | Intended responsibility |
| --- | --- |
| `AMORABEZAGameMode` | Server-side zone rules and spawn/entry admission; no first-controller assumptions. |
| `AMORABEZAGameState` (new) | Replicated public zone state only. |
| `AMORABEZAPlayerController` (new) | Owning connection, input/RPC ownership, client-side HUD, local camera. |
| `AMORABEZAPlayerState` (evolve) | Only approved replicated public character metadata; money and inventory must not be globally exposed. |
| `AMORABEZACharacter` (evolve) | Predicted movement and representation of server-owned actor state; existing local functionality preserved behind explicit server contracts. |
| `UMORABEZAInteractionComponent` (evolve) | Local target presentation + owner-requested server RPC; server resolves and revalidates target. |
| `UMORABEZAAuthoritativeInventoryComponent` (candidate) | Owner-only replicated view of a MORABEZZA-server-authoritative inventory model. |
| `AMORABEZAZoneResourceActor` (candidate) | Server-owned resource state with bounded interactions. |

Do not implement these class additions in the architecture PR. Review Unreal version compatibility and compile them only in separately bounded implementation PRs.

### 4.2 Network/session contract (candidate)

- Connection entry receives a short-lived, audience-bound session ticket associated with `AccountId`, `CharacterId`, `ZoneId`, session revision and expiry; replay, wrong audience and revoked sessions fail closed.
- Server RPCs are only issued by a controller/actor owned by the requesting connection. The server derives actor identity from the authenticated connection; client-supplied actor/character identifiers are untrusted selectors.
- Interaction requires an observed target reference, the authoritative position/LOS and product-level permissions. The server checks inventory capacity and resource availability atomically before granting items.
- Interest management and replication frequency are bounded by an actual performance test; don't invent a player-capacity figure or network tick SLO before measurements.
- Cross-zone admission and global chat/social systems are **out of the first slice** unless specifically approved.

## 5. Product data and persistence (candidate)

```text
Account          upstream auth subject mapping; owned Morabeza account record
Character        stable character identity + ownership + progression
CharacterSession active zone, revision, expiry, fencing
Inventory        server-authoritative item instances and quantities
ResourceNode     zone-owned state; durable respawn policy TBD
RewardOperation  idempotency key, authoritative outcome and provenance
```

Recommend a MORABEZZA-owned relational store for durable account/character/inventory data; PostgreSQL is a candidate to approve during domain-schema review. Zone simulation state may be held in memory and checkpointed deliberately; no client-side SaveGame can be authoritative for multiplayer currency or owned items.

Required negative tests before acceptance: forged ownership, duplicate pickup/reward, concurrent spend, replayed interaction, stale/expired ticket, zone mismatch, disconnected/reconnected player, and failure during a durable commit. No player data, production credentials or production DB mutation in development/CI.

## 6. Lariba Cloud consumer boundary (candidate, not automatically enabled)

| Capability | Proposed consumption | Constraint |
| --- | --- | --- |
| Authentication / service identity | Conditional | Use an approved versioned contract; product owns character authorization and bans. |
| Secrets / environment identity | Conditional | Per-product, per-environment and per-service binding; no secrets in public repo or client build. |
| Events / observability | Optional non-real-time | After MORABEZZA commits authoritative domain outcome, publish bounded events/telemetry; never route each simulation frame through Lariba. |
| Audit/evidence / release provenance | Candidate | Platform provides primitives; MORABEZZA retains game-domain truth, provenance and operational responsibility. |
| Managed infrastructure | Future candidate | Deployment and server scaling require separate authorization and observed operational capacity. |

No first-slice runtime hard dependency on incomplete/unaccepted shared contracts. Nonproduction synthetic or approved dev identity can test a local server, but production login cannot bypass the later approved identity/security contract. Each dependency must eventually specify version, auth, timeout, retries, degraded behavior, ownership and security tests. MedicamentOS remains the first planned Lariba consumer under the current portfolio roadmap.

## 7. First MMO vertical slice — scope

**Working designation:** MMO-S0.1 / ONE ZONE, TWO PLAYERS. The label is a planning candidate, not a claim that the old M0 milestone was completed or replaced.

In scope:
- a temporary recovery-safe development zone, not a replacement of either unresolved canonical Landscape candidate;
- two simultaneously connected players with separate account/character bindings;
- server-authoritative movement and one interaction;
- one NPC/contact and one server-owned collectible/resource node;
- one limited enemy encounter or dummy with server-authoritative damage;
- a single durable reward/inventory operation persisted across logout/reconnect;
- basic elevated isometric camera and player name/health visibility;
- repeatable local/CI evidence where the available environment permits.

Out of scope: whole-island simulation, large population, PvP, guilds, trading, global economy, production Lariba integration, cross-zone handoff, shipping client, and Unreal map/art rewrites.

**Blocker separation:** documentation and pure service/schema design can proceed remotely now. Gameplay acceptance requires an Unreal-compatible compiler/runtime and a verified development map. The RM-01 canonical-world gate remains separate and unresolved.

## 8. Proposed implementation order (each on a bounded branch/PR)

1. **RM-02 acceptance:** reconcile genre, avatar model, domain ownership, Lariba boundary and roadmap priority through explicit review.
2. **S0-W0:** dedicated-server target and two-client connectivity harness; no current-map mutation; compile/PIE test required.
3. **S0-W1:** player controller, per-connection spawn and authority-safe character representation.
4. **S0-W2:** authenticated dev session and validated server-side interaction request; remove first-player fallbacks in a reviewed change.
5. **S0-W3:** resource/reward operation and MORABEZZA-owned persistence with idempotency and owner-only replication.
6. **S0-W4:** basic combat dummy and negative security tests.
7. **S0-W5:** elevated camera/HUD integration and measured two-player acceptance.
8. **S0-W6:** review Lariba consumer contracts and only integrate capabilities that pass their separate platform readiness gates.

No step is COMPLETE merely because a source file or mock demonstration exists.

## 9. Required architecture decisions before structural implementation

- AD-01: player-character identity versus named Morabezza protagonist.
- AD-02: final genre/scope and approved change to old action/life-sandbox roadmap.
- AD-03: Unreal dedicated server topology and development hosting capacity.
- AD-04: authoritative game persistence, durability and reconciliation contract.
- AD-05: initial auth/session contract and whether Lariba is optional or required by phase.
- AD-06: first playable map and how it coexists with unresolved RM-01 canonical-world authority.
- AD-07: license and visibility handling for code, third-party assets and artist deliverables.

**Disposition:** CANDIDATE FOR FOUNDER REVIEW. Existing single-player files are not rewritten, removed or reclassified by this document.
