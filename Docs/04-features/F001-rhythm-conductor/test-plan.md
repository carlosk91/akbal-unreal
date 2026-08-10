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
| TEST-007 | Arrow key lane mapping | Unit | Left/Right/Up/Down map to the four lanes |
| TEST-008 | Spike chart instrument coverage | Unit | Required spike notes use Drum, Chime, String, and Wind at count 4 |
| TEST-009 | Chart lane + instrument judgment | Unit | Wrong lane/instrument misses; matching input judges |
| TEST-010 | Instrument library round-trip | Unit | All four instruments index and name correctly |
| TEST-011 | Spike chart note count | Unit | At least 36 required notes in spike chart |
| TEST-012 | Chart forces instrument/lane changes | Unit | Frequent alternation between instruments and lanes |
| TEST-013 | Instrument switch preserves chart | Unit | Consumed/missed/upcoming notes unchanged by Q/E |
| TEST-014 | Chart instrument count filter | Unit | Charts for 1/2/3/4 instruments exclude higher-index channels |
| TEST-015 | Note preview visibility | Unit | Notes outside `NotePreviewSeconds` hidden; approaching notes visible; hit-time notes hidden |

## Functional

| ID | Scenario | Steps | Expected |
|---|---|---|---|
| FUNC-001 | Clock start/stop | PIE with spike game mode, press R to restart | Bar/beat advances, stops on stop |
| FUNC-002 | Beat subscription | Watch harness/conductor readout | Increments on each Quartz beat |
| FUNC-003 | Lane judgment | Press matching arrow key on beat at 80 BPM with correct instrument selected | `Perfect` or `Good` on intentional hits |
| FUNC-004 | Pause/resume | Press P to pause game, wait, unpause | Position frozen during pause, resumes correctly |
| FUNC-005 | Ritual HUD layout | PIE third-person, move player | Square HUD (≤20% min viewport dimension) tracks player center |
| FUNC-006 | Note preview window | Observe approaching notes | Only notes within ~1s preview appear |
| FUNC-007 | Harness instrument count | Set 1/2/3 instruments in harness | Chart and HUD show only active channels |
| FUNC-008 | Ghost notes | Switch instrument with Q/E | Inactive instrument notes visible faded |

## Manual / feel

| ID | Scenario | Steps | Expected |
|---|---|---|---|
| MAN-001 | 30 FPS cap | `t.MaxFPS 30`, tap along click track | Judgment stable vs 60 FPS for same timing intent |
| MAN-002 | 120 FPS cap | `t.MaxFPS 120`, repeat MAN-001 | No systematic tier shift from frame rate |
| MAN-003 | MetaSound on clock | Assign `MS_Spike_Click`, play quantized | Audible click aligns with beat counter within documented latency |
| MAN-004 | Audio buffer sweep | Try 512/1024 frame buffer in project settings | Record latency impact in research.md |
| MAN-005 | Playability at default BPM | Play spike chart at 80 BPM default | Chart is playable without retuning windows |

## Performance

Not a Phase 1 gate. Note overlay tick/paint cost qualitatively.

## Regression

Re-run `Akbal.Rhythm` automation group before merging timing changes.

## Evidence

Record:

- Automation command: `Automation RunTests Akbal.Rhythm`
- Screenshots of ritual HUD at 30/60/120 FPS
- `research.md` latency table and pause/resume notes
