---
id: TECH-010
title: Testing, Debugging and Telemetry
status: proposed
last_updated: 2026-08-10
---

# Testing, Debugging and Telemetry

## Automated targets

Unit/Automation coverage should prioritize:

- Sync passive decay and cross-section error regression.
- Soothing anchor behavior.
- Berserk Perfect-chain behavior.
- Stun counter conversion.
- Heartbeat increasing probability with seeded rolls.
- Section completion ending stun.
- Re-entry not refreshing stun.
- Corruption/effective health failure.
- Shamanic Charge normalization.
- MissStreak / Ritual Composure formula.
- Ending qualification.

### F001 implemented (2026-08-10)

| Group | Tests | Location |
|---|---|---|
| `Akbal.Rhythm.*` | 13 | `Source/Akbal/Private/Tests/AkbalRhythmJudgmentTests.cpp`, `AkbalRhythmChartTests.cpp` |
| `Akbal.UI.*` | 4 | `Source/Akbal/Private/Tests/AkbalRitualUiTests.cpp` |

Run locally:

```powershell
.\Scripts\RunAutomationTests.ps1 -Filter "Akbal" -BuildFirst
```

Close the editor before CLI builds (Live Coding blocks UBT).

## Functional maps

Create focused test maps rather than requiring the full game world to validate systems.

Suggested:

- `L_Test_Ritual`
- `L_Test_StunHeartbeat`
- `L_Test_AttackInterrupt`
- `L_Test_AudioTiming` — **not yet created** (F001 T006)

F001 spike currently PIEs in any map with `AAkbalRhythmSpikeGameMode` as global default.

## Debug overlay

Developer-only overlay should expose hidden values and musical timeline.

F001 spike:

- `UAkbalRhythmHarnessWidget` — BPM, instruments, latency, approach/preview tuning, pause/restart.
- `LogAkbalRitualUi` — widget creation and HUD paint diagnostics.
- On-screen status from `AAkbalRhythmSpikePlayerController::LogRitualUiState`.
