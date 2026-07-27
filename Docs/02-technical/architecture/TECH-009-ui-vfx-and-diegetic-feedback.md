---
id: TECH-009
title: UI, VFX and Diegetic Feedback
status: proposed
---

# UI, VFX and Diegetic Feedback

## Shipping HUD

Only three primary bars:

- Health with corruption representation.
- Shamanic Charge.
- Alebrije Sync.

## Pattern state bridges

Gameplay exposes normalized/read-only state; materials/VFX decide presentation.

Alebrije pattern inputs may include:

- PreparedStunNormalized.
- RemainingGuaranteedStunNormalized.
- bInHeartbeatRecovery.
- bStunned.

Player pattern inputs may include:

- RitualAvailabilityState.
- ProgressionPatternLevel/collection.

Do not make material parameter state authoritative gameplay state.
