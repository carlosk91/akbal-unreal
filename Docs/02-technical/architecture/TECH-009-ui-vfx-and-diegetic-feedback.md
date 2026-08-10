---
id: TECH-009
title: UI, VFX and Diegetic Feedback
status: proposed
last_updated: 2026-08-10
---

# UI, VFX and Diegetic Feedback

## F001 ritual input prototype (not shipping HUD)

Phase 1 separates production-shaped presentation from dev tooling:

| Widget | Role |
|---|---|
| `UAkbalRitualInputHud` | Paint-only lane overlay; reads `UAkbalRitualInputSession` + conductor — no gameplay rules |
| `UAkbalRhythmHarnessWidget` | Dev-only tuning (BPM, instruments, latency, pause) — not shipping UI |

Layout math: `FAkbalRitualViewportLayout` (unit-tested). Session owns chart/input state.

Guide: [`F001 implementation-status`](../../04-features/F001-rhythm-conductor/implementation-status.md).

## Shipping HUD

Only three primary bars:

- Health with corruption representation.
- Shamanic Charge.
- Alebrije Sync.

## Pattern state bridges

Gameplay exposes normalized/read-only state; materials/VFX decide presentation.

Alebrije pattern inputs may include:

- PreparedStunNormalized.
- RemainingGuaranteedStunNormalized.
- bInHeartbeatRecovery.
- bStunned.

Player pattern inputs may include:

- RitualAvailabilityState.
- ProgressionPatternLevel/collection.

Do not make material parameter state authoritative gameplay state.
