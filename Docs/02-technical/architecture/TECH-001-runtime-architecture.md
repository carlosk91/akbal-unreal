---
id: TECH-001
title: Runtime Architecture
status: proposed
last_updated: 2026-08-10
---

# Runtime Architecture

## Module strategy

Start with one runtime module: `Akbal`.

Reason: a two-developer early project benefits more from low ceremony and fast iteration than from speculative module boundaries. Organize by domain folders and extract modules only when dependencies, compile times, editor-only requirements, or ownership justify it.

**F001 domains populated:** `Audio/`, `Input/`, `Spike/`, `UI/Ritual/`, `UI/Dev/`, `Tests/`. See [`F001 implementation-status`](../../04-features/F001-rhythm-conductor/implementation-status.md).

## C++ / Blueprint boundary

C++ owns:

- Resource math/state.
- Ritual state machine.
- Sync section rules.
- Stun/heartbeat rules.
- Timing interfaces.
- Save contracts.
- Attack semantic effects.
- Debug/test hooks.

Blueprint owns:

- Creature-specific orchestration.
- Animation/VFX/audio hookups.
- Encounter assembly.
- Content tuning via exposed data.
- UI presentation.

## Dependency direction

Presentation may depend on gameplay state interfaces; gameplay core must not depend on UI widgets or specific visual assets.
