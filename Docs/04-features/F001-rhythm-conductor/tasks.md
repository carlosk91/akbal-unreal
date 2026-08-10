# Tasks — F001 Rhythm Conductor

> Status snapshot: [`implementation-status.md`](implementation-status.md)

- [x] T001 — Establish feature docs and governing references.
- [x] T002 — Add Public/Private module layout and audio/input dependencies.
- [x] T003 — Add judgment types, evaluator, and automation tests.
- [x] T004 — Implement `UAkbalMusicConductorSubsystem` Quartz wrapper.
- [x] T005 — Implement spike game mode, player controller, character, and legacy spike actor.
- [x] T009 — Split production `UAkbalRitualInputHud` from dev `UAkbalRhythmHarnessWidget`; add session, preview window, and player-centered square layout.
- [x] T010 — Add `Scripts/BuildEditor.ps1`, `Scripts/RunAutomationTests.ps1`, and `Akbal.UI.*` / extended `Akbal.Rhythm.*` tests.
- [ ] T006 — Create `L_Test_AudioTiming` and `MS_Spike_Click` in Editor (manual).
- [ ] T007 — Execute manual spike measurements and complete `research.md`.
- [ ] T008 — Promote ADR-0005 based on research evidence.
- [ ] T011 — Remove legacy `UAkbalRhythmSpikeWidget` (superseded; optional cleanup).
- [ ] T012 — Revert `GlobalDefaultGameMode` from spike game mode when F001 is no longer default PIE target.
