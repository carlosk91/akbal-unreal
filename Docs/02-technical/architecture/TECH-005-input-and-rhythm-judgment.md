---
id: TECH-005
title: Input and Rhythm Judgment
status: proposed
---

# Input and Rhythm Judgment

## Input

Use Enhanced Input. Keep input mapping separate from judgment math.

## Judgment

A note judgment consumes:

- Required musical timestamp/position.
- Actual player input timestamp referenced to the conductor clock.
- Allowed timing windows after active modifiers.

Outputs should be strongly typed, e.g. `Perfect`, `Good`, `Pass`, `Miss`.

Berserk accepts only `Perfect` for chain continuation; normal ritual may accept other passing judgments.

## Chart model

Required inputs are authored gameplay events, not every sound event in the source music. Chart fidelity can increase across sections/mastery versions.

## Testability

Judgment math must be testable without rendering UI/audio.
