# MOR_GOV_01-S1 — Android File Server Security Remediation

**Parent control:** MOR_GOV_01
**Date:** 2026-10-03
**Governance authority:** `node63labs/node63-governance@06d4521482ca041bdb5eaff68b3d55872b033454`
**Status:** REMEDIATION CANDIDATE — HOSTED QUALIFICATION REQUIRED

## Finding

MOR_GOV_01's first hosted secret scan identified one real historical/current committed token in `Config/DefaultEngine.ini`, associated with Android File Server runtime settings. The exposed value is intentionally not reproduced here.

Safe identifying metadata:

```text
RULE=generic-api-key
FILE=Config/DefaultEngine.ini
INTRODUCING_COMMIT=b9e00c4599ee4ebad4c6f4255761feb80b21d350
```

## Current-source remediation

The committed project configuration now sets:

```text
bEnablePlugin=False
bAllowNetworkConnection=False
SecurityToken=<empty>
```

No replacement secret is committed.

This change is bounded security hardening. It does not establish Unreal/Android runtime acceptance and does not authorize Android File Server use.

## Historical scan disposition

The public Git history is not rewritten. `.gitleaks.toml` extends the default Gitleaks rules and narrows the historical exception by all of:

- rule `generic-api-key`;
- introducing commit `b9e00c4599ee4ebad4c6f4255761feb80b21d350`;
- path `Config/DefaultEngine.ini`;
- AND matching semantics.

Any unrelated or new finding must continue to fail the secret-scan job.

## Explicit boundaries

```text
REPLACEMENT_SECRET_COMMITTED=NO
HISTORY_REWRITE=NO
BLANKET_SECRET_SCAN_BYPASS=NO
UNREAL_ENGINE_EXECUTION_AUTHORIZED=NO
ANDROID_RUNTIME_ACCEPTED=NO
GIT_LFS_HYDRATION_AUTHORIZED=NO
GAME_MAP_MUTATION_AUTHORIZED=NO
GAME_ASSET_MUTATION_AUTHORIZED=NO
DEDICATED_SERVER_EXECUTION_AUTHORIZED=NO
PLAYER_DATA_MUTATION_AUTHORIZED=NO
PRODUCTION_RELEASE_AUTHORIZED=NO
PR_2_TO_13_MERGE_AUTHORIZED=NO
```

The remediation becomes integrated only after fresh exact-head hosted checks and successful post-merge `main` checks. MOR_GOV_01 remains open until native `main` protection is also verified.
