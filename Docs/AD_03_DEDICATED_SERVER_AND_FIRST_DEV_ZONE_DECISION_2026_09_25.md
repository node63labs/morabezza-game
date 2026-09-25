# AD-03 — Dedicated Server Topology and First Development-Zone Boundary

**Decision ID:** MORABEZZA-AD-03  
**Date:** 2026-09-25  
**Decision class:** Bounded first-slice runtime architecture; source/design review, no runtime acceptance.  
**Status:** FOUNDER APPROVED — BOUNDED AD-03 DESIGN ONLY; IMPLEMENTATION / HOSTING / PRODUCTION TOPOLOGY NOT AUTHORIZED.  
**Provenance:** The founder explicitly approved the proposed **one-dedicated-server, one-disposable-development-zone, two-independent-client design** on 2026-09-25, after reviewing its bounded scope. This approves AD-03's first-slice architecture, **not** a particular cloud provider, machine, capacity, cost, production shard topology, implementation PR or merge. PC-01, AD-01 Option B and AD-02 remain the prior product decisions.  
**Review branch:** `review/rm-02-architecture-decision-docket-20260925`, draft PR #12 stacked on draft PR #3. The later S0-W0/W1a/W1b/W1c/INT-02 candidate source exists on separate draft PR #11, not in this branch's Git ancestry.  
**Game source authority:** `morabezza-game/main@af8f3d0e5f9291b1358232c7055190e4bd57b9f6` remains unchanged.  
**Governance authority:** MORABEZZA founder classification recorded in PC-01; its approved charter incorporation remains draft governance PR #15.

## 1. Bounded AD-03 design direction

For the **first development-zone / two-client proof**, use **one standalone Unreal Engine 5.8 dedicated-server process, one explicitly disposable development map, and two independently connected Unreal game-client processes**. The **one server** is authoritative for the single zone's active gameplay simulation. Both clients connect to the **same** zone server; there is no client-hosted listen-server fallback, peer-to-peer gameplay authority, second server, server handoff or production matchmaking requirement for this proof.

**Topology is intentionally limited to the first slice.** No production decision is made between single-shard and multi-shard, between one machine and separate client machines, on player capacity, region, cloud provider, DNS, deployment automation, billing, service-level objectives or persistent server availability. A headless server build host may differ from the later host capable of running two graphical clients; no host is currently available or procured.

The existing `MORABEZAServer.Target.cs` on draft PR #11 declares `TargetType.Server` for UE 5.8, but it has **not** passed UHT/C++ or executable-launch verification. The S0-W0 smoke script is a **launch/liveness harness**, not a connection, replication, session-identity or security test.

## 2. Logical control and authority boundary

```text
Client A (distinct dev character A)     Client B (distinct dev character B)
rendering/input/HUD; client intent        rendering/input/HUD; client intent
             \                              /
              \  separate connections      /
               v                          v
                   ONE UE 5.8 dedicated
                    zone-server process
                 (disposable dev zone)
                 - connection/pawn owner
                 - simulation and movement
                 - validated test interaction
                 - public replicated zone view
                       |
                       v
             MORABEZZA-owned game domain
          (future sessions and persistence)
          AD-04 and AD-05 NOT YET APPROVED
```

**MORABEZZA owns the zone/server game-domain authority.** The clients may request actions and render/predict local presentation but do not decide shared player identity, final movement, target authority, damage, inventory, rewards or currency. The server derives the requesting connection's controller/pawn ownership from server-held state; a client-supplied character, actor or outcome must not be accepted merely because it appears in an RPC payload. Existing draft W1a/W1b per-player development-contact code is a **source candidate**, not accepted multi-player authorization.

**GameMode** is authoritative server-side zone/spawn policy; **GameState** is a future bounded replicated public-zone view; player-scoped state and UI stay owner-scoped. Globally replicated state must not expose private inventory, account details or secrets. Game identity/ban/session policy remains MORABEZZA-owned even if a later versioned Lariba authentication adapter is approved. Lariba and the Morabeza hospitality product are **not** required to make the first disposable zone operate.

**Admission design is bounded by AD-05:** first-slice test clients may use separately approved synthetic/dev identity, but no currently implemented or accepted authenticated account-to-character admission is evidenced. The eventual admission contract must validate distinct account/character bindings, target zone, session revision and revocation before authorizing a pawn. Do not treat two operating-system processes or two client windows as proof of distinct authenticated players.

## 3. First development-zone world boundary

**Planned test-only map package path:** `/Game/Dev/MMO_S0_TestZone`. This path is used in the existing S0-W0 launch-harness documentation and conforms to its `/Game/Dev/<Name>` input guard. **The map asset does not currently exist as a verified tracked/hydrated Unreal package.** Choosing a planned name does not create a `.umap`, prove cook/load, or authorize a landscape copy.

The first-zone asset, if later created under AD-06 on an authorized UE 5.8 host, must be:
- **Independent and disposable**, built as a minimal representative level for spawn, navigation, two-player visibility, one dev contact and later server-owned test interactions.
- Located under `/Game/Dev/` with a reviewed, explicit asset path. The launcher must pass the dev map explicitly; do **not** change `GameDefaultMap`/`EditorStartupMap` or load the unresolved candidate maps by default.
- **Not** `Content/Untitled.umap` or `Content/dskjnfkljsdnf.umap`. Those remain unresolved RM-01 canonical-world candidates. No rename, destructive edit, Landscape rebuild, World Partition conversion, redirect or asset deletion is authorized.
- Separately created, reviewed and recovered with a real pinned Git checkout and hydrated Git LFS objects. The user's source ZIP with 293 pointer placeholders cannot prove valid map content.

For S0 there is **one zone identifier** and **no zone crossing or World Partition-to-server-handoff inference**. Unreal streaming inside a map and transfer between authoritative server processes are distinct concerns. Production world placement, geographic fidelity and island expansion remain future design work.

## 4. Runtime life cycle and explicit non-goals

**Connection/spawn candidate:** server starts on the verified dev map and a local, non-publicly exposed test endpoint; dev admission verifies the approved AD-05 test identity and zone; the server binds each connection to its own controller/pawn; each client's UI and dev contact resolve only its own owner. The server observes both connections and the relevant other pawn for cross-client presence.

**Disconnect/logout:** revoke the departing connection's authority over its pawn and dev contact, clean up its transient zone state and ensure the other connection remains unaffected. Reconnect/restart with stable character ownership and session fencing are separate **AD-05** and **AD-04** test requirements; the local client `SaveGame` cannot restore authoritative multiplayer currency or ownership.

**First-slice isolation:** no second zone, cross-zone transfer, hosted directory service, global chat, full Volta mission, drivable vehicles, NPC economy, persistent rewards, PvP, guilds, trading, production identity adapter, game scaling automation or production server deployment is authorized by AD-03. The broader MMO-S0.1 target still separately requires a limited enemy/resource and a single persistent owner-specific reward once AD-04/05 are approved; none is accepted now.

**No dependency inversion:** AD-03's single-zone topology does **not** authorize creating a production session/world-directory database. A future multi-zone topology needs its own capacity evidence, session handoff protocol, state checkpoint/fencing, destination admission, failure/rollback handling, versioned service contracts and separate owner review.

## 5. Evidence and acceptance gates

| Gate | Required future evidence | Current classification |
| --- | --- | --- |
| AD-03 design | This bounded one-server/one-zone/two-client topology with owner-reviewed scope and explicit exclusions. | **FOUNDER APPROVED at the design level; not a build or gameplay acceptance.** |
| INT-02 host/source | UE 5.8 source engine, supported toolchain/plugins, exact reviewed Git SHA, hydrated LFS packages and clean checkout; Editor/Game/Server UHT+C++ target outputs/logs reviewed. | **BLOCKED — no host or compile.** |
| AD-06 dev map | Actual disposable `/Game/Dev/MMO_S0_TestZone` package created and verified; original two candidate maps preserved. | **NOT PRESENT / NOT VERIFIED.** |
| AD-05 session identity | Approved synthetic dev admission first, then versioned character/session binding and negative entry tests; no unreviewed production login. | **PENDING.** |
| INT-03 connection lifecycle | Same source/server binary and dev map; server listen and two distinct authenticated dev connections/pawns proven by server and both client logs, replication and owner-only UI, clean logout/respawn. | **NOT EXECUTED.** |
| INT-04 server interaction | Wrong owner/target, range, LOS, replay/rate, cross-client dialogue and disconnect failures rejected in real two-client tests. | **NOT EXECUTED.** |
| Product continuity | AD-01 distinct characters, AD-02 Volta and vehicles deferred; no canonical-world or capacity claim. | **Recorded product direction; runtime gates separate.** |

The existing `scripts/mmo/s0_two_client_smoke.sh --plan` only validates executable paths and prints commands; `--run` checks liveness of the launched processes. **Neither mode validates actual network admission, controller/character binding, replication or interactive gameplay.** Preserve independent observed acceptance logs. The selected development map cannot be tested before an actual UE 5.8 host and asset exist.

**Do not invent or purchase hosting.** No dedicated engine machine is currently available to the owner. No public server exposure, GitHub Actions invocation, engine-source redistribution, cloud payment or local asset recovery is requested by this decision.

## 6. Decision and follow-up classification

**Founder-approved AD-03 design:** one **dedicated Unreal 5.8 server**, one **disposable `/Game/Dev/MMO_S0_TestZone`** (planned, not created), **two distinct clients**, **server-authoritative zone**, and **no cross-zone/production topology**. The founder approved this exact bounded design on 2026-09-25; the RM-02 register records that approval in draft PR #12. This is **not** approval of an engine host, spend, capacity, production network, security implementation, executable build or source PR merge.

**Still separate:** AD-04 game-owned persistence, AD-05 admission/reconnect, AD-06 dev map and canonical-world separation, AD-07 assets/license, PC-01 governance PR #15 (initial CI failure), full Unreal compilation, two-client runtime and all game PR merges. PRs #3–#12 and the INT-02 source integration stack remain drafts. No implementation claim is promoted by this architecture decision.
