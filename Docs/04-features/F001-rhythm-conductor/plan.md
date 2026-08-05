# Implementation Plan — F001 Rhythm Conductor (Phase 1 Spike)

## Governing specs

- SYS-002 Music and Conductor
- TECH-002 Audio Timing and Conductor
- TECH-005 Input and Rhythm Judgment
- ADR-0005 Quartz + MetaSounds Timing Direction

## Architecture

```
UAkbalMusicConductorSubsystem (UWorldSubsystem)
  └── wraps UQuartzSubsystem / UQuartzClockHandle

FAkbalRhythmJudgmentEvaluator (pure static math, no UObject)
  └── used by conductor + automation tests

AAkbalRhythmSpikeActor (dev-only test harness)
  └── input capture, debug HUD, manual validation
```

## Runtime ownership

- `UAkbalMusicConductorSubsystem` owns the encounter Quartz clock handle and beat subscription.
- Spike actor is ephemeral dev tooling; not part of shipping encounter flow.

## C++ responsibilities

- Quartz clock lifecycle (create, start, stop, pause, resume).
- Musical position query and seconds-since-transport.
- Input timestamp judgment against beat targets.
- Debug snapshot struct for overlay rendering.
- Automation tests for judgment math and FPS simulation.
- Optional MetaSound playback hook on the Quartz clock.

## Blueprint responsibilities

- Author `MS_` test click/pulse MetaSound in Editor for manual MetaSound validation.
- Create `L_Test_AudioTiming` map and assign `BP` or C++ game mode override.

## Data assets / tags

None for Phase 1. Prototype BPM and timing windows are C++ defaults exposed as editable properties on the spike actor.

## Public interfaces

| Type | Purpose |
|---|---|
| `UAkbalMusicConductorSubsystem` | Authoritative encounter clock API |
| `FAkbalMusicalPosition` | Bar/beat/seconds snapshot |
| `EAkbalRhythmJudgment` | Judgment tier enum |
| `FAkbalRhythmTimingWindows` | Window config in milliseconds |
| `UAkbalRhythmJudgmentEvaluator` | Static judgment functions |
| `AAkbalRhythmSpikeActor` | Dev spike harness |

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

- On-screen debug strings via `AddOnScreenDebugMessage`.
- `akbal.rhythmspike.*` console variables.
- Latency fields from `UQuartzSubsystem` round-trip APIs.

## Testing strategy

- Unit: judgment evaluator tiers, beat-to-seconds conversion, simulated FPS input capture.
- Manual: `L_Test_AudioTiming` with 30/60/120 FPS cap, pause key, MetaSound click on beat.

## Binary assets expected to change

- `Content/_Akbal/Maps/L_Test_AudioTiming` (manual Editor creation)
- `Content/_Akbal/Music/MetaSounds/MS_Spike_Click` (manual Editor creation)

## Risks / rollback

- Quartz latency may exceed prototype windows at default audio buffer size — document, do not retune permanently.
- If Quartz fails spike acceptance, ADR-0005 remains Proposed and alternative clock research is required.
