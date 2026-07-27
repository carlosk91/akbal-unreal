---
id: GTD
title: Akbal Game Technical Design
status: working
last_updated: 2026-07-27
---

# Akbal — Game Technical Design

## 1. Technical goals

- Preserve musical timing accuracy under variable frame rate.
- Keep gameplay rules testable and data-driven.
- Let two developers work without creating unnecessary module or binary-asset contention.
- Keep Blueprint content authoring productive while C++ owns stable contracts.
- Make encounter state observable through debug tooling.
- Keep the repository compatible with GitHub Free / Git LFS constraints.

## 2. Repository layout

For a single Unreal project, place `Akbal.uproject` at repository root.

```text
akbal-unreal/
├── Akbal.uproject
├── Config/
├── Content/
│   └── _Akbal/
├── Source/
│   └── Akbal/
├── Docs/
├── Scripts/
├── Tests/                 # text fixtures/reference data if needed
├── SourceAssets/          # curated import sources only; LFS policy applies
├── AGENTS.md
├── README.md
├── CONTRIBUTING.md
├── .gitattributes
├── .gitignore
├── .editorconfig
├── .cursor/
└── .aiassistant/
```

Do not create multiple gameplay modules prematurely. Start with the single `Akbal` runtime module and domain folders. Add an `AkbalEditor` module only when editor-only C++ tools justify it.

## 3. Runtime module structure

```text
Source/Akbal/
├── Akbal.Build.cs
├── Public/
│   ├── Ability/
│   ├── AI/
│   ├── Audio/
│   ├── Encounter/
│   ├── Input/
│   ├── Player/
│   ├── Ritual/
│   ├── Save/
│   ├── World/
│   └── Akbal.h
└── Private/
    ├── Ability/
    ├── AI/
    ├── Audio/
    ├── Encounter/
    ├── Input/
    ├── Player/
    ├── Ritual/
    ├── Save/
    ├── Tests/
    ├── World/
    └── Akbal.cpp
```

Public headers are only for contracts truly used across subsystem/domain boundaries. Internal implementation details stay Private.

## 4. Content structure

```text
Content/_Akbal/
├── Core/
├── Player/
├── Alebrijes/
│   ├── Pulse/
│   ├── Fractured/
│   ├── Resonant/
│   └── Fragmented/
├── Encounters/
├── Instruments/
├── Music/
│   ├── Songs/
│   ├── Channels/
│   ├── ZoneMixes/
│   └── MetaSounds/
├── World/
│   ├── Zones/
│   ├── Checkpoints/
│   └── Props/
├── UI/
├── VFX/
├── Audio/
├── Data/
├── Maps/
└── Tests/
```

Feature folders may contain mixed asset types when they belong together; do not create global `Textures/`, `Meshes/`, etc. as the sole organizing strategy.

## 5. Runtime ownership proposal

### Player

`AAkbalPlayerCharacter` owns movement/avatar behavior and composes components rather than implementing every system directly.

Candidate components:

- `UAkbalHealthComponent`
- `UAkbalShamanicChargeComponent`
- `UAkbalRitualComponent`
- `UAkbalAbilityComponent`
- `UAkbalPlayerPatternComponent` (presentation-facing state bridge)

### Alebrije

`AAkbalAlebrijeCharacter` / appropriate Pawn base owns avatar/AI integration and composes:

- `UAkbalSynchronizationComponent`
- `UAkbalStunComponent`
- `UAkbalAttackComponent`
- `UAkbalAlebrijePatternComponent`

Exact Actor/Pawn/Character inheritance should be chosen by movement/animation requirements, not forced globally.

### Encounter

An encounter-scoped runtime owner coordinates:

- Encounter activation/resolution.
- Song/arrangement selection.
- Current boss/participants.
- Seeded random stream for reproducible heartbeat/attack behavior where needed.
- Escalation phase from Sync.
- Ending qualification flags.

Avoid storing encounter rules in Level Blueprint.

### Musical conductor

A dedicated conductor service exposes authoritative musical position and quantized events to gameplay systems. The current preferred direction is Quartz for sample-accurate scheduling plus MetaSounds for adaptive/layered rendering, validated by a spike.

## 6. State/data separation

Runtime mutable state belongs in components/encounter objects. Content definitions belong in Data Assets.

Likely Primary Data Asset types:

- Alebrije Definition
- Encounter Definition
- Song Definition
- Ritual Chart Definition
- Instrument Definition
- Ability Definition
- Zone Definition
- Attack Pattern Definition

Use soft references where they reduce loading coupling.

## 7. Gameplay Tags

Use tags for semantic state/events that cross systems; avoid replacing strongly typed local state with tags everywhere.

Examples:

- `State.Alebrije.Stunned`
- `State.Player.RitualCooldown`
- `Attack.Delivery.Physical`
- `Attack.Effect.Corruption`
- `Ritual.Path.Berserk`

## 8. Input

Use Enhanced Input with separate/stacked contexts where useful:

- Exploration/encounter.
- Ritual.
- UI.

Rhythm judgment must timestamp input against conductor time rather than using UI animation position as truth.

## 9. Audio timing

Prototype architecture:

```text
Quartz Clock / authoritative musical time
        |
        +--> Quantized beat/bar/section events
        +--> Encounter attack scheduler
        +--> Ritual chart evaluator
        +--> MetaSound parameters/stem transitions
        +--> Debug musical timeline
```

Technical spike must measure input-to-judgment consistency, game-thread notification jitter, audio output latency assumptions, pause behavior, and section transitions.

## 10. Randomness

Heartbeat recovery is probabilistic and increasing. Use an encounter-scoped seeded stream so automated tests and debug sessions can reproduce rolls.

Gameplay-visible randomness must not depend on global non-deterministic calls scattered across Blueprints.

## 11. Save state

Persist only durable progression:

- Checkpoint/respawn progression.
- Restored alebrijes/bosses.
- Instruments.
- Power-ups/upgrades.
- Shamanic Charge capacity upgrades.
- Opened shortcuts/gates.
- Bestiary discovery/restoration.
- Path selection (Berserk/Soothing).
- Boss ending qualification records.

Do not persist ephemeral encounter Sync/stun/composure state unless a future save-anywhere design requires it.

## 12. Debugging

Provide developer-only visualization for:

- Musical time, beat/bar/section.
- Input timestamps and judgments.
- Sync value, section, anchor, error floor.
- Corruption and effective max health.
- Shamanic Charge gain/spend.
- Ritual Composure and MissStreak.
- Stun counters, guaranteed stun time, heartbeat chance/index/roll.
- Attack event schedule.

Debugging values may be visible even when the shipping HUD intentionally hides them.

## 13. Testing

Use layered testing:

- Low-level/automation tests for formulas/state machines.
- Automation Specs for behavior contracts.
- Functional Tests for full encounter interactions in test maps.
- Manual rhythm/feel tests for latency and readability.

Timing systems should support test clocks or deterministic time injection where practical.

## 14. Editor tooling

Do not create an editor module on day one. Add `AkbalEditor` when repeated authoring pain justifies tools such as:

- Ritual chart validators.
- Song/section preview.
- Attack timeline validation.
- Asset naming/data validation.
- Zone restoration preview.

Editor-only dependencies must never leak into the runtime module.

## 15. Performance

Performance budgets remain `TBD`, but the project must measure rather than guess. Watch:

- Bullet count and collision cost.
- Niagara overdraw.
- Audio/MetaSound graph cost.
- Tick-heavy Blueprint logic.
- AI perception/behavior cost.
- Streaming and asset memory.

Avoid per-frame polling where events/quantized callbacks suffice.
