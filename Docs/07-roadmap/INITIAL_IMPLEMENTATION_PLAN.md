# Initial Implementation Roadmap

Build a vertical technical slice before broad content production.

## Phase 0 — Repository bootstrap

1. Flatten the Unreal project to repository root.
2. Configure Git LFS and ignore rules.
3. Add `Public/Private` module structure.
4. Confirm UE 5.7.4 Editor build in Cursor/Rider environments.
5. Install/enable only plugins required by approved spikes.

## Phase 1 — Timing spike

Feature: `F001-rhythm-conductor`

- Validate Quartz/MetaSounds timing.
- Build musical clock/debug readout.
- Test input timestamp judgment at multiple frame rates.
- Validate pause/resume.

Do not build full boss AI before this question is stable.

## Phase 2 — Player resource slice

Feature: `F002-player-resources`

- Health.
- Corruption/effective max.
- Continuous Shamanic Charge.
- Checkpoint restore.
- Automated tests.

## Phase 3 — Ritual core

Feature: `F003-ritual-sync`

- Chart event model.
- Judgment.
- Sync sections/anchors/decay/regression.
- Ritual Composure/MissStreak.
- Cooldown.

## Phase 4 — Stun + heartbeat

Feature: `F004-stun-heartbeat`

- Stun counters.
- Initial guaranteed stun.
- Seeded escalating heartbeat recovery.
- Section completion break.
- Diegetic debug/pattern prototype.

## Phase 5 — Combat-feel sandbox

Feature: `F005-player-defense-and-attacks`

- Movement/dash/defensive posture.
- One physical rhythmic attack that interrupts ritual.
- One ranged rhythmic attack that does not.
- One corrupting attack.
- Bullet-hell scheduler/readability.

## Phase 6 — Path abilities

Feature: `F006-berserk-soothing`

Implement ending qualification flags only after the underlying ritual/stun behavior is stable.

## Phase 7 — Vertical-slice alebrije

One Pulse alebrije/boss with a temporary song and simplified art. It should prove the complete loop before producing three zones.

## Phase 8 — World restoration

Prototype normal-zone stem restoration, bestiary mixer and one instrument gate.

## Principle

Do not build generalized systems for all future content until the first complete encounter has proven the contracts. Refactor from evidence.
