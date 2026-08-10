---
id: F001-STATUS
title: F001 Rhythm Conductor — Implementation Status
status: working
last_updated: 2026-08-10
branch: tech/phase-1-rhythm-conductor
---

# F001 Rhythm Conductor — What Was Built

This document is the **zero-context entry point** for Phase 0 (repository bootstrap) and Phase 1 (timing spike) work delivered on branch `tech/phase-1-rhythm-conductor`. Read it before touching timing, ritual input, or spike PIE setup.

Governing specs: [`spec.md`](spec.md), [`plan.md`](plan.md), [`tasks.md`](tasks.md), [`test-plan.md`](test-plan.md), [`research.md`](research.md).

---

## Summary

| Area | Status |
|---|---|
| Phase 0 — repo bootstrap | **Complete** |
| Phase 1 — F001 C++ spike | **Implemented** (automated tests pass) |
| Phase 1 — manual spike validation | **Pending** (`research.md` tables, T006–T007) |
| ADR-0005 (Quartz direction) | **Proposed** — not promoted until manual evidence |

The project now has a **working Quartz-backed musical clock**, **deterministic rhythm judgment math**, a **prototype ritual input chart**, a **production-shaped lane HUD**, and a **developer harness** for tuning. This is spike/dev tooling, not shipping encounter flow.

---

## How to run the spike (PIE)

1. Open `Akbal.uproject` in UE **5.7.4**.
2. Build `AkbalEditor` (see [`Scripts/README.md`](../../../Scripts/README.md)).
3. Press **Play** — `Config/DefaultEngine.ini` sets `GlobalDefaultGameMode` to `AAkbalRhythmSpikeGameMode` for spike validation.
4. Use any editor map (default: Open World template).

### Controls

| Input | Action |
|---|---|
| Arrow keys | Lane input (Left / Right / Up / Down) |
| Q / E | Previous / next active instrument |
| P | Toggle game pause (conductor pause/resume) |
| R | Restart chart / encounter |

### On-screen UI

- **Top-left — `UAkbalRhythmHarnessWidget`**: BPM ±5, instruments 1–4, note approach ±0.25s, preview window ±0.25s, latency ±10ms, Pause, Restart.
- **Centered on player — `UAkbalRitualInputHud`**: square lane overlay, sync center, approaching notes, ghost notes on inactive instruments.
- **Green debug line** (brief): `Akbal UI | HUD:… Harness:…` from player controller startup.

Filter Output Log by **`LogAkbalRitualUi`** for widget creation and layout diagnostics.

---

## Source code map

All code lives in the single `Akbal` runtime module.

```text
Source/Akbal/
├── Public/Audio/
│   ├── AkbalMusicConductorSubsystem.h   # Quartz encounter clock (UWorldSubsystem)
│   ├── AkbalMusicalPosition.h           # Bar/beat/seconds snapshot
│   └── AkbalMetronomeSoundFactory.h     # Procedural beat click (optional)
├── Public/Input/
│   ├── AkbalRhythmJudgmentTypes.h       # EAkbalRhythmJudgment, timing windows
│   ├── AkbalRhythmJudgmentEvaluator.h   # Pure judgment math
│   ├── AkbalRhythmChartTypes.h          # Lanes, instruments, chart notes
│   ├── AkbalRhythmChartEvaluator.h      # Chart matching, preview visibility
│   └── AkbalRitualInputTypes.h          # FAkbalRitualInputHudConfig
├── Public/UI/Ritual/
│   ├── AkbalRitualInputSession.h        # Chart state + lane input processing
│   ├── AkbalRitualInputHud.h            # Production-shaped paint-only HUD
│   ├── AkbalRitualViewportLayout.h    # Square HUD viewport math
│   └── AkbalRitualUiLog.h               # LogAkbalRitualUi category
├── Public/UI/Dev/
│   └── AkbalRhythmHarnessWidget.h       # F001 dev harness (not shipping UI)
├── Public/Spike/
│   ├── AkbalRhythmSpikeGameMode.h       # Default spike game mode
│   ├── AkbalRhythmSpikePlayerController.h
│   ├── AkbalRhythmSpikeCharacter.h      # Third-person pawn for HUD projection
│   └── AkbalRhythmSpikeActor.h          # Legacy optional debug actor
└── Private/Tests/
    ├── AkbalRhythmJudgmentTests.cpp     # Akbal.Rhythm.Judgment*
    ├── AkbalRhythmChartTests.cpp        # Akbal.Rhythm.Chart*
    └── AkbalRitualUiTests.cpp           # Akbal.UI.*
```

### Deprecated / legacy

- `Source/Akbal/Public/UI/Spike/AkbalRhythmSpikeWidget.h` — monolithic early spike widget, **superseded** by `UAkbalRitualInputHud` + `UAkbalRhythmHarnessWidget`. Not used by the current spike player controller. Safe to delete in a follow-up cleanup PR.

---

## Architecture (runtime)

```
UAkbalMusicConductorSubsystem (Quartz clock)
    ↑ reads transport / beat callbacks
    │
UAkbalRitualInputSession (chart + config + ProcessLaneInput)
    ↑ binds
    ├── UAkbalRitualInputHud        (paint-only, follows player screen position)
    └── UAkbalRhythmHarnessWidget   (dev controls, top-left viewport overlay)

AAkbalRhythmSpikePlayerController
    └── owns session + both widgets; spike PIE entry point
```

**Timing invariant:** judgment uses `GetSecondsSinceTransportStart()` at input time — never frame delta or widget paint timing.

---

## Prototype defaults (`FAkbalRitualInputHudConfig`)

| Field | Default | Notes |
|---|---|---|
| `BeatsPerMinute` | 80 | Lowered from early 120 for playability |
| `BeatsPerBar` | 4 | |
| `AvailableInstrumentCount` | 4 | Harness can set 1–4 |
| `NoteApproachSeconds` | 1.0 | Edge-to-center travel time |
| `NotePreviewSeconds` | 1.0 | Only notes hitting within this window are drawn |
| `MaxViewportSizeFraction` | **0.28** | Square HUD max size (28% of smaller viewport dimension) |

Judgment windows (`FAkbalRhythmTimingWindows`): Perfect ±25 ms, Good ±50 ms, Pass ±80 ms (prototype — `TBD-001`).

---

## Automation tests

Run from repository root (editor **closed**, or Live Coding will block CLI build):

```powershell
.\Scripts\RunAutomationTests.ps1 -Filter "Akbal" -BuildFirst
```

| Test name | Covers |
|---|---|
| `Akbal.Rhythm.JudgmentTiers` | Perfect/Good/Pass/Miss boundaries |
| `Akbal.Rhythm.NearestBeatTarget` | Nearest-beat selection |
| `Akbal.Rhythm.FrameRateIndependence` | Same timestamp → same tier at 30/60/120 FPS simulation |
| `Akbal.Rhythm.MusicalPositionRoundTrip` | Position ↔ seconds at 120 BPM |
| `Akbal.Rhythm.LaneArrowKeyMapping` | Arrow → lane mapping |
| `Akbal.Rhythm.ChartUsesAllInstruments` | Spike chart uses all four instruments |
| `Akbal.Rhythm.ChartLaneInstrumentJudgment` | Lane + instrument matching |
| `Akbal.Rhythm.InstrumentLibrary` | Instrument index/name round-trip |
| `Akbal.Rhythm.ChartNoteCount` | ≥36 required notes |
| `Akbal.Rhythm.ChartForcesInstrumentChanges` | Chart alternates instruments/lanes |
| `Akbal.Rhythm.InstrumentSwitchPreservesChart` | Q/E does not mutate chart state |
| `Akbal.Rhythm.ChartInstrumentCount` | 1–4 instrument filter |
| `Akbal.Rhythm.NotePreviewVisibility` | Preview window visibility rules |
| `Akbal.UI.ViewportLayout` | Square HUD size math |
| `Akbal.UI.RitualHudConfigDefaults` | Config defaults |
| `Akbal.UI.Harness.BuildsWidgetTree` | Harness widget tree builds |
| `Akbal.UI.RitualHud.CreatesAndBinds` | HUD creates and binds session |

---

## Config changes worth knowing

`Config/DefaultEngine.ini`:

- `GlobalDefaultGameMode=/Script/Akbal.AkbalRhythmSpikeGameMode` — **spike-only**; revert or override when leaving F001.

Manual Editor assets still **not created** (T006):

- `Content/_Akbal/Maps/L_Test_AudioTiming`
- `Content/_Akbal/Music/MetaSounds/MS_Spike_Click`

---

## What is explicitly not built

- Full encounter state machine, boss AI, or production ritual sync (F003).
- Player resources, stun, corruption (F002+).
- Enhanced Input action assets (spike uses direct key binds on player controller).
- Shipping MetaSound content pipeline.
- Path-aware pause rules (Berserk vs Soothing) — spike pauses conductor with game pause only.

---

## Remaining work before closing F001

- [ ] **T006** — Create `L_Test_AudioTiming` and `MS_Spike_Click` in Editor.
- [ ] **T007** — Fill `research.md` measurement tables (30/60/120 FPS, latency, pause drift, MetaSound alignment).
- [ ] **T008** — Promote ADR-0005 to Accepted only after T007 evidence.
- [ ] Remove legacy `UAkbalRhythmSpikeWidget` when convenient.
- [ ] Revert `GlobalDefaultGameMode` to a neutral default when spike is no longer the default PIE experience.

---

## Related documents

- Design: [`SYS-002`](../../01-design/systems/SYS-002-music-and-conductor.md)
- Technical: [`TECH-002`](../../02-technical/architecture/TECH-002-audio-timing-and-conductor.md), [`TECH-005`](../../02-technical/architecture/TECH-005-input-and-rhythm-judgment.md)
- Decision: [`ADR-0005`](../../05-decisions/ADR-0005-quartz-metasounds-spike.md)
- Roadmap: [`INITIAL_IMPLEMENTATION_PLAN.md`](../../07-roadmap/INITIAL_IMPLEMENTATION_PLAN.md)
