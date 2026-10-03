# MOR_GOV_01-S1 — AFS Security Token Remediation

**Control:** MOR_GOV_01-S1
**Repository:** `node63labs/morabezza-game`
**Date:** 2026-10-03
**Governance authority:** `node63labs/node63-governance@06d4521482ca041bdb5eaff68b3d55872b033454`
**Entry game main:** `af8f3d0e5f9291b1358232c7055190e4bd57b9f6`
**Status:** IMPLEMENTED CANDIDATE — HOSTED QUALIFICATION REQUIRED

## Trigger

The first MOR_GOV_01 CI bootstrap failed closed because Gitleaks v8.30.1 identified one real historical/current `generic-api-key` finding in `Config/DefaultEngine.ini`. The secret value is intentionally not reproduced here.

Safe finding metadata retained for control purposes:

```text
RULE=generic-api-key
FILE=Config/DefaultEngine.ini
INTRODUCING_COMMIT=b9e00c4599ee4ebad4c6f4255761feb80b21d350
LINE=28
FINDING_COUNT=1
```

## Current-source remediation

The committed Android File Server path is neutralized as follows:

```text
bEnablePlugin=False
bAllowNetworkConnection=False
SecurityToken=
```

No replacement token is committed. Other AFS settings are left unchanged.

This is a bounded security hardening change. It is not Unreal runtime validation, Android packaging validation, or a product-platform acceptance decision.

## Historical finding policy

Repository history is not rewritten. `.gitleaks.toml` extends the default Gitleaks rule set and permits only the known historical finding by conjunction of:

```text
TARGET_RULE=generic-api-key
INTRODUCING_COMMIT=b9e00c4599ee4ebad4c6f4255761feb80b21d350
PATH=^Config/DefaultEngine\.ini$
MATCH_CONDITION=AND
```

Any different rule, commit, or path remains subject to normal detection and must fail the scan.

## Qualification requirements

Before merge:

- exact-head Repository Validation must succeed;
- exact-head Secret Scan using Gitleaks v8.30.1 must succeed;
- the PR head must remain unchanged after qualification.

After merge:

- `main` Repository Validation must succeed;
- `main` Secret Scan must succeed;
- native `main` protection or an equivalent ruleset must then be established and independently verified before MOR_GOV_01 may close.

## Explicit boundaries

```text
SECRET_REPLACEMENT_IN_GIT_AUTHORIZED=NO
GITLEAKS_BLANKET_ALLOWLIST_AUTHORIZED=NO
GIT_HISTORY_REWRITE_AUTHORIZED=NO
UNREAL_ENGINE_EXECUTION_AUTHORIZED=NO
ANDROID_RUNTIME_ACCEPTANCE=NO
GIT_LFS_HYDRATION_AUTHORIZED=NO
GAME_MAP_MUTATION_AUTHORIZED=NO
GAME_ASSET_MUTATION_AUTHORIZED=NO
DEDICATED_SERVER_EXECUTION_AUTHORIZED=NO
PLAYER_DATA_MUTATION_AUTHORIZED=NO
PRODUCTION_RELEASE_AUTHORIZED=NO
COMMERCIAL_LAUNCH_AUTHORIZED=NO
MORABEZZA_PR_2_TO_13_MERGE_AUTHORIZED=NO
N4_COHORT_CHANGED=NO
MOR_GOV_01_CLOSED=NO
```
