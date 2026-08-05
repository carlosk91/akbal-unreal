# Test Plan — F001 Rhythm Conductor

## Automated

| ID | Scenario | Type | Expected |
|---|---|---|---|
| TEST-001 | Perfect window hit | Unit | `Perfect` for delta ≤ 25 ms |
| TEST-002 | Good/Pass/Miss boundaries | Unit | Correct tier per window |
| TEST-003 | Beat seconds round-trip | Unit | Position ↔ seconds consistent at 120 BPM |
| TEST-004 | Nearest-beat target selection | Unit | Correct target for off-beat input |
| TEST-005 | FPS simulation 30/60/120 | Unit | Same timestamp → same judgment regardless of frame duration |
| TEST-006 | Input before clock start | Unit | `Miss` |

## Functional

| ID | Scenario | Steps | Expected |
|---|---|---|---|
| FUNC-001 | Clock start/stop | PIE with spike actor, press R to restart | Bar/beat advances, stops on stop |
| FUNC-002 | Beat subscription | Watch debug overlay beat counter | Increments on each Quartz beat |
| FUNC-003 | Tap judgment | Press Space on beat at 120 BPM | `Perfect` or `Good` on intentional taps |
| FUNC-004 | Pause/resume | Press P to pause game, wait, unpause | Position frozen during pause, resumes correctly |

## Manual / feel

| ID | Scenario | Steps | Expected |
|---|---|---|---|
| MAN-001 | 30 FPS cap | `t.MaxFPS 30`, tap along click track | Judgment stable vs 60 FPS for same timing intent |
| MAN-002 | 120 FPS cap | `t.MaxFPS 120`, repeat MAN-001 | No systematic tier shift from frame rate |
| MAN-003 | MetaSound on clock | Assign `MS_Spike_Click`, play quantized | Audible click aligns with beat counter within documented latency |
| MAN-004 | Audio buffer sweep | Try 512/1024 frame buffer in project settings | Record latency impact in research.md |

## Performance

Not a Phase 1 gate. Note overlay tick cost qualitatively.

## Regression

Re-run `Akbal.Rhythm` automation group before merging timing changes.

## Evidence

Record:

- Automation command: `Automation RunTests Akbal.Rhythm`
- Screenshots of debug overlay at 30/60/120 FPS
- `research.md` latency table and pause/resume notes
