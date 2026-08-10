# ADR-0005 — Quartz + MetaSounds Timing Direction

**Status:** Proposed / Requires Spike  
**Last updated:** 2026-08-10

## Context

Akbal requires an authoritative musical clock for rhythm judgment, attack scheduling, and adaptive audio. Frame-based timers are insufficient.

## Decision (candidate)

Quartz is the preferred candidate for sample-accurate musical scheduling and MetaSounds for adaptive audio.

## Spike progress (F001, branch `tech/phase-1-rhythm-conductor`)

| Criterion | Evidence |
|---|---|
| Quartz clock lifecycle | `UAkbalMusicConductorSubsystem` — start/stop/pause/resume implemented |
| Musical position query | `FAkbalMusicalPosition`, `GetSecondsSinceTransportStart` |
| Beat subscription | `OnBeat` via Quartz metronome |
| Judgment vs conductor time | `FAkbalRhythmJudgmentEvaluator`; 13 `Akbal.Rhythm.*` tests pass |
| Frame-rate independence | `Akbal.Rhythm.FrameRateIndependence` automation test |
| Pause/resume transport | Implemented; manual drift measurement **TBD** in `research.md` |
| MetaSound validation | **TBD** — `MS_Spike_Click` asset not created (T006) |
| Audio buffer / round-trip latency | **TBD** — manual table in `research.md` |

Full implementation guide: [`Docs/04-features/F001-rhythm-conductor/implementation-status.md`](../04-features/F001-rhythm-conductor/implementation-status.md).

## Promotion criteria

Promote to **Accepted** only when:

1. `F001/research.md` manual measurement tables are filled (T007).
2. Pause/resume drift is within acceptable prototype bounds.
3. MetaSound quantized click alignment is documented (or explicitly deferred with rationale).

Until then, treat Quartz as **validated for judgment math and clock API**, not as fully proven for shipping audio latency.

## Consequences

- Gameplay timing code should use `UAkbalMusicConductorSubsystem` (or its successor), not `GetWorld()->GetTimeSeconds()` for rhythm judgment.
- If spike fails acceptance, revisit this ADR before F003 ritual sync.
