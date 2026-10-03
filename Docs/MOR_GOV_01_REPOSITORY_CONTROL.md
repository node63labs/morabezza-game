# MOR_GOV_01 — MORABEZZA Repository Admission Controls

**Control:** MOR_GOV_01
**Repository:** `node63labs/morabezza-game`
**Date:** 2026-10-03
**Entry main:** `af8f3d0e5f9291b1358232c7055190e4bd57b9f6`
**Governance authority:** `node63labs/node63-governance@06d4521482ca041bdb5eaff68b3d55872b033454`
**Status:** S1 SECURITY REMEDIATION + CI BOOTSTRAP CANDIDATE — MOR_GOV_01 CLOSURE NOT YET GRANTED

## Purpose

This record defines the generic repository-admission controls introduced for the public MORABEZZA game repository after the initial bootstrap correctly failed closed on a real committed Android File Server token.

These controls qualify repository changes. They do not qualify Unreal runtime behavior, multiplayer behavior, game assets, maps, Git LFS payloads, production infrastructure, player data, or product release readiness.

## Hosted baseline

The generic workflow is `.github/workflows/ci.yml` and runs for pull requests and pushes to `main` with read-only repository permissions.

It provides:

- exact-source checkout and SHA binding;
- `git diff --check`;
- syntax validation for tracked shell and Python source;
- JSON and `.uproject` parsing;
- repository-boundary checks;
- assertions that committed Android File Server access is disabled and the committed token is empty;
- Gitleaks v8.30.1 committed-history scanning using `.gitleaks.toml`.

The generic workflow deliberately does not:

- hydrate Git LFS;
- install or execute Unreal Engine;
- compile Editor, Game, or Server targets;
- launch a dedicated server or clients;
- create or mutate maps or assets;
- access production services or player data.

## Historical secret handling

Current source is hardened under MOR_GOV_01-S1. The historical Gitleaks exception is constrained to the `generic-api-key` rule, the single introducing commit recorded by governance, and the exact `Config/DefaultEngine.ini` path with `condition = "AND"`.

No blanket path, rule, repository, entropy, or scanner bypass is permitted. Any additional finding must still fail admission.

## Native protection target

MOR_GOV_01 is not closed merely because this workflow is merged. The public repository must also establish and verify native `main` protection or an equivalent repository ruleset requiring PR-based changes and the established hosted checks, with force-push and branch deletion blocked.

## Existing draft PR boundary

Open draft PRs #2 through #13 predate this admission baseline. This remediation does not authorize, rebase, retarget, merge, or accept any of them. Each retained work product must be reconciled separately against the resulting controlled `main` and re-qualified under applicable checks.

## Authority boundary

```text
MOR_GOV_01_S1=IN_PROGRESS
MOR_GOV_01_CI_BOOTSTRAP=IN_PROGRESS
MOR_GOV_01_CLOSED=NO
PR_2_TO_13_MERGE_AUTHORIZED=NO
UNREAL_RUNTIME_ACCEPTED=NO
GIT_LFS_HYDRATION_AUTHORIZED=NO
GAME_MAP_MUTATION_AUTHORIZED=NO
GAME_ASSET_MUTATION_AUTHORIZED=NO
DEDICATED_SERVER_EXECUTION_AUTHORIZED=NO
PLAYER_DATA_MUTATION_AUTHORIZED=NO
PRODUCTION_RELEASE_AUTHORIZED=NO
COMMERCIAL_LAUNCH_AUTHORIZED=NO
N4_COHORT_CHANGED=NO
```
