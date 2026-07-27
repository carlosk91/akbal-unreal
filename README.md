# Akbal

**Akbal** is a single-player isometric adventure for Windows built in **Unreal Engine 5.7.4** with a **hybrid C++ + Blueprint** architecture. The player restores alebrijes through musical ritual and synchronization — not conventional combat damage.

This repository uses **spec-driven development (SDD)**: approved documents define behavior and architecture; code implements them. If you are an AI agent or a new contributor with no prior context, read this file first, then follow the paths below.

---

## Quick orientation

| Question | Answer |
|---|---|
| What is this game? | Restoration-focused adventure where encounters are musical, spatial, and ritual-driven. See [`Docs/01-design/GDD.md`](Docs/01-design/GDD.md). |
| What engine / platform? | UE **5.7.4**, **Windows**, **single-player** only. |
| Where is the Unreal project? | Repository root — open `Akbal.uproject` from here. |
| What rules bind agents? | [`AGENTS.md`](AGENTS.md) (mandatory). Cursor: [`.cursor/rules/`](.cursor/rules/). Rider: [`.aiassistant/rules/`](.aiassistant/rules/). |
| What should I build next? | Phase 0 bootstrap is complete. Next: **`F001-rhythm-conductor`** — see [`Docs/07-roadmap/INITIAL_IMPLEMENTATION_PLAN.md`](Docs/07-roadmap/INITIAL_IMPLEMENTATION_PLAN.md). |
| Where is everything documented? | [`Docs/README.md`](Docs/README.md) — full index. |
| How do I implement a feature? | Follow [How to implement anything](#how-to-implement-anything) below. |

---

## First read order (zero context)

Read these **before making changes**:

1. [`AGENTS.md`](AGENTS.md) — binding agent and engineering rules
2. [`Docs/00-governance/GOV-001-project-constitution.md`](Docs/00-governance/GOV-001-project-constitution.md) — product identity and pillars
3. [`Docs/PROJECT_STRUCTURE.md`](Docs/PROJECT_STRUCTURE.md) — repository layout
4. [`Docs/02-technical/GTD.md`](Docs/02-technical/GTD.md) — technical architecture summary
5. [`Docs/07-roadmap/INITIAL_IMPLEMENTATION_PLAN.md`](Docs/07-roadmap/INITIAL_IMPLEMENTATION_PLAN.md) — phased delivery order
6. The relevant **`SYS-*`**, **`TECH-*`**, and **`F###-*`** documents for your task

For human onboarding, also see [`START_HERE.md`](START_HERE.md) and [`CONTRIBUTING.md`](CONTRIBUTING.md).

---

## Repository map

Repository root **is** the Unreal project root.

```text
akbal-unreal/
├── Akbal.uproject          # Open this in UE 5.7 / Rider
├── Config/                 # Engine and project INI settings
├── Content/
│   └── _Akbal/             # All first-party Unreal assets live here
├── Source/
│   ├── Akbal.Target.cs
│   ├── AkbalEditor.Target.cs
│   └── Akbal/              # Single runtime module (domain folders inside)
│       ├── Public/         # Cross-boundary headers (Ability, AI, Audio, …)
│       └── Private/        # Implementation + Tests/
├── Docs/                   # All design, technical, feature, and governance specs
├── Scripts/                # Build/automation scripts (when added)
├── SourceAssets/           # Curated import sources (WAV, FBX, etc.) — not raw DAW sessions
├── AGENTS.md               # Agent instructions (tool-neutral)
├── .cursor/rules/          # Cursor project rules
├── .cursor/skills/         # Cursor procedural skills
└── .aiassistant/rules/     # JetBrains Rider AI Assistant rules
```

### Where things go

| You are adding… | Put it here |
|---|---|
| Stable gameplay rules, state machines, formulas | `Source/Akbal/Private/<Domain>/` with public headers in `Public/<Domain>/` only when needed cross-domain |
| Content assembly, tuning, VFX/audio hookups | Blueprint under `Content/_Akbal/<Domain>/` |
| Alebrije / boss / encounter content | `Content/_Akbal/Alebrijes/`, `Encounters/`, etc. |
| Songs, channels, MetaSounds | `Content/_Akbal/Music/` |
| Data definitions (Primary Data Assets) | `Content/_Akbal/Data/` and/or feature folders |
| Feature specification | `Docs/04-features/F###-short-name/` |
| Irreversible architecture decision | `Docs/05-decisions/ADR-####-title.md` |
| Import source files (committed) | `SourceAssets/` (see LFS policy) |

Naming conventions: [`Docs/00-governance/GOV-003-naming-and-syntax-conventions.md`](Docs/00-governance/GOV-003-naming-and-syntax-conventions.md).

Do **not** create additional runtime modules without an approved ADR. Do **not** put canonical gameplay logic only in a Level Blueprint.

---

## Documentation system

All authoritative design and engineering knowledge lives under **`Docs/`**.

```text
Docs/
├── 00-governance/     GOV-*   Rules, workflow, naming, Git policy, definition of done
├── 01-design/         GDD, SYS-*   What the game is and how systems behave
├── 02-technical/      GTD, TECH-*  How systems are implemented
├── 03-content/        Content templates for alebrijes, songs, zones, etc.
├── 04-features/       F###-* per-feature spec / plan / tasks / test-plan
├── 05-decisions/      ADR-*   Architecture decision records
├── 06-prompts/        Role-based AI prompt templates
└── 07-roadmap/        Phased implementation plan
```

### Document authority (when docs disagree)

Use this precedence — **lower levels must not silently override higher levels**:

1. Approved **ADR** that explicitly supersedes an older decision
2. Approved **system spec** (`SYS-*`) — gameplay behavior
3. Approved **technical spec** (`TECH-*`) — implementation contracts
4. **GDD** / **GTD** summaries
5. **Feature specs** (`Docs/04-features/F###-*/spec.md`)
6. **Content specs**
7. Prototype notes and research

If two approved documents conflict and no ADR resolves it: **stop, report the conflict, and ask for human resolution.**

Full index: [`Docs/README.md`](Docs/README.md).

---

## How to implement anything

This is the standard workflow for non-trivial work. Do not skip steps that apply to your task.

### 1. Understand scope

- Identify which **phase** and **feature ID** your work belongs to ([`INITIAL_IMPLEMENTATION_PLAN.md`](Docs/07-roadmap/INITIAL_IMPLEMENTATION_PLAN.md)).
- Read the governing **`SYS-*`** (behavior) and **`TECH-*`** (implementation) documents.
- Read [`AGENTS.md`](AGENTS.md) invariants — especially what Akbal is **not** (no conventional damage to alebrijes, no invented `TBD` values as permanent rules).

### 2. Spec before code

For any feature that changes player-visible behavior or architecture:

1. Create or update `Docs/04-features/F###-name/` using [`Docs/04-features/_template/`](Docs/04-features/_template/)
2. Write **`spec.md`** — what it does (behavior, edge cases, data)
3. Write **`plan.md`** — C++/Blueprint boundary, owners, state, timing source, tests
4. Write **`tasks.md`** — executable checklist
5. Write **`test-plan.md`** — how to validate

**Do not implement** until the spec (and plan, for non-trivial work) is approved.

Prompt templates for each role: [`Docs/06-prompts/README.md`](Docs/06-prompts/README.md).

### 3. Branch and implement

```text
main
  └── feature/F###-short-description    # or fix/, docs/, tech/, spike/
```

- Branch naming: [`Docs/00-governance/GOV-005-source-control-policy.md`](Docs/00-governance/GOV-005-source-control-policy.md)
- Workflow: [`Docs/00-governance/GOV-004-development-workflow.md`](Docs/00-governance/GOV-004-development-workflow.md)
- Keep branches short-lived; integrate often to reduce binary merge risk.

Implementation rules:

- **C++** owns stable, reusable gameplay rules and testable logic.
- **Blueprint** owns content assembly, tuning, presentation, and per-creature orchestration.
- Prefer **Data Assets** and **Gameplay Tags** where specs call for data-driven content.
- Rhythm/timing must use an **authoritative musical clock** and input timestamps — never judge rhythm from animation or widget timing alone.
- If implementation forces a design decision not covered by specs → **stop and record an ADR**.

### 4. Validate

Minimum bar ([`Docs/00-governance/GOV-006-definition-of-done.md`](Docs/00-governance/GOV-006-definition-of-done.md)):

- Relevant **Editor target compiles**
- **Automated tests** for deterministic logic (timing math, sync, corruption, stun, charge formulas)
- **Playtest or Functional Test** checklist from the feature test plan
- No unintended binary asset changes
- Documentation updated in the same change when contracts change

A feature is not done because it compiles.

### 5. Review and merge

See [`CONTRIBUTING.md`](CONTRIBUTING.md) for PR expectations.

---

## Core game invariants (do not violate)

These are non-negotiable unless an approved ADR and spec explicitly change them. Full list: [`AGENTS.md`](AGENTS.md).

- The player **never conventionally attacks or damages** alebrijes.
- **Restoration**, **synchronization**, **stun**, **physical health**, **corruption**, and **Shamanic Charge** are distinct systems.
- HUD shows three bars: **Health**, **Shamanic Charge**, **Synchronization**. Stun is diegetic, not a fourth bar.
- **Ritual failure** affects sync/corruption/composure — not direct health damage.
- Only **physical attacks** interrupt ritual by default; ranged attacks do not unless a content spec overrides.
- **Berserk** and **Soothing** are mutually exclusive path abilities chosen at playthrough start.
- **No pausing** during Berserk-path boss encounters; Soothing path may pause normally.
- **Single-player only** — no networking unless an approved design change adds it.

If a requirement is marked **`TBD`**, do not invent a permanent value. Use configuration, a labeled prototype assumption, or ask.

---

## Build and tooling

### Requirements

- Unreal Engine **5.7.4** (project association: `5.7`)
- Visual Studio 2022 or compatible toolchain (UE 5.7 preferred MSVC **14.44**)
- Git + Git LFS

### Generate project files

UE 5.7 uses UnrealBuildTool directly (no `GenerateProjectFiles.bat` on Windows):

```powershell
dotnet "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.dll" `
  -projectfiles -project="$PWD\Akbal.uproject" -game -engine -progress
```

Adjust the UE install path if yours differs.

### Build Editor target

```powershell
& "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat" `
  AkbalEditor Win64 Development `
  -Project="$PWD\Akbal.uproject" -WaitMutex
```

### IDE setup

- **Cursor**: open repository root; rules in [`.cursor/rules/`](.cursor/rules/), skills in [`.cursor/skills/`](.cursor/skills/)
- **Rider**: open repository root; import [`.aiassistant/rules/`](.aiassistant/rules/) per [`Docs/IDE_AI_SETUP.md`](Docs/IDE_AI_SETUP.md)

---

## Git and binary assets

- **Git LFS** tracks `*.uasset`, `*.umap` (lockable), and curated production sources (`*.wav`, `*.fbx`, `*.png`, etc.)
- **Never commit**: `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `.vs/`, IDE caches
- Before editing a shared binary asset: pull latest `main`, **lock with LFS**, tell your partner, merge promptly, unlock after merge
- Do not mass-rename or move Unreal assets without explicit approval

Policy details: [`Docs/00-governance/GOV-005-source-control-policy.md`](Docs/00-governance/GOV-005-source-control-policy.md).

---

## Current status and roadmap

| Phase | Status | Focus |
|---|---|---|
| **Phase 0** — Repository bootstrap | **Complete** | Flat layout, Docs, Git LFS, module folders, Editor build verified |
| **Phase 1** — Timing spike | **Next** | `F001-rhythm-conductor` — Quartz/MetaSounds, musical clock, input judgment |
| Phase 2 | Planned | `F002-player-resources` |
| Phase 3 | Planned | `F003-ritual-sync` |
| Phase 4+ | Planned | Stun, combat sandbox, path abilities, vertical slice |

Full roadmap: [`Docs/07-roadmap/INITIAL_IMPLEMENTATION_PLAN.md`](Docs/07-roadmap/INITIAL_IMPLEMENTATION_PLAN.md).

**Principle:** Do not build generalized systems for all future content until the first complete encounter proves the contracts.

Many tuning values, zone genres, and content details remain **`TBD`** by design (July 2026 baseline).

---

## AI agent quick reference

| Task | Start here |
|---|---|
| Implement a new feature | [`AGENTS.md`](AGENTS.md) → feature `SYS-*`/`TECH-*` → `Docs/04-features/F###/` → [`.cursor/skills/implement-feature/`](.cursor/skills/implement-feature/SKILL.md) |
| Write a feature spec | [`Docs/06-prompts/PROMPT-001-spec-author.md`](Docs/06-prompts/PROMPT-001-spec-author.md) → [`.cursor/skills/spec-feature/`](.cursor/skills/spec-feature/SKILL.md) |
| Plan implementation | [`Docs/06-prompts/PROMPT-003-technical-planner.md`](Docs/06-prompts/PROMPT-003-technical-planner.md) → [`.cursor/skills/plan-feature/`](.cursor/skills/plan-feature/SKILL.md) |
| Debug Unreal issue | [`.cursor/skills/unreal-debug/`](.cursor/skills/unreal-debug/SKILL.md) |
| Sync docs after a change | [`.cursor/skills/doc-sync/`](.cursor/skills/doc-sync/SKILL.md) |
| Review a feature | [`.cursor/skills/review-feature/`](.cursor/skills/review-feature/SKILL.md) |

Agent policy: [`Docs/00-governance/GOV-007-ai-agent-policy.md`](Docs/00-governance/GOV-007-ai-agent-policy.md).

### Stop and ask when…

- Two approved documents conflict
- Behavior is materially undefined or marked `TBD` and the task requires a permanent decision
- A change would alter player experience, save compatibility, public APIs, or architecture without spec/ADR coverage
- You would need to enable plugins, add modules, or mass-rename binary assets
- Binary asset work might conflict with another developer's in-progress edit

---

## Key design and technical entry points

| Topic | Document |
|---|---|
| Game overview | [`Docs/01-design/GDD.md`](Docs/01-design/GDD.md) |
| Terminology | [`Docs/01-design/GLOSSARY.md`](Docs/01-design/GLOSSARY.md) |
| Encounters | [`Docs/01-design/systems/SYS-001-encounter-framework.md`](Docs/01-design/systems/SYS-001-encounter-framework.md) |
| Music / conductor | [`Docs/01-design/systems/SYS-002-music-and-conductor.md`](Docs/01-design/systems/SYS-002-music-and-conductor.md) |
| Ritual / sync | [`Docs/01-design/systems/SYS-004-ritual-and-synchronization.md`](Docs/01-design/systems/SYS-004-ritual-and-synchronization.md) |
| Runtime architecture | [`Docs/02-technical/architecture/TECH-001-runtime-architecture.md`](Docs/02-technical/architecture/TECH-001-runtime-architecture.md) |
| Audio timing | [`Docs/02-technical/architecture/TECH-002-audio-timing-and-conductor.md`](Docs/02-technical/architecture/TECH-002-audio-timing-and-conductor.md) |
| Input / rhythm judgment | [`Docs/02-technical/architecture/TECH-005-input-and-rhythm-judgment.md`](Docs/02-technical/architecture/TECH-005-input-and-rhythm-judgment.md) |
| Testing | [`Docs/02-technical/architecture/TECH-010-testing-debugging-and-telemetry.md`](Docs/02-technical/architecture/TECH-010-testing-debugging-and-telemetry.md) |
| Build / source control | [`Docs/02-technical/architecture/TECH-011-build-and-source-control.md`](Docs/02-technical/architecture/TECH-011-build-and-source-control.md) |

System specs (`SYS-001`–`SYS-013`) and technical specs (`TECH-001`–`TECH-013`) cover the full game surface — browse [`Docs/01-design/systems/`](Docs/01-design/systems/) and [`Docs/02-technical/architecture/`](Docs/02-technical/architecture/).

---

## License and contributors

See [`CONTRIBUTING.md`](CONTRIBUTING.md) for branch, PR, and collaboration expectations.
