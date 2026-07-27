---
id: GOV-006
title: Definition of Done
status: approved
---

# Definition of Done

A feature is done only when all relevant items are true.

## Contract

- [ ] Behavior matches an approved spec.
- [ ] Any changed design/technical decision is reflected in docs/ADR.
- [ ] No accidental new `TBD` is hidden in code.

## Implementation

- [ ] Code follows `GOV-003`.
- [ ] C++ / Blueprint ownership matches the technical plan.
- [ ] Tuning is data-driven where iteration is expected.
- [ ] No unrelated refactor is included.

## Validation

- [ ] Relevant target compiles.
- [ ] Automated tests for deterministic logic exist and pass.
- [ ] Integration/manual test plan executed.
- [ ] Random systems can be reproduced with a seed in debug/tests.
- [ ] Debug state is observable enough to diagnose failures.

## Content/version control

- [ ] Binary asset edits are intentional.
- [ ] LFS locks released.
- [ ] No generated files committed.
- [ ] Asset naming/folder rules followed.

## Documentation

- [ ] Feature `spec.md`, `plan.md`, `tasks.md`, and `test-plan.md` are current.
- [ ] GDD/GTD/system docs updated if their summaries changed.
- [ ] New gameplay tags/data contracts documented.
