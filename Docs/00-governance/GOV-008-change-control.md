---
id: GOV-008
title: Change Control
status: approved
---

# Change Control

## When to create an ADR

Create an ADR when changing:

- A core design pillar.
- System ownership or module boundaries.
- The authoritative musical clock.
- Save format/state ownership.
- Asset/data architecture.
- Source-control strategy.
- A rule that impacts multiple systems or existing content.

## When a SYS update is enough

Use a normal system-spec revision when changing player-facing rules within an existing architecture, such as sync drain behavior or attack interruption semantics.

## Deprecation

Never delete historical decisions merely because they changed. Mark the old document/section deprecated and link to the superseding ADR/spec.

## Prototype promotion

Prototype code may be promoted only after:

- The prototype question has an answer.
- The production contract is documented.
- Technical debt from the prototype is explicitly accepted or removed.
