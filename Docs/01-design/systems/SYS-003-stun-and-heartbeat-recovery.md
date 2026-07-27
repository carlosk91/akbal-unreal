---
id: SYS-003
title: Stun and Heartbeat Recovery
status: approved
---

# Stun and Heartbeat Recovery

## Stun preparation

Stun counters are accumulated through approved non-offensive interactions. They are internal numeric state. Alebrije body patterns communicate buildup rather than a dedicated HUD bar.

## Ritual conversion

At ritual entry, prepared counters establish guaranteed stun duration according to alebrije/encounter tuning.

Re-entering ritual never restarts, refreshes, or extends remaining stun unless an explicit ability contract says otherwise; no current ability does.

## Heartbeat recovery

After guaranteed stun expires:

1. A heartbeat event occurs on authored musical timing.
2. A recovery roll is evaluated.
3. If recovery fails, the probability increases for the next heartbeat.
4. Eventually recovery must become guaranteed or reach a configured hard maximum heartbeat.

Exact probability is hidden from the player.

Debug/tests must expose:

- Current probability.
- Heartbeat index.
- Seed/roll.
- Recovery result.

## Stun termination

Outside Berserk, stun ends immediately when any synchronization section completes. Remaining stun time is discarded.

Stun may also end when heartbeat recovery succeeds or a specifically authored override occurs.

## Berserk exception

While Berserk Perfect-chain validity is maintained:

- Boss remains stunned.
- Heartbeat recovery is suspended.
- Section completion does not end stun.

Any non-Perfect required input ends Berserk and causes immediate boss recovery.
