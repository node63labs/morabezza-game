# S0-W1c — Possession-Safe Input and HUD Initialization

**Date:** 2026-09-24  
**Repository:** `node63labs/morabezza-game`  
**Branch:** `feat/s0-w1c-possession-safe-input-hud`  
**Stacked base:** S0-W1b draft PR #6 at `e7f9ed86b61a8a8f4ee22de2903ef1d68a7f18c2`  
**Status:** SOURCE IMPLEMENTATION + OFFLINE SOURCE REVIEW PASS; UE 5.8 RUNTIME NOT VERIFIED  
**GitHub Actions:** Not used or required for this change.

## 1. Purpose

Address two source-derived lifecycle risks found during S0-W1b review: (1) player input mapping previously depended on a controller already existing during character `BeginPlay`; (2) the HUD bound interaction prompts only if a pawn already existed during HUD `BeginPlay`. Those assumptions may fail when a networked player is possessed or respawned after initial actor initialization.

S0-W1c is intentionally limited to the **owning client's** Enhanced Input mapping and presentation-layer HUD wiring. It does not alter server interaction authority, session identity, inventory, gameplay rewards, world assets, or the two unresolved candidate production maps.

## 2. Input ownership and lifecycle

Updated `MORABEZACharacter.h/.cpp`:

- Retains a safe idempotent `RefreshLocalInputMapping()` call in `BeginPlay`.
- Reconciles mappings after `PawnClientRestart()`, Unreal's owning-client pawn-restart hook, and after `OnRep_Controller()` for replicated possession/unpossession.
- Reconciles again after `SetupPlayerInputComponent`, which can run in a different order from controller availability.
- Locates `UEnhancedInputLocalPlayerSubsystem` only through the locally controlled pawn's local player; simulated proxies and nonlocal server pawns cannot manipulate that mapping.
- Installs the pawn's mapping context only if `HasMappingContext` says it is absent; records the specific subsystem to clean it up.
- Removes only **this pawn's** mapping context on controller/subsystem loss and `EndPlay`. Removes the old `ClearAllMappings()` behavior, which would also erase unrelated menu/vehicle contexts.

This design relies on UE's documented owning-client `PawnClientRestart` lifecycle and Enhanced Input mapping-context APIs, but remains **uncompiled** in the actual pinned UE 5.8 project.

## 3. HUD ownership and lifecycle

Updated `MORABEZAHUD.h/.cpp`:

- HUD `BeginPlay` attempts local UI creation and prompt binding; local HUD `Tick` retries if the owning controller or pawn is not yet available.
- UI widgets are created once after a valid local owning controller is present; widgets are **not** rebuilt on every tick.
- An interaction-component/pawn pointer comparison avoids re-adding the same prompt delegate every frame.
- When the pawn changes or disappears, the HUD detaches the previous prompt delegate, clears stale prompt text, hides dialogue, and unregisters dialogue delegates from the last contact—even when that dialogue had already finished.
- After possession or respawn, it binds the new pawn's interaction component and asks it to refresh the target to avoid losing a prompt emitted before HUD registration.
- HUD `EndPlay` removes prompt and dialogue subscriptions.

The owner-HUD tick performs lightweight reconciliation of existing pointers; its actual timing and performance still require verification in PIE/dedicated-server sessions. Failed widget creation after a valid controller is currently logged and not retried automatically; this remains a bounded runtime acceptance risk.

## 4. Changes and boundaries

| Path | Change |
| --- | --- |
| `Source/MORABEZA/MORABEZACharacter.h/.cpp` | Possession-aware local input lifecycle, idempotent mapping, scoped cleanup. |
| `Source/MORABEZA/MORABEZAHUD.h/.cpp` | Late-controller/pawn UI setup, single-owner prompt binding, stale dialogue cleanup. |
| `tests/mmo/test_s0_w1c_source_lifecycle.py` | Nine standard-library-only offline source regression methods; not executed during this session. |
| `Docs/S0_W1C_POSSESSION_SAFE_INPUT_AND_HUD.md` | This implementation and acceptance record. |

No existing `MORABEZAInteractionComponent`, `MORABEZAContactActor`, `GameMode`, `.uproject`, `Config`, `.uasset`, `.umap`, secret, environment or product database is modified in this PR.

## 5. Offline verification evidence

The actual GitHub branch files were fetched and inspected in a read-only, isolated source-check runner. The final check comprised **35/35 passing source assertions**, including **five in-memory negative mutation fixtures** confirming that removal of a key lifecycle or delegation guard is detected.

The initial negative-fixture prototype replaced a matching delegate-removal statement in HUD `EndPlay` rather than the intended synchronization method. The fixture was corrected to target only synchronization, then the complete check was rerun at **35/35 PASS**. Neither run changed repository files.

The committed Python suite contains **nine** test methods. It has **not** been executed against an independently checked-out repository. No Unreal Header Tool, C++ compiler, PIE session, dedicated server, two-client session, or GitHub Actions run was used.

### Local/offline Python command for a future checkout

    python3 -m unittest discover -s tests/mmo -p 'test_s0_w1c_source_lifecycle.py' -v

This verifies source guardrails only; a PASS is not proof of a working engine runtime.

## 6. Runtime acceptance matrix (NOT EXECUTED)

| ID | Scenario | Expected result |
| --- | --- | --- |
| L01 | Owning client pawn exists before BeginPlay | WASD / Space / Shift / E work; one mapping context. |
| L02 | Controller arrives after character BeginPlay | PawnClientRestart/controller replication installs input without requiring another BeginPlay. |
| L03 | PawnClientRestart fires repeatedly | No duplicate mappings or duplicate E-key effects. |
| L04 | Remote simulated pawn and dedicated server pawn | Never registers a mapping in an unrelated local-player subsystem. |
| L05 | Client swaps pawns or respawns | Old pawn mapping removed; new pawn mapping present; unrelated UI contexts retained. |
| L06 | HUD BeginPlay runs before the owner receives a pawn | Interaction prompt binds when the pawn later becomes available. |
| L07 | Pawn lost/destroyed while dialogue is active or completed | Old prompt and dialogue delegates removed; stale text/dialogue hidden. |
| L08 | Respawn then interact with own replicated test contact | Exactly one local HUD receives the new pawn's prompt and dialogue. |
| L09 | Two connected clients | A's input/HUD stays isolated from B; no cross-player contact/dialogue. |
| L10 | UI widget creation failure / delayed asset availability | Error is logged; recovery policy reviewed separately if it occurs. |
| L11 | S0-W1b authorization regression | RPC ownership, target/range/LOS checks and server-only effects still pass their own tests. |

## 7. Integration order and gate

RM-01 draft PR #2, MMO architecture draft PR #3, dedicated-server target draft PR #4, per-player contact draft PR #5 and server interaction draft PR #6 remain independently controlled. S0-W1c is **stacked on PR #6**. This candidate may not be promoted to gameplay acceptance until upstream review, compatible Unreal 5.8 compilation, disposable-map validation, and the above two-client runtime tests.

**Current outcome:** source-level lifecycle mitigation is committed and reviewed; runtime behavior and full multiplayer acceptance remain unverified.
