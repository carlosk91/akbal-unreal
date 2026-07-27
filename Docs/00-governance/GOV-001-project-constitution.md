---
id: GOV-001
title: Project Constitution
status: approved
---

# Project Constitution

## Purpose

This document contains rules that should change rarely. System tuning and content details do not belong here.

## Product identity

Akbal is a single-player isometric adventure built around restoration rather than destruction. Alebrijes are threatened, corrupted, fragmented, or out of rhythm; the player survives them, creates ritual opportunities, and restores them through synchronization.

## Design pillars

1. **Restore, do not kill.** The player has no conventional offensive damage loop against alebrijes.
2. **The world is musical.** Encounter movement, attack choreography, ritual play, environmental restoration, and progression are tied to musical identity.
3. **Healing is exposed.** Ritual play occurs under pressure; it is not a detached safe minigame.
4. **Progress is communion.** Instruments, restored alebrijes, world changes, body patterns, and traversal access represent growth.
5. **Difficulty is musical and spatial.** Difficulty grows through chart fidelity, layering, polyrhythm, attack density, interference, and simultaneous attention—not only BPM.
6. **Readable systems, minimal HUD.** Prefer diegetic feedback when it is legible. The three primary visible bars are Health, Shamanic Charge, and Synchronization.

## Development principles

1. Specifications precede non-trivial implementation.
2. Approved behavior is data-driven where content varies.
3. C++ owns stable system rules; Blueprint owns content-facing assembly and presentation.
4. Timing-critical rhythm logic must use authoritative timing, not frame-dependent approximations.
5. Tests must cover deterministic gameplay math and critical state transitions.
6. Unknown values remain explicit `TBD`s rather than fabricated constants.
7. Every major irreversible architectural choice receives an ADR.

## Scope constraints

- Engine: Unreal Engine 5.7.4.
- Platform: Windows.
- Single-player only.
- Hybrid C++ and Blueprint.
- Git + Git LFS under GitHub Free constraints.
