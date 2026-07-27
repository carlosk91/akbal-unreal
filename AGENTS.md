# AGENTS.md — Akbal Agent Instructions

These instructions apply to coding agents working in the Akbal repository.

## Project

- Game: **Akbal**
- Engine: **Unreal Engine 5.7.4**
- Target: **Windows**
- Mode: **Single-player**
- Programming model: **Hybrid C++ + Blueprint**
- Version control: **Git + Git LFS**
- Primary IDEs: **Cursor** and **JetBrains Rider**

## Mandatory reading before changes

1. `Docs/00-governance/GOV-001-project-constitution.md`
2. `Docs/00-governance/GOV-003-naming-and-syntax-conventions.md`
3. `Docs/00-governance/GOV-004-development-workflow.md`
4. `Docs/00-governance/GOV-006-definition-of-done.md`
5. The relevant `SYS-*`, `TECH-*`, and feature files for the task.

## Core game invariants

- The player never conventionally attacks or damages alebrijes.
- Encounters use bullet-hell spatial pressure and hack-and-slash responsiveness without offensive damage.
- Restoration, synchronization, stun, physical health, corruption, and Shamanic Charge are distinct concepts.
- The HUD has three visible bars: Player Health, Player Shamanic Charge, Alebrije Synchronization.
- Stun is communicated diegetically through alebrije patterns, not a separate stun bar.
- Ritual failure affects synchronization, corruption, and hidden Ritual Composure; it does not directly damage player health.
- Only physical attacks interrupt ritual by default. Ranged attacks do not interrupt unless a content spec explicitly overrides the attack contract.
- Re-entering ritual never refreshes stun duration.
- Outside Berserk, completing a synchronization section ends stun immediately.
- Berserk and Soothing are mutually exclusive path abilities selected at the beginning of a playthrough.
- No pausing is allowed during a boss encounter on the Berserk path. Soothing-path boss encounters may pause normally.

## Spec-driven workflow

- Never implement a feature whose behavior is materially undefined.
- If a requirement is `TBD`, do not invent a permanent rule. Use data/configuration, a clearly marked prototype assumption, or ask.
- For non-trivial features, create or update `Docs/04-features/F###-name/` before code.
- `spec.md` defines behavior; `plan.md` defines implementation; `tasks.md` defines executable work; `test-plan.md` defines validation.
- Keep design decisions out of implementation details. If code forces a design decision, stop and request/record an ADR.
- Update documentation in the same change when behavior or architecture changes.

## Unreal architecture

- Prefer stable reusable gameplay rules in C++.
- Use Blueprint for content assembly, tuning, animation/VFX/audio hookups, and creature-specific orchestration.
- Do not hide canonical gameplay rules only in a Level Blueprint or a large Event Graph.
- Prefer data-driven content definitions using Data Assets / Primary Data Assets where appropriate.
- Prefer Gameplay Tags for cross-system semantic state/event identifiers where they improve decoupling.
- Treat Quartz + MetaSounds as the planned audio timing direction, but validate implementation details through a technical spike before hard-coding unsupported assumptions.
- Core rhythm judgment must use an authoritative musical clock and input timestamps; do not judge rhythm from animation frames or visual widget timing.

## Code standards

- Follow Epic's Unreal C++ coding standard.
- Project code is English-only: identifiers, comments, docs, asset names, commits.
- C++ project prefix is `Akbal`: `AAkbalPlayerCharacter`, `UAkbalRitualComponent`, `FAkbalSyncSection`.
- One primary C++ type per file when practical.
- Avoid unnecessary public headers. Keep implementation details private.
- Avoid speculative abstractions. Build only contracts required by approved specs.
- No networking code unless an approved design change adds multiplayer.

## Binary asset safety

- `.uasset` and `.umap` are binary; avoid parallel edits.
- Lock shared binary assets before editing when LFS locking is in use.
- Do not mass-rename or move Unreal assets without explicit approval because redirectors and binary references can create broad changes.
- Do not commit `Binaries/`, `DerivedDataCache/`, `Intermediate/`, `Saved/`, or local IDE caches.

## Testing

- New pure/domain logic must have automated tests where feasible.
- Timing math, synchronization progression, corruption, stun recovery, and charge formulas require deterministic tests.
- Randomized systems must support seeded reproducibility in tests and debug builds.
- Gameplay integration should use Unreal Automation Specs / Functional Tests where appropriate.
- A feature is not done merely because it compiles.

## Agent behavior

- Inspect before editing.
- Keep diffs narrowly scoped.
- Do not change unrelated files to “clean things up.”
- Do not install dependencies or enable plugins without stating why and obtaining approval when the change is architectural.
- Do not silently rename canonical gameplay terminology.
- If two approved documents conflict, report the conflict and stop before implementation.
- Summarize files changed, tests run, assumptions, and remaining risks at task completion.
