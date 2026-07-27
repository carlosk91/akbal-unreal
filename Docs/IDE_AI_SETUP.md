# Cursor and JetBrains Rider AI Setup

## Shared source of truth: `AGENTS.md`

Keep the repository-root `AGENTS.md` tool-neutral. Both Cursor and modern JetBrains coding agents can consume project instructions; this prevents two conflicting instruction sets.

## Cursor

### Rules

Project rules live in `.cursor/rules/*.mdc` and are version-controlled. Use:

- Always rules for small non-negotiable project constraints.
- Auto-attached rules for C++ or docs by glob.
- Agent-requested rules for specialized context.
- Manual rules for workflows you only invoke intentionally.

Avoid a giant always-on rule; keep context focused.

### Skills

Project-scoped skills live under `.cursor/skills/<skill>/SKILL.md`. Skills are procedural workflows, while rules are constraints/context. Invoke a skill with the slash menu or allow Cursor to discover it from the skill description.

Restart/reopen an agent session if a newly created skill is not visible.

### Recommended workspace

Open the repository root after flattening the Unreal project so Cursor can see `AGENTS.md`, `Docs/`, `.cursor/`, and `Akbal.uproject` together.

## JetBrains Rider — AI Assistant chat

JetBrains project rules can be stored under `.aiassistant/rules/*.md`.

In Rider:

1. Open `Settings`.
2. Go to `Tools > AI Assistant > Rules`.
3. Create/import the project rule files from `.aiassistant/rules`.
4. Configure `00-core` and `10-spec-driven` as **Always**.
5. Configure the C++ rule by file pattern for `**/*.h`, `**/*.cpp`, `**/*.cs`.
6. Configure documentation rule for `Docs/**/*.md`.

## Rider — Junie

Junie reads project instructions from `AGENTS.md`. Current JetBrains documentation also supports `.junie/AGENTS.md`, but using the root file avoids duplication.

Open/configure the project root as Junie's project path. If Rider treats only a nested Unreal directory as the project root, point Junie's project settings at the repository root or flatten the repository as proposed.

Junie respects `.aiignore`.

## Rider — other agents

JetBrains AI Assistant can host agents such as Codex, Claude Agent and GitHub Copilot depending on installation/account. Their support for instruction files and skills differs. `AGENTS.md` is the preferred common denominator for Akbal; do not duplicate canonical game rules into agent-specific files unless necessary.

## Code style

Prefer `.editorconfig` plus Epic/Unreal conventions for cross-IDE consistency. Rider can share Project code-style settings under `.idea/codeStyles`, but do not commit the entire `.idea` directory by default.

## Ignore files

`.aiignore` and `.cursorignore` should exclude generated Unreal directories and local caches so agents spend context on source/specs rather than generated artifacts.
