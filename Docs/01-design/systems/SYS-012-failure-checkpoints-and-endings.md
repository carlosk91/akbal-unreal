---
id: SYS-012
title: Failure, Checkpoints and Endings
status: approved
---

# Failure, Checkpoints and Endings

## Failure conditions

- Current Health reaches zero.
- Corruption reaches the failure threshold/full effective-max-health loss.

## Failure reset

Respawn at latest checkpoint and:

- Restore Health.
- Clear Corruption.
- Refill Shamanic Charge.
- Reset failed encounter Sync.
- Reset stun/heartbeat state.
- Reset boss phase transitions and temporary hazards.

Permanent progression remains.

## Boss checkpoints

A checkpoint exists before each boss. After restoration, the boss arena becomes a restored permanent checkpoint.

## Ending classification

`RequiredBosses` includes mandatory bosses in outer zones and mandatory boss/bosses in the central Fragmented zone.

### Communion

Path is Soothing.

### Dominion

Path is Berserk and every required boss remained continuously without agency from Berserk activation through restoration.

### Internal Corruption

Path is Berserk and at least one required boss regained agency before restoration.

Optional bosses do not determine primary ending classification.
