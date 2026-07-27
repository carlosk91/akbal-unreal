---
id: GOV-007
title: AI Agent Policy
status: approved
---

# AI Agent Policy

## Goal

Use agents to accelerate implementation without allowing them to become an untracked source of game design or architecture.

## Agent roles

Agents should be explicitly assigned one role per task, for example:

- Spec author
- Unreal gameplay engineer
- Audio/timing engineer
- Test engineer
- Code reviewer
- Documentation steward
- Content-data author

## Mandatory behavior

Agents must:

1. Read `AGENTS.md` and relevant specs first.
2. State assumptions before changing behavior.
3. Ask when an ambiguity changes player experience, save compatibility, public APIs, or architecture.
4. Keep changes minimal.
5. Run or describe validation.
6. Report files changed and unresolved risks.

## Forbidden behavior

Agents must not:

- Invent lore, cultural claims, final zone genres, final instrument identity, or tuning and present them as approved.
- Add conventional offensive damage to alebrijes.
- Introduce multiplayer/network architecture.
- Create hidden canonical logic in a Level Blueprint.
- Mass-rename Unreal assets.
- Change build dependencies/plugins without explicit justification.
- Mark a feature complete without validation.

## Prompt pattern

Every implementation prompt should contain:

1. **Role** — who the agent is acting as.
2. **Objective** — one concrete outcome.
3. **Authority** — files/specs that govern the task.
4. **Constraints** — what must not change.
5. **Process** — inspect, plan, implement, validate.
6. **Deliverable** — expected files/output.
7. **Stop conditions** — when to ask rather than guess.

Use the templates in `Docs/06-prompts/`.
