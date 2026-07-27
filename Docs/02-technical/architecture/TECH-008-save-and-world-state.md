---
id: TECH-008
title: Save and World State
status: proposed
---

# Save and World State

## Durable IDs

Use stable content IDs for restored alebrijes, bosses, checkpoints, gates, upgrades, and bestiary entries. Do not persist raw Actor pointers or map-instance references.

## Save categories

- Player progression.
- Instrument unlocks.
- Power-up acquisition/upgrades.
- Path choice.
- Restored alebrijes/bosses.
- Zone/music restoration state.
- Shortcuts/gates.
- Current checkpoint.
- Ending qualification flags per required boss.

## Respawn

Encounter-local Sync/stun/Composure resets after failure. Durable world restoration remains.

## Versioning

Add save versioning before public playtest builds. New fields must have safe defaults.
