# Implementation Plan — F001 Rhythm Conductor (Phase 1 Spike)

## Governing specs

- SYS-002 Music and Conductor
- SYS-004 Ritual Input (prototype HUD alignment)
- SYS-007 Channel Archetypes (instrument display names)
- TECH-002 Audio Timing and Conductor
- TECH-005 Input and Rhythm Judgment
- ADR-0005 Quartz + MetaSounds Timing Direction

## Architecture

```
UAkbalMusicConductorSubsystem (UWorldSubsystem)
  └── wraps UQuartzSubsystem / UQuartzClockHandle

FAkbalRhythmJudgmentEvaluator (pure static math, no UObject)
  └── used by conductor + automation tests

FAkbalRhythmChartEvaluator / FAkbalRhythmChartBuilder
  └── spike test chart, lane/instrument judgment, note preview window

UAkbalRitualInputSession (chart + config + input processing)
  └── production-facing ritual input state; consumed by HUD and harness

UAkbalRitualInputHud (production ritual input overlay)
  └── square lane HUD, ghost notes, configurable preview window

UAkbalRhythmHarnessWidget (F001 dev harness only)
  └── pause/restart, latency, BPM, instrument count, approach/preview tuning

AAkbalRhythmSpikePlayerController (F001 spike game mode)
  └── owns session + HUD + harness; projects HUD to player screen position

AAkbalRhythmSpikeActor (legacy debug harness)
  └── optional on-screen debug overlay; forwards lane input to session when present
```

## Runtime ownership

- `UAkbalMusicConductorSubsystem` owns the encounter Quartz clock handle and beat subscription.
- `UAkbalRitualInputSession` owns spike chart state, active instrument, and `FAkbalRitualInputHudConfig`.
- `UAkbalRitualInputHud` is paint-only and reads session + conductor snapshots each frame.
- `UAkbalRhythmHarnessWidget` is dev-only; mutates session config and conductor transport.
- Spike actor and player controller are ephemeral dev tooling; not part of shipping encounter flow.

## C++ responsibilities

- Quartz clock lifecycle (create, start, stop, pause, resume).
- Musical position query and seconds-since-transport.
- Input timestamp judgment against beat/chart targets.
- Ritual input HUD layout: square (max 20% of smaller viewport dimension), centered on player via world-to-screen projection.
- Note preview window (`NotePreviewSeconds`, default 1.0s) and approach travel (`NoteApproachSeconds`).
- Automation tests for judgment math, chart filtering, and preview visibility.
- Optional MetaSound / procedural metronome playback on beat.

## Blueprint responsibilities

- Author `MS_` test click/pulse MetaSound in Editor for manual MetaSound validation.
- Create `L_Test_AudioTiming` map and assign spike game mode override.

## Data assets / tags

None for Phase 1. Prototype BPM (default 80), timing windows, HUD sizing, preview, and approach values are exposed via `FAkbalRitualInputHudConfig` on the spike player controller and harness.

## Public interfaces

| Type | Purpose |
|---|---|
| `UAkbalMusicConductorSubsystem` | Authoritative encounter clock API |
| `FAkbalMusicalPosition` | Bar/beat/seconds snapshot |
| `EAkbalRhythmJudgment` | Judgment tier enum |
| `FAkbalRhythmTimingWindows` | Window config in milliseconds |
| `FAkbalRhythmJudgmentEvaluator` | Static judgment functions |
| `FAkbalRitualInputHudConfig` | Ritual HUD/session tuning (BPM, instruments, preview, approach, size) |
| `UAkbalRitualInputSession` | Chart state, lane input, instrument cycling |
| `UAkbalRitualInputHud` | Production ritual input overlay (lanes, notes, sync center) |
| `UAkbalRhythmHarnessWidget` | F001 dev harness controls |
| `AAkbalRhythmSpikePlayerController` | Spike PIE entry point |
| `AAkbalRhythmSpikeActor` | Legacy debug harness |

## State transitions

```
Idle → Running (StartEncounter)
Running → Paused (PauseEncounter / game pause)
Paused → Running (ResumeEncounter / game unpause)
Running → Idle (StopEncounter)
```

## Timing/threading

- Quartz callbacks arrive on game thread via `FOnQuartzMetronomeEventBP`.
- Input timestamps read synchronously on game thread at key press.
- Judgment math is synchronous and deterministic.

## Save impact

None.

## Debug tooling

- `UAkbalRhythmHarnessWidget`: BPM ±5, instruments 1–4, approach/preview ±0.25s, latency ±10ms, pause/restart.
- Optional on-screen debug strings via spike actor or player controller.
- `akbal.rhythmspike.*` console variables.
- Latency fields from `UQuartzSubsystem` round-trip APIs.

## Testing strategy

- Unit: judgment evaluator tiers, beat-to-seconds conversion, simulated FPS input capture, chart instrument filtering, note preview visibility.
- Manual: spike game mode at 80 BPM default; verify square HUD on player, 1s preview window, harness controls, ghost notes on inactive instruments.

## Binary assets expected to change

- `Content/_Akbal/Maps/L_Test_AudioTiming` (manual Editor creation)
- `Content/_Akbal/Music/MetaSounds/MS_Spike_Click` (manual Editor creation)

## Risks / rollback

- Quartz latency may exceed prototype windows at default audio buffer size — document, do not retune permanently.
- If Quartz fails spike acceptance, ADR-0005 remains Proposed and alternative clock research is required.
