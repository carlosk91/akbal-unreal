---
id: TECH-002
title: Audio Timing and Conductor
status: proposed
last_updated: 2026-08-10
implementation: partial
---

# Audio Timing and Conductor

## Candidate stack

- Quartz for authoritative musical scheduling/quantization.
- MetaSounds for adaptive/layered audio rendering.

This is a candidate architecture pending a UE 5.7.4 technical spike.

## F001 spike implementation (partial)

Delivered in C++ (`UAkbalMusicConductorSubsystem`):

| API concept | Status |
|---|---|
| Start/stop encounter clock | Implemented (`StartEncounter`, `StopEncounter`) |
| Query musical position | Implemented (`GetMusicalPosition`, `GetSecondsSinceTransportStart`) |
| Subscribe to beat | Implemented (`OnBeat` delegate, Quartz metronome) |
| Pause/resume transport | Implemented (`PauseEncounter`, `ResumeEncounter`) |
| Input latency offset | Implemented (`SetInputLatencyOffsetMs`) |
| Quartz round-trip latency introspection | Implemented (`GetDebugSnapshot`) |
| MetaSound adaptive stems | Not implemented (manual asset T006) |
| Path-aware pause (Berserk vs Soothing) | Not implemented |

Evidence: [`Docs/04-features/F001-rhythm-conductor/implementation-status.md`](../../04-features/F001-rhythm-conductor/implementation-status.md).

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

F001 spike: pauses conductor with game pause only (no path logic).

## Spike acceptance

Measure and document:

- Input judgment consistency at 30/60/120+ FPS.
- Quantized callback behavior.
- Audio latency configuration.
- MetaSound/Quartz transition behavior.
- Pause/resume behavior.

Automated judgment/FPS tests pass; manual rows in `F001/research.md` remain TBD.
