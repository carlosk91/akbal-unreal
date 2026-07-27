---
id: TECH-002
title: Audio Timing and Conductor
status: proposed
---

# Audio Timing and Conductor

## Candidate stack

- Quartz for authoritative musical scheduling/quantization.
- MetaSounds for adaptive/layered audio rendering.

This is a candidate architecture pending a UE 5.7.4 technical spike.

## Required conductor API concepts

- Start/stop encounter clock.
- Query musical position.
- Subscribe to beat/subdivision/bar/section.
- Convert between musical position and time where supported.
- Publish authored section boundaries.
- Provide stable timing to ritual judgment and attack scheduling.

## Pause

Pause behavior must be path-aware:

- Berserk boss encounter: game pause disabled.
- Soothing boss encounter: normal pause allowed; conductor/audio must resume without losing section state.

## Spike acceptance

Measure and document:

- Input judgment consistency at 30/60/120+ FPS.
- Quantized callback behavior.
- Audio latency configuration.
- MetaSound/Quartz transition behavior.
- Pause/resume behavior.
