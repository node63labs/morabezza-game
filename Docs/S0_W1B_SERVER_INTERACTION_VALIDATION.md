# S0-W1b — Server-Authoritative Interaction Validation (Candidate)

**Date:** 2026-09-24  
**Repository:** `node63labs/morabezza-game`  
**Branch:** `feat/s0-w1b-server-interaction-validation`  
**Stacked base:** S0-W1a draft PR #5 at `6962de04e7403af8fa8dbb2624fad504e64b7276`  
**Status:** SOURCE IMPLEMENTATION CANDIDATE — UNREAL COMPILE AND NETWORK RUNTIME NOT VERIFIED  
**Validation environment:** GitHub read-only source inspection; **no new GitHub Actions workflow or run required**.  
**Scope:** Development-only `TEST_INTERACTION` contact; no inventory, reward, mission completion, trade, currency or player account changes.

## 1. Objective

Change the existing E-key workflow from a client-executed interface call to an **owner-pawn server RPC** that treats the selected target as untrusted, repeats interaction geometry and ownership checks using server state, and invokes the development contact interaction only on the server. The accepted contact then sends cosmetic dialogue to the correct owning client. The existing standalone, unowned map-contact dialogue route is retained as a compatibility path; no canonical production map is declared or modified.

This is a bounded precursor to general product-domain interaction. It does not claim secure identity, durable replay protection, anti-cheat or successful two-client gameplay.

## 2. Files and contracts

| File | Candidate change |
| --- | --- |
| `MORABEZACharacter.h/.cpp` | Explicitly replicates the character; defines reliable `ServerTryInteract(AActor*)` on the owning pawn and cosmetic `ClientPresentValidatedContact(AMORABEZAContactActor*)` on the same pawn. Server RPC delegates to the server-only interaction component; client RPC can only present local dialogue. |
| `MORABEZAInteractionComponent.h/.cpp` | Keeps local tracing as a prompt/candidate only; never invokes an interaction interface from local input. Adds server-only candidate validation and per-component request spacing. |
| `MORABEZAContactActor.h/.cpp` | Separates server interaction acceptance from local cosmetic dialogue; validates the actual interactor's controller, rejects wrong owner and sends a client RPC only for the server-accepted, replicated contact. |
| `tests/mmo/test_s0_w1b_source_contract.py` | Offline standard-library regression assertions for source guardrails; **not** an Unreal test or executed CI check. |
| `Docs/S0_W1B_SERVER_INTERACTION_VALIDATION.md` | This scope, evidence and runtime acceptance specification. |

The test contact depends on S0-W1a's owner-only replicated spawning; this PR is stacked directly on PR #5, **not** based on `main`. S0-W0's dedicated-server target in draft PR #4 remains a separate, unmerged prerequisite for future real runtime tests.

## 3. Request and authority model

```text
Locally controlled player presses E
  -> InteractionComponent::UpdateInteractionTarget() [COSMETIC CANDIDATE]
  -> local TryInteract() chooses actor pointer; no local Execute_Interact
  -> owning Character::ServerTryInteract(RequestedTarget)
  -> server InteractionComponent::ExecuteServerInteraction(Target)
       controller currently possesses this character?
       rate limit?
       target valid + same world + implements interface?
       in networked mode, is the target MY replicated TEST_INTERACTION contact?
       server distance <= 350 UU, forward sphere sweep first-hit match?
       independent server line-of-sight obstruction check?
  -> server-only IMORABEZAInteractable::Execute_Interact(Contact, Character)
  -> contact validates interactor + private ownership
  -> owning character ClientPresentValidatedContact(Contact) [COSMETIC ONLY]
  -> local owner-only contact PresentDialogueToLocalPlayer() [HUD ONLY]
```

A standalone local game calls the server-authoritative implementation directly because the local game also has authority. In a networked game the server accepts **only** the owner-scoped, replicated `TEST_INTERACTION` development actor; other actor classes and map-placed/unowned contacts are rejected in this slice. A client with an invalid or stale target receives no gameplay effect.

## 4. Threat checks and limitations

| Condition | Server-side handling |
| --- | --- |
| Forged actor pointer, invalid/deleted actor, different world, non-interactable | Refuse |
| Request from pawn not possessed by current player controller | Refuse |
| Another player's owner-scoped contact | Refuse |
| Unowned/world actor in networked mode | Refuse; standalone compatibility remains |
| Candidate beyond server cap or behind the player | Refuse |
| First blocking sweep hit is not candidate | Refuse |
| Blocking geometry on the subsequent sight line | Refuse |
| Rapid repeat request inside 0.35 s | Refuse |
| Valid owned test contact within range and unobstructed | Call development contact interaction **on the server** |
| Client receives acknowledgement | Present cosmetic dialogue only; no durable reward |

**Not yet established:** authenticated account-to-character binding, authoritative consumable-item rules, exactly-once semantics, replay ledger, robust denial-of-service protection, generic interaction permissions, UE5.8 compile, network-packet replication behavior, server/client interest management and production trust-boundary acceptance. A throttle is not an idempotency contract. Arbitrary Blueprint calls outside the audited E-key path are not made authoritative by this PR; real domain effects must be redesigned separately.

## 5. Offline source checks (no GitHub Actions)

The repository includes `tests/mmo/test_s0_w1b_source_contract.py`. On any machine with this branch checked out and Python 3:

    python3 -m unittest discover -s tests/mmo -p test_s0_w1b_source_contract.py -v

These tests check source guardrails only. The GitHub connector's source readback may be used for a bounded remote source review without Actions. A successful Python run **would not** prove Unreal compilation, a functioning server RPC or authoritative combat/inventory.

No GitHub Actions workflow is added or modified by S0-W1b, and this branch must not be merged just because source assertions are present.

## 6. Required Unreal runtime acceptance (NOT EXECUTED)

With a compatible Unreal 5.8 build, the reviewed S0-W0 server target, an approved disposable development map and S0-W1a integration, run both clients against a single dedicated server. Capture exact engine/compiler/source SHA, server and client logs, valid and invalid actor identities, test coordinates/obstacles, RPC/log ordering, and disconnect behavior.

| Test | Acceptance |
| --- | --- |
| T01 — own contact in range, unobstructed | Exactly one server acceptance and one owner-only dialogue, no reward |
| T02 — player A supplies B's actor reference | Rejected; A/B HUD unchanged |
| T03 — stale/deleted actor or target in another world | Rejected safely; no crash or event |
| T04 — target more than 350 UU away | Rejected regardless of client visual prompt/distance config |
| T05 — target blocked by wall | Rejected; no dialogue |
| T06 — behind-pawn or non-first sweep target | Rejected |
| T07 — repeated E in less than 0.35 s | Rate-limited by authoritative server; no duplicate server effect |
| T08 — respawn/logout and reconnect | Old actor/ownership cannot open dialogue; fresh contact available after per-player spawn |
| T09 — dedicated server no local HUD | Owner client's HUD only; no first-controller fallback |
| T10 — local standalone existing contact | Existing dialogue compatibility retained if valid |
| T11 — client attempts direct interface or UI call | No durable server effect; UI is not game authority |

**Acceptance classification:** source-code candidate. Do not mark server-authoritative multiplayer interaction PASS until T01–T11 are executed with captured evidence and relevant defects are resolved.

## 7. Integration and merge order

- RM-01 draft PR #2: LFS restoration evidence; full SSD/world recovery remains pending.
- RM-02 draft PR #3: MMO architecture review remains open; no portfolio implementation-priority change implied.
- S0-W0 draft PR #4: dedicated-server target and harness source-only preflight.
- S0-W1a draft PR #5: per-player owner-scoped test contact; **direct parent branch for this PR**.
- S0-W1b: this PR. Keep draft until upstream decisions, source review, UE compile and multiplayer runtime evidence are complete.

No Unreal binary assets, `.uproject`, maps, existing world authority, repository visibility or Lariba services are modified by this change.
