# AD-01 — MORABEZZA Player Character and Story Heroine

**Decision ID:** MORABEZZA-AD-01  
**Decision date:** 2026-09-25  
**Decision authority:** NODE63 LABS founder (MORABEZZA product)  
**Status:** FOUNDER DECISION RECORDED — OPTION B; GAME PRODUCT-DEFINITION BRANCH ONLY; NO ENGINE/SCHEMA IMPLEMENTATION ACCEPTANCE  
**Source:** The founder explicitly selected **“option B”** in the RM-02 AD-01 discussion, referring to the previously presented choice that **players have their own characters while Morabezza remains the central story heroine**.  
**Repository branch:** `review/rm-02-architecture-decision-docket-20260925` (draft PR #12, stacked on draft PR #3; not merged to `main`).

## Decision

**Option B — Player-specific characters; Morabezza remains the central authored story heroine.**

Each player controls their **own distinct player character** in the shared-world MORABEZZA game, rather than every player controlling an instance of the named heroine Morabezza. The player character has its own game identity and progression under the MORABEZZA product boundary. **Morabezza remains the central narrative heroine** and retains her established named-character visual direction and importance to the game's story.

This is a **product-design identity decision**, not the approval of an authentication service, persistence schema or Unreal gameplay implementation. The precise character-creation/customization options, number of characters per account, public naming rules and gameplay presentation are still to be specified and validated.

## Explicitly retained

- The established female heroine Morabezza and her approved visual reference are preserved. The decision does not rename, replace, remove or silently redesign her.
- The Cape Verde-inspired shared-world MMO/isometric product direction remains a candidate under AD-02's separate formal game-definition and historical-M0 disposition gate.
- Player characters and Morabezza the heroine are **distinct narrative and game identities**; a player character is not implicitly the heroine and does not inherit her authored story authority.
- The existing Unreal project/module spelling `MORABEZA` remains unchanged pending a separate migration decision.

## Deferred decisions and boundaries

1. **Heroine's exact gameplay role:** central story heroine is fixed by AD-01. Whether she is a non-player story character at all times, appears in scripted sequences or becomes playable in bounded narrative chapters is **not decided**; do not classify her as an NPC or approve playable chapters merely by inference.
2. **Avatar creation specifics:** available appearances, character classes/professions, name uniqueness, cosmetics, progression sharing and account-to-character cardinality require a later product/design contract. Do not fabricate a database schema from this decision.
3. **AD-02:** the formal MMO product-definition baseline and disposition of historical `Volta`, vehicles, the third-person M0 and local saves remain pending. No feature is silently removed.
4. **AD-03–AD-07:** game-server topology, persistence, dev/production authentication, canonical-world/test map and asset rights are independent technical gates. Individual character ownership must ultimately be enforced by a MORABEZZA-controlled session and authorization contract.
5. **PC-01:** MORABEZZA's independent NODE63 game-product classification is founder-recorded, while the v1.1 charter incorporation remains in draft governance PR #15; no priority, budget or implementation capacity is implied.

## Acceptance and evidence

**Decision-level acceptance:** founder selection of Option B recorded on this branch and reflected in the RM-02 decision register and game product-definition candidate.

**Not accepted by this decision:** game code, network identity, character database, inventory, art-package delivery, multiplayer runtime, Unreal Engine 5.8 builds, map authority, public README replacement, product release or merger of any draft PR. Draft PR #12 and earlier source PRs remain subject to separate review and acceptance.

**Next product-definition question:** AD-02's legacy scope disposition—retain `Volta` and vehicle gameplay as later MMO features, keep a separate legacy/single-player mode, or explicitly retire the relevant historical M0 requirements—requires an explicit founder choice. It is not decided here.
