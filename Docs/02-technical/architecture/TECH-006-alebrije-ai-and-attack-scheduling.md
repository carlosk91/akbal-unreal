---
id: TECH-006
title: Alebrije AI and Attack Scheduling
status: proposed
---

# Alebrije AI and Attack Scheduling

## Separation

- AI/behavior chooses intent/pattern according to encounter state.
- Attack timeline defines musically scheduled attack events.
- Attack definition defines semantic effects.

## Attack semantic flags/data

Prefer composable properties/tags over `if Ranged else Physical` hardcoding.

Examples:

- `Attack.Delivery.Physical`
- `Attack.Delivery.Ranged`
- `Attack.Effect.HealthDamage`
- `Attack.Effect.Corruption`
- `Attack.Effect.Knockback`
- `Attack.Effect.RitualInterrupt`

Project defaults:

- Physical attacks: interrupt ritual when they hit.
- Ranged attacks: do not interrupt ritual by default.

Content can opt into corruption/effects explicitly.

## Music scheduling

Attack launch/telegraph/impact can be authored to musical positions. Visual telegraphs must account for travel/animation time instead of launching everything directly on the intended impact beat.
