---
id: GOV-002
title: Documentation Standard
status: approved
---

# Documentation Standard

## Canonical document types

| Prefix | Meaning |
|---|---|
| `GOV` | Governance and team rules |
| `SYS` | Player-facing/system design behavior |
| `TECH` | Technical architecture and implementation contracts |
| `ADR` | Architecture/design decision record |
| `F###` | Feature specification package |
| `CONTENT` | Specific boss, zone, song, instrument, or encounter definition |

## Front matter

Canonical documents should begin with:

```yaml
---
id: SYS-001
title: Encounter Framework
status: draft | proposed | approved | deprecated
owners:
  - design
  - gameplay
last_updated: YYYY-MM-DD
---
```

## Requirement IDs

Use stable IDs for requirements that will be implemented or tested:

- `FR-###` functional requirement
- `INV-###` invariant
- `AC-###` acceptance criterion
- `TBD-###` unresolved decision
- `RISK-###` known risk

Do not renumber existing requirement IDs solely for aesthetics.

## Language

All canonical documentation is written in English. Original Spanish source material may be retained in `/Docs/99-source/` if desired, but canonical rules are English.

## Cross-reference rule

A document may summarize another canonical document, but it should link to the authoritative owner of the rule instead of duplicating detailed formulas or tuning values.

## `TBD` policy

A `TBD` must state:

- What is unknown.
- Whether implementation is blocked.
- The intended method of resolution: prototype, playtest, narrative decision, art test, etc.

## Change discipline

When implementation reveals a design conflict:

1. Stop before silently choosing behavior.
2. Update or create the relevant spec/ADR.
3. Obtain human approval.
4. Implement against the approved contract.
