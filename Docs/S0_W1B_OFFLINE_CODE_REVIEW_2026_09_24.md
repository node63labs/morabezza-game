# S0-W1b — Offline Code Review and Source-Contract Test Evidence

**Review date:** 2026-09-24  
**Scope:** GitHub branch `feat/s0-w1b-server-interaction-validation` (stacked on S0-W1a PR #5)  
**Review source after test-suite correction:** `5fa6c326e9c4de138465dc57a5751c75c1549737`  
**Review disposition:** SOURCE-CONTRACT CHECKS PASS; RUNTIME / COMPILE ACCEPTANCE OPEN  
**Execution location:** Isolated in-conversation JavaScript source-review runner with read-only GitHub connector; **no GitHub Actions usage**.

## 1. Review method and evidence boundaries

Live GitHub file content was fetched from the exact S0-W1b branch. The review examined:
- `MORABEZACharacter.h/.cpp`;
- `MORABEZAInteractionComponent.cpp`;
- `MORABEZAContactActor.cpp`;
- `MORABEZAGameMode.cpp` (stacked S0-W1a dependency);
- `tests/mmo/test_s0_w1b_source_contract.py`;
- `Docs/S0_W1B_SERVER_INTERACTION_VALIDATION.md`;
- separate readback of `MORABEZAHUD.cpp` and `Dialogue/MORABEZADialogueComponent.cpp` for lifecycle and effect-boundary review.

The offline checker evaluated the actual fetched source as strings. It executed 27 bounded assertions, including five deliberately corrupted **in-memory** source variants to check whether the review predicates detect missing owner/range/line-of-sight guards and added client-side effects. No repository file was mutated by those in-memory fixtures.

**Result: 27 of 27 assertions PASS; five of five negative mutation fixtures detected.** This establishes only that the reviewed source contains the checked guardrails in the expected method/order structure. It does **not** execute the committed Python tests or prove the behavior of C++ code.

An initial ad-hoc mutation fixture only removed one occurrence of an owner-check string where the source legitimately had more than one; that fixture was corrected to remove all occurrences. The corrected full check completed 27/27. This was a defect in the review fixture, not a demonstrated runtime defect.

## 2. Source review observations

| ID | Evidence from inspected source | Finding / disposition |
| --- | --- | --- |
| REV-01 | Owning character declares `UFUNCTION(Server, Reliable)`; local `TryInteract` delegates the candidate to the server and has no direct `Execute_Interact` call. | Expected routing **present in source**. RPC runtime ownership not verified. |
| REV-02 | Server method checks possessed pawn, valid target/world/interface, owner-scoped test contact, hard range cap, forward sphere sweep, line-of-sight and request spacing before invoking the interface. | Bounded allowlist/guard order **present in source**. Collision behavior and rate-limit behavior not exercised. |
| REV-03 | Contact effect rejects non-authority/wrong owner; valid server request leads to `ClientPresentValidatedContact` and a local cosmetic dialogue presentation. | Cosmetic-only game-domain boundary **present in audited path**; other Blueprint implementations of the interface are out of scope. |
| REV-04 | `MORABEZAHUD::BeginPlay` binds `OnPromptChanged` only if a pawn is already available; otherwise it returns without a later rebind in this file. | **Runtime risk:** on asynchronous possession/respawn, interaction prompt may remain unbound. Verify and address in a dedicated HUD/possession-lifecycle PR. |
| REV-05 | `AMORABEZACharacter::BeginPlay` installs the input mapping only when `GetController()` and `GetLocalPlayer()` are available at that moment. No subsequent retry was found in the inspected character file. | **Runtime risk:** owning client might miss E-key/movement mappings when possession arrives after BeginPlay. Verify and address before two-client acceptance. |
| REV-06 | Client acknowledgement refers to a replicated owner-only contact actor; its dialogue component provides local prototype lines. | **Runtime risk:** replication/ownership delivery and dialogue availability have not been observed on a dedicated-server client. Test after Unreal compilation. |
| REV-07 | Request spacing is an in-memory 0.35-second throttle; no reward or database effects are wired into this slice. | **Scope limitation:** do not describe this as durable replay prevention, inventory idempotency or an MMO authorization system. |
| REV-08 | S0-W1b is stacked on PR #5, while S0-W0 target/harness is a separate draft PR #4. | **Integration blocker:** tested two-client build requires explicit reviewed integration; no independent multiplayer runtime exists in this branch. |

The HUD/input findings are **plausible lifecycle risks derived from source control flow**, not confirmed defects. No Unreal runtime is available to establish whether the lifecycle ordering actually causes either failure. No existing gameplay, map or binary asset was changed to address them during this review.

## 3. Offline test-suite improvement

The originally committed Python source-test file included one vacuous assertion that only checked whether `MORABEZACharacter.h` existed. That test has been replaced with checks for owner-scoped network prompts, absence of first-player fallbacks, confinement of interface effects to the server method, and absence of reward/effect calls in cosmetic dialogue. The file now contains **eight source-contract test methods**.

The committed Python suite is executable by a later checkout with Python 3:

    python3 -m unittest discover -s tests/mmo -p 'test_s0_w1b_source_contract.py' -v

**Python suite status: NOT EXECUTED in this review.** The 27/27 result is from an independent JavaScript source-contract check against live GitHub content, not from this command. GitHub Actions was neither required nor invoked.

## 4. Required follow-up before runtime acceptance

1. On a compatible Unreal Engine 5.8 host, compile the stacked source with S0-W0 and S0-W1a integrated in an approved test branch; record engine/compiler/commit provenance.
2. Use only a disposable development map. Preserve `Untitled` and `dskjnfkljsdnf` as unresolved RM-01 candidate worlds.
3. Validate possession ordering and HUD/input binding before assuming a two-client E-key playtest is meaningful.
4. Execute S0-W1b T01–T11, including valid own-contact interaction, wrong-owner pointer, range/LOS, request spacing, stale actor, respawn/logout and dedicated-server local-HUD isolation.
5. Test client dialogue receipt when the owner-only actor is newly replicated, including stale/reconnect behavior.
6. Preserve the rule that cosmetic test dialogue cannot grant money, items, inventory changes or mission completion.

**Acceptance:** source review complete, but S0-W1b remains DRAFT / NOT UNREAL-VERIFIED.
