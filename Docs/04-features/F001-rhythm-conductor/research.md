# Research / Spike Notes — F001 Rhythm Conductor

## Question

Can UE 5.7.4 Quartz (+ MetaSounds) provide a sample-stable authoritative musical clock for Akbal rhythm judgment, including acceptable latency, quantized callbacks, and pause/resume behavior at 30/60/120+ FPS?

## Evidence / references

- `UQuartzSubsystem`, `UQuartzClockHandle` (AudioMixer module)
- `FQuartzTransportTimeStamp` for bar/beat position
- `UQuartzSubsystem::GetRoundTripAverageLatency` and related APIs
- ADR-0005 (Proposed pending this spike)

## Options considered

### Option A — Quartz clock as authoritative conductor

Pros: Engine-native musical scheduling, beat subscriptions, latency introspection.
Cons: Audio-thread latency must be measured; pause requires explicit clock pause.

### Option B — Frame-accumulated game timer

Pros: Simple.
Cons: Violates SYS-002/TECH-005; frame-rate dependent. Rejected.

## Prototype measurements

| Metric | 30 FPS | 60 FPS | 120 FPS | Notes |
|---|---|---|---|---|
| Judgment tier (same intended tap) | TBD | TBD | TBD | Automated unit tests pass; manual PIE pending |
| Round-trip latency (ms) | TBD | TBD | TBD | Read from conductor debug snapshot |
| Pause position drift (beats) | TBD | — | — | After 5s pause |
| MetaSound click alignment (ms) | TBD | TBD | TBD | Requires `MS_Spike_Click` asset |

### Setup

1. Build Editor target.
2. Open any map (or create `Content/_Akbal/Maps/L_Test_AudioTiming`).
3. Set Game Mode Override to `AkbalRhythmSpikeGameMode`.
4. PIE third-person; use **arrow keys** for lanes, **Q/E** for instrument, **P** pause, **R** restart chart.
5. Use the top-left **Rhythm Harness** panel for BPM, instrument count (1–4), note speed, preview window, and latency.

### Engine APIs/assets used

- `UAkbalMusicConductorSubsystem`
- `UAkbalRitualInputSession` / `UAkbalRitualInputHud` (production-shaped ritual input overlay)
- `UAkbalRhythmHarnessWidget` (F001 dev controls)
- `AAkbalRhythmSpikePlayerController`
- Optional: `MS_Spike_Click` MetaSound assigned on spike actor

### Measurement method

- Read on-screen debug overlay for position, judgment, delta ms, latency.
- Compare tap judgments across `t.MaxFPS 30`, `60`, `120`.
- Record Quartz round-trip latency from debug snapshot before/after buffer size changes.

### Audio/input latency assumptions

- Default project buffer: 1024 frames @ 48 kHz (~21.3 ms) per `DefaultEngine.ini`.
- Input timestamp captured at key-press on game thread referencing conductor seconds, not frame index.

### Pause/resume behavior

- Conductor calls `PauseClock` / `ResumeClock` when game pause toggles.
- Transport position should not reset on resume (validate manually).

### Failure cases

- Tap before `StartEncounter`: `Miss`, logged.
- Double clock start: ignored if already running.

## Recommendation

Pending manual measurements (T006–T007). Automated judgment math and FPS timestamp simulation pass in CI.

## Decision status

`research only` — promote to ADR-0005 Accepted only after manual latency/pause/MetaSound rows are filled.
