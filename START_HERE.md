# Start Here

## 1. Review the bootstrap

Read in order:

1. `Docs/PROJECT_STRUCTURE.md`
2. `Docs/00-governance/GOV-001-project-constitution.md`
3. `Docs/01-design/GDD.md`
4. `Docs/02-technical/GTD.md`
5. `Docs/07-roadmap/INITIAL_IMPLEMENTATION_PLAN.md`
6. `Docs/IDE_AI_SETUP.md`

## 2. Repository layout

The repository uses a single-project layout: `Akbal.uproject` lives at the repository root alongside `Docs/`, `Source/`, `Config/`, and AI configuration files.

When updating `.gitattributes` or `.gitignore`, merge additions deliberately rather than replacing the whole file.

## 3. Configure AI tools

- Cursor: root `AGENTS.md`, `.cursor/rules`, `.cursor/skills`.
- Rider AI Assistant: `.aiassistant/rules` and root `AGENTS.md`.
- Junie: use root `AGENTS.md` as project guidelines.

## 4. Current development status

| Phase | Status |
|---|---|
| Phase 0 — bootstrap | **Complete** |
| Phase 1 — `F001-rhythm-conductor` | **Implemented** — C++, tests, spike PIE. Manual validation (T006–T008) still open. |
| Phase 2 — `F002-player-resources` | **Next** |

**Zero-context guide to Phase 1:** [`Docs/04-features/F001-rhythm-conductor/implementation-status.md`](Docs/04-features/F001-rhythm-conductor/implementation-status.md)

### Quick spike test

1. `.\Scripts\BuildEditor.ps1`
2. Open `Akbal.uproject`, press Play (spike game mode is default in `Config/DefaultEngine.ini`).
3. Arrow keys = lanes, Q/E = instrument, P = pause, R = restart. Harness panel = top-left.

### Quick automated test

```powershell
.\Scripts\RunAutomationTests.ps1 -Filter "Akbal" -BuildFirst
```
