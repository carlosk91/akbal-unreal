---
id: TECH-003
title: Encounter State Model
status: proposed
---

# Encounter State Model

Avoid a single mutually exclusive `ActionPhase/RitualPhase` enum.

Model orthogonal state dimensions:

```text
EncounterLifecycle: Inactive | Active | Restored | Failed
AlebrijeActivity: Active | Stunned | Recovering | Restored
RitualState: Unavailable | Available | Active | Cooldown
PathMode: Berserk | Soothing
```

Additional state:

- Current Sync and section.
- Solidified anchors.
- Stun counters/timer.
- Heartbeat index/chance.
- Ritual Composure/MissStreak.
- Escalation phase.

Transitions should be explicit and testable rather than distributed across animation callbacks and Level Blueprint.
