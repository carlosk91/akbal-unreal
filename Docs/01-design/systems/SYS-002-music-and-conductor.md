---
id: SYS-002
title: Music and Conductor
status: approved
last_updated: 2026-08-10
---

# Music and Conductor

## Intent

Music is an authoritative gameplay structure, not background decoration.

## F001 spike alignment

Phase 1 delivers a Quartz-backed prototype conductor (`UAkbalMusicConductorSubsystem`) and chart-driven ritual input session. This validates musical-time judgment but does not implement full encounter arrangements, stem mixing, or section authoring. See [`F001 implementation-status`](../../04-features/F001-rhythm-conductor/implementation-status.md).

## Requirements

- One encounter uses one musical composition identity.
- Variations may add/remove stems, channels, processing, layers, or arrangement complexity.
- Boss genre identity informs attack choreography, movement cadence, ritual vocabulary, and visual rhythm.
- Gameplay events may be authored against beats, subdivisions, bars, and sections.
- Rhythm judgment must reference authoritative musical time.

## Chart fidelity

`ChartFidelity` represents how closely required player inputs follow audible musical events.

Low difficulty may ask only for structural beats. High difficulty may encode accents, subdivisions, silences, channel changes, and dense musical detail.

## Charge normalization

For section `S`:

```text
N = number of required player inputs in S
NoteWeight = C / N
```

The goal is for perfect execution of a baseline section to yield approximately 1.0 Shamanic Charge independent of note density.

`C` and accuracy multipliers remain tuning data.
