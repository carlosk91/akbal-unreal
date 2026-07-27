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

## 4. Next development task

Bootstrap is complete. Start `F001-rhythm-conductor` as a technical spike before broad gameplay implementation.
