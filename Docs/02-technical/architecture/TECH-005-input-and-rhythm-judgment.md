---
id: TECH-005
title: Input and Rhythm Judgment
status: proposed
last_updated: 2026-08-10
implementation: partial
---

# Input and Rhythm Judgment

## Input

Use Enhanced Input. Keep input mapping separate from judgment math.

**F001 spike note:** spike PIE uses direct key binds on `AAkbalRhythmSpikePlayerController` (arrow keys, Q/E, P, R) for rapid validation. Enhanced Input assets are not yet authored.

## Judgment

A note judgment consumes:

- Required musical timestamp/position.
- Actual player input timestamp referenced to the conductor clock.
- Allowed timing windows after active modifiers.

Outputs should be strongly typed, e.g. `Perfect`, `Good`, `Pass`, `Miss`.

Berserk accepts only `Perfect` for chain continuation; normal ritual may accept other passing judgments.

## F001 spike implementation (partial)

| Component | Role |
|---|---|
| `EAkbalRhythmJudgment` | Judgment tier enum |
| `FAkbalRhythmTimingWindows` | Prototype windows: Perfect ±25 ms, Good ±50 ms, Pass ±80 ms |
| `FAkbalRhythmJudgmentEvaluator` | Pure static beat judgment math |
| `FAkbalRhythmChartEvaluator` | Chart note matching, preview visibility, auto-miss |
| `FAkbalRhythmChartBuilder` | Prototype spike chart (lanes + instruments) |
| `UAkbalRitualInputSession` | Session state, `ProcessLaneInput`, instrument cycling |

Judgment references `UAkbalMusicConductorSubsystem::GetSecondsSinceTransportStart()` at input time — not frame delta or widget timing.

Automation: `Akbal.Rhythm.JudgmentTiers`, `Akbal.Rhythm.FrameRateIndependence`, `Akbal.Rhythm.ChartLaneInstrumentJudgment`, etc.

## Chart model

Required inputs are authored gameplay events, not every sound event in the source music. Chart fidelity can increase across sections/mastery versions.

## Testability

Judgment math must be testable without rendering UI/audio.

Implemented: 13 `Akbal.Rhythm.*` automation tests + 4 `Akbal.UI.*` widget/layout tests.
