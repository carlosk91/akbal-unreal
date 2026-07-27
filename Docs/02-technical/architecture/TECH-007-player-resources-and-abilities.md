---
id: TECH-007
title: Player Resources and Abilities
status: proposed
---

# Player Resources and Abilities

## Health model

Track separately:

- Base maximum health.
- Current health.
- Corruption amount/effective maximum.

Do not encode corruption by permanently mutating base max health.

## Shamanic Charge

Continuous numeric resource with max capacity, spend events, note gains, and checkpoint refill.

Perfect-section baseline target: ~1.0 charge before gain upgrades.

## Ability model

Ritual active set is fixed. Non-ritual active set is separately fixed. Data definitions should control costs/tuning but should not imply player loadout selection where design disallows it.

## Path ability

Persist path choice in save state. Unselected Berserk/Soothing ability is unavailable for that playthrough.
