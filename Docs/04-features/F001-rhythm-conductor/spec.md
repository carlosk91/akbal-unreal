---
id: F001
name: Rhythm Conductor
status: approved
owners: []
design_specs:
  - SYS-002
technical_specs:
  - TECH-002
  - TECH-005
adrs:
  - ADR-0005
---

# Feature Specification — Rhythm Conductor (Phase 1 Spike)

## Intent

Validate that Unreal Quartz can serve as Akbal's authoritative musical clock and that rhythm input judgment can be measured consistently against that clock across frame rates and pause/resume.

## Goals

- G-001: Prove Quartz clock start/stop, position query, and beat subscription for encounter timing.
- G-002: Provide a developer debug readout of musical position, latency, and last judgment.
- G-003: Prove input timestamp judgment is frame-rate independent when referenced to the conductor clock.
- G-004: Measure pause/resume behavior without losing transport state.

## Non-goals

- NG-001: Full encounter state machine or boss AI.
- NG-002: Production ritual chart authoring or UI.
- NG-003: Final tuning values for judgment windows (prototype defaults only).
- NG-004: Shipping MetaSound content pipeline.

## Player-facing behavior

Not player-facing in Phase 1 as shipped content. The spike game mode exercises a **production-shaped ritual input HUD** (`UAkbalRitualInputHud`) plus a separate **developer harness** (`UAkbalRhythmHarnessWidget`) for pause, restart, latency, BPM, instrument count, and note timing tuning.

## Functional requirements

- FR-001: A world-scoped conductor can start/stop/pause/resume a Quartz encounter clock.
- FR-002: The conductor exposes current musical position (bar, beat, beat fraction, seconds).
- FR-003: The conductor exposes Quartz latency metrics for spike documentation.
- FR-004: Input taps are judged against the nearest beat using configurable millisecond windows.
- FR-005: Judgment outputs are `Perfect`, `Good`, `Pass`, or `Miss` per TECH-005.
- FR-006: A spike actor provides on-screen debug readout and keyboard controls for manual validation.
- FR-007: `UAkbalRitualInputHud` renders a square lane overlay (max 20% of the smaller viewport dimension) centered on the player with equal X/Y scale.
- FR-008: The HUD shows only notes within a configurable preview horizon (`NotePreviewSeconds`, default 1.0s).
- FR-009: `UAkbalRhythmHarnessWidget` exposes dev-only controls (BPM, instrument count 1–4, note approach speed, preview window, latency, pause/restart) without coupling to production HUD paint logic.

## Invariants

- INV-001: Judgment math must not depend on frame delta or widget timing.
- INV-002: Input timestamps must be captured against the conductor clock at input processing time.
- INV-003: Pause must freeze musical transport; resume must continue from the paused position.

## Scenarios

### SC-001 — Beat judgment at 60 FPS

Given an encounter clock at 120 BPM and a player tap within the Perfect window of the nearest beat,
When the tap timestamp is evaluated,
Then the judgment is `Perfect`.

### SC-002 — Frame-rate independence

Given identical tap offsets from the nearest beat,
When evaluated at simulated 30/60/120 FPS capture delays,
Then the judgment tier is identical for the same underlying timestamp.

### SC-003 — Pause and resume

Given a running encounter clock,
When the game is paused and later resumed,
Then musical position continues from the pre-pause transport without reset.

## Edge cases

- Clock started twice: second start is ignored or safely resets per spike policy (document in research).
- Input before clock start: returns `Miss` with diagnostic reason.
- Tap during pause: timestamp reflects frozen transport.

## Debugging requirements

- On-screen overlay: bar/beat, seconds, BPM, running/paused state, last judgment, delta ms, Quartz latency.
- Console-accessible spike controls for pause/resume and clock restart.

## Acceptance criteria

- AC-001: Project compiles on UE 5.7.4 Editor target.
- AC-002: Automation tests pass for judgment math and simulated multi-FPS timestamp capture.
- AC-003: Manual spike map exercise documents Quartz latency and pause/resume in `research.md`.
- AC-004: MetaSound hook exists for editor-authored validation (asset creation is manual).

## TBD / questions

- TBD-001: Final judgment window milliseconds — use prototype values until ritual tuning begins.
- TBD-002: MetaSound adaptive stem transitions — measure in editor, document in research.
