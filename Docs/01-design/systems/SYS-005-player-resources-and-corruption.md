---
id: SYS-005
title: Player Resources and Corruption
status: approved
---

# Player Resources and Corruption

## Visible resources

1. Health bar.
2. Continuous Shamanic Charge bar.
3. Alebrije Synchronization bar.

Corruption is represented within/against the Health bar rather than as a fourth bar.

## Health

- Reduced by physical/ranged damage as authored.
- Restored by checkpoint and active healing ability.
- Ritual success does not directly restore current Health.

## Corruption

- Reduces effective maximum Health.
- Added by ritual mistakes and selected attacks.
- Cleansed by successful ritual performance.
- Accumulates more aggressively than it cleanses.
- Checkpoints fully clear it.
- Full corruption causes encounter failure.

Corruption must not be implemented as ordinary Health damage.

## Shamanic Charge

- Continuous float-like resource.
- Active abilities consume charge.
- Checkpoints refill it.
- Successful ritual inputs add fractional charge.
- Exploration/power-ups may improve max capacity and/or gain rate.

## Ritual Composure

Hidden normalized ritual-state value.

Prototype:

```text
Hit:  MissStreak = 0; Composure += 0.015
Miss: MissStreak += 1; Composure -= 0.03 + MissStreak * 0.005
```

Clamp and final values are tuning decisions.

When Composure reaches its floor, ritual is forced to exit and cooldown begins.
