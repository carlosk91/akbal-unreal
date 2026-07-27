# Contributing to Akbal

Akbal is developed through short-lived branches and spec-backed changes.

## Before starting

- Pull the latest `main`.
- Read `AGENTS.md` and the relevant specifications.
- For binary Unreal assets, confirm that your partner is not editing the same asset. Use Git LFS locking when configured.
- Create a short-lived branch using the naming rules in `GOV-005-source-control-policy.md`.

## Pull request expectation

A PR should state:

- Why the change exists.
- Which `SYS-*`, `TECH-*`, `ADR-*`, and `F###` documents govern it.
- What was changed.
- How it was tested.
- Which binary assets were touched.
- Any remaining `TBD` or risk.

Do not mix unrelated refactors, content edits, and gameplay changes in one PR.
