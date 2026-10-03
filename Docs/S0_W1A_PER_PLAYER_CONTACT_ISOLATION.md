# S0-W1a — Per-Player Test-Contact Isolation (Unreal Source Candidate)

**Date:** 2026-09-24  
**Repository:** `node63labs/morabezza-game`  
**Branch:** `feat/s0-w1a-per-player-test-contact`  
**Base authority:** `main@af8f3d0e5f9291b1358232c7055190e4bd57b9f6`  
**Status:** SOURCE-REVIEW PASS / UNREAL COMPILATION AND MULTIPLAYER RUNTIME NOT EXECUTED  
**Dependency:** S0-W0 server-target draft PR #4; RM-02 architecture draft PR #3; RM-01 LFS/recovery draft PR #2.

## 1. Narrow purpose

Remove two observed first-player assumptions from the **development-only contact/dialogue path** while preserving the existing Unreal project and candidate maps. This is preparation for an isolated two-client smoke test. It is **not** a product login system, gameplay authorization implementation, trustworthy reward source or multiplayer acceptance.

## 2. Code changes

| File | Candidate change |
| --- | --- |
| `MORABEZAGameMode.h/.cpp` | Replace global BeginPlay/first-controller test-contact creation with `RestartPlayer(AController*)` per-player spawning after the base spawn/possess attempt; store weak owner/contact mappings; remove stale contacts on respawn and Logout; calculate placement from the actual pawn rather than the dedicated server's camera. |
| `MORABEZAGameMode.cpp` | Assign the test actor's Owner and Instigator to its player/pawn. Mark the runtime test contact owner-relevant and replicated. Do not modify production world maps. |
| `MORABEZAContactActor.h/.cpp` | Replicate test-contact name/mission metadata and resolve dialogue controller from the *interacting pawn*, never the world's first player; refuse wrong-owner or missing-owner test dialogue. Only display dialogue on the local owning client. |

A runtime-spawned contact remains a disposable development actor. Its `TEST_INTERACTION` label is **not** a credential, permission or idempotency key. UI-only interaction must not grant durable items, currency, progression or mission completion.

## 3. Source readback checks

GitHub readback of the four edited C++ files completed with all six bounded static assertions true:

- Server-side per-controller restart path present.
- Owner-specific replicated test contact configured.
- `GetFirstPlayerController` absent from edited GameMode and ContactActor sources.
- Test-contact cleanup present on logout/respawn.
- Test-contact metadata replication declarations and lifetime registration present.
- Dialogue controller derived from the interacting pawn; owner and local-controller checks present.

These assertions inspect source text only. They cannot validate generated Unreal headers, Engine 5.8 API compatibility, successful compilation, actor-channel owner relevancy, replicated property delivery, gameplay connection ownership or client-visible dialogue. No GitHub Actions workflow is added or required by this branch.

## 4. Explicit non-goals and unresolved authority

- `UMORABEZAInteractionComponent::TryInteract` still runs its original local interaction path; server RPC verification and authoritative interaction outcomes are reserved for S0-W1b.
- The two candidate production maps and the canonical Landscape/World Partition authority are unchanged and unresolved.
- No game-account or character-session identity, persistence, inventory, currency or live Lariba Cloud integration is created.
- This branch is based on `main` and does not contain draft PR #4's server target/harness. Do not attempt a dedicated-server test from this branch alone until dependent changes are reviewed and integrated.
- Existing public-repository configuration-security review remains a separate change.
- MORABEZZA remains P5/incubation in the current NODE63 portfolio baseline.

## 5. Unreal execution acceptance (future)

With a compatible Engine 5.8 host, approved disposable development map, reviewed S0-W0 harness and compiled server/client binaries, verify:

1. A dedicated server accepts two independent clients; capture server and client logs.
2. Each player spawns a distinct pawn and gets one owner-relevant test contact.
3. Each owner sees its own contact name/mission and can open dialogue on its **own** client.
4. Player A cannot cause player B's dialogue/HUD to open, even after reconnect or respawn.
5. Respawning does not accumulate duplicate test contacts; Logout destroys the correct test contact.
6. A missing owner/controller fails closed without falling back to the first player.
7. No game reward, currency, item or mission completion is granted by local dialogue.
8. Separately validate all future server-side interaction intents in S0-W1b with authoritative target/range, ownership and replay protection.

Until all relevant runtime evidence is recorded, **S0-W1a is not complete for gameplay**, regardless of source-level PASS.
