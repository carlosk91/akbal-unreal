# Akbal — Spec-Driven Development Bootstrap

This repository uses **spec-driven development (SDD)** for **Akbal**, a Windows single-player Unreal Engine 5.7.4 project developed by two people using Cursor and JetBrains Rider.

## Repository principle

Akbal uses **spec-driven development (SDD)**:

1. Design intent is defined in the GDD and system specifications.
2. Technical contracts are defined in the GTD and technical specifications.
3. A feature receives its own `spec.md`, implementation `plan.md`, executable `tasks.md`, and `test-plan.md`.
4. Implementation begins only when the relevant specification is approved.
5. Important decisions are recorded as ADRs instead of being silently re-decided in code or chat.
6. AI agents must ask when a requirement is ambiguous or conflicts with an approved document.

## Recommended repository root

Because this repository contains a single Unreal project, the recommended layout places `Akbal.uproject` at the **Git repository root**, rather than inside a second `Akbal/` directory. This makes build paths, AI instruction discovery, scripts, documentation, and version control simpler.

See `Docs/00-governance/GOV-004-development-workflow.md` before implementing gameplay systems.

## Canonical document order

When documents conflict, use this precedence:

1. Approved ADR that explicitly supersedes an older decision.
2. Approved system specification (`SYS-*`) for gameplay behavior.
3. Approved technical specification (`TECH-*`) for implementation contracts.
4. GDD / GTD summaries.
5. Feature specifications.
6. Content specifications.
7. Prototype notes and research.

A lower-level document may not silently override a higher-level canonical contract.

## Current status

This bootstrap captures the design decisions discussed through July 2026. Numeric tuning values, final zone genres, final fourth instrument identity, exact ability tuning, and some content details remain `TBD` by design.
