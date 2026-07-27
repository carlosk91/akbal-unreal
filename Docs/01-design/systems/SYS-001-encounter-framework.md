---
id: SYS-001
title: Encounter Framework
status: approved
---

# Encounter Framework

## Intent

Define the continuous encounter model without treating action and ritual as mutually exclusive phases.

## Requirements

- **FR-001** Encounter music begins when the encounter becomes active and remains the same composition through resolution.
- **FR-002** Survival/defense and ritual activity may overlap.
- **FR-003** The player never deals conventional offensive damage to alebrijes.
- **FR-004** Alebrije attacks are authored in musical relationship to the encounter piece.
- **FR-005** Ritual can be entered only when eligibility/cooldown rules allow it.
- **FR-006** Ritual entry consumes no implicit extra resource unless specified by an ability.

## Encounter layers

```text
Music layer             always active
Alebrije behavior       active or stunned
Player survival         always available subject to action locks
Ritual interaction      optional while eligible
Sync progression        active while ritual is evaluated
```

## Invariants

- **INV-001** No safe-mode transition is guaranteed by entering ritual.
- **INV-002** Re-entering ritual never refreshes stun.
- **INV-003** Physical attack interruption is not a ritual miss.
- **INV-004** Encounter resolution is restoration, not death.

## TBD

- `TBD-001`: exact universal stun-counter generation loop; prototype required.
