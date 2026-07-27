---
id: TECH-004
title: Gameplay Data, Assets and Tags
status: proposed
---

# Gameplay Data, Assets and Tags

## Data-driven principle

Content variation belongs in authored definitions, not subclass proliferation.

Candidate C++ definition classes:

- `UAkbalAlebrijeDefinition`
- `UAkbalEncounterDefinition`
- `UAkbalSongDefinition`
- `UAkbalRitualChartDefinition`
- `UAkbalInstrumentDefinition`
- `UAkbalAbilityDefinition`
- `UAkbalAttackDefinition`
- `UAkbalZoneDefinition`

Use `UPrimaryDataAsset` for content needing stable IDs/Asset Manager control.

## Tags

Gameplay Tags represent cross-system semantics such as attack delivery/effects, path, archetype, states, and event categories.

Do not use free-form `FName` strings for canonical states when a typed enum or Gameplay Tag is more appropriate.
