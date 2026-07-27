---
id: TECH-010
title: Testing, Debugging and Telemetry
status: proposed
---

# Testing, Debugging and Telemetry

## Automated targets

Unit/Automation coverage should prioritize:

- Sync passive decay and cross-section error regression.
- Soothing anchor behavior.
- Berserk Perfect-chain behavior.
- Stun counter conversion.
- Heartbeat increasing probability with seeded rolls.
- Section completion ending stun.
- Re-entry not refreshing stun.
- Corruption/effective health failure.
- Shamanic Charge normalization.
- MissStreak / Ritual Composure formula.
- Ending qualification.

## Functional maps

Create focused test maps rather than requiring the full game world to validate systems.

Suggested:

- `L_Test_Ritual`
- `L_Test_StunHeartbeat`
- `L_Test_AttackInterrupt`
- `L_Test_AudioTiming`

## Debug overlay

Developer-only overlay should expose hidden values and musical timeline.
