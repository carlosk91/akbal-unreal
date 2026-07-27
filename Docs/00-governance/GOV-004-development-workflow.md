---
id: GOV-004
title: Development Workflow
status: approved
---

# Development Workflow

## Feature lifecycle

```text
Idea / bug
  -> design/system contract
  -> feature spec
  -> research/spike if needed
  -> technical plan
  -> tasks
  -> implementation
  -> automated + playtest validation
  -> documentation sync
  -> PR review
  -> merge
```

## Step 1 — establish authority

Before work, identify the authoritative `SYS-*` and `TECH-*` docs. If none exists, create/update them first.

## Step 2 — feature package

Use `Docs/04-features/_template/` for non-trivial work.

A feature is `approved` only after humans agree on its behavior and critical architecture.

## Step 3 — plan before code

The plan must define:

- Runtime owner.
- C++ / Blueprint boundary.
- Data assets and tags.
- State transitions.
- Timing source.
- Save impact.
- Debugging support.
- Tests.
- Binary assets expected to change.

## Step 4 — implementation

- Work in small commits.
- Avoid unrelated refactors.
- Prefer vertical playable slices over broad framework construction.
- Expose tuning values as data/configuration when designers need iteration.

## Step 5 — validate

At minimum:

- Compile the relevant Editor target.
- Run targeted automation tests.
- Run the relevant Functional Test or manual playtest checklist.
- Confirm no unintended asset changes.

## Step 6 — review

Review against the spec, not only against code quality.

## Prototype policy

A prototype may intentionally violate final architecture to answer a question quickly, but it must be clearly labeled `prototype` and cannot become production architecture by inertia.

## Branching

Default flow:

```text
main
  \-- feature/F###-short-name
```

Branches should usually live less than a few days. Integrate often to reduce `.uasset` merge risk.
