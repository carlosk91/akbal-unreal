---
id: GOV-003
title: Naming and Syntax Conventions
status: approved
---

# Naming & Syntax Conventions — Akbal

Adapted from the supplied CuatroMachetes convention, Epic's Unreal coding standard, and Epic's asset naming recommendations.

## Golden rules

1. English only for code, comments, assets, canonical docs, branches, commits, and PR titles.
2. Consistency beats novelty.
3. No spaces or special characters in Unreal asset names.
4. C++ project prefix is the full word `Akbal` after the Unreal type prefix.
5. Names describe what an object **is**, not its temporary visual appearance or use.

## C++ naming

Examples:

- `AAkbalPlayerCharacter`
- `AAkbalAlebrijeCharacter`
- `UAkbalRitualComponent`
- `UAkbalMusicConductorSubsystem`
- `FAkbalSyncSection`
- `EAkbalAlebrijeArchetype`
- `IAkbalRitualParticipant`

Filenames omit only Unreal's type prefix:

- `AAkbalPlayerCharacter` -> `AkbalPlayerCharacter.h/.cpp`
- `UAkbalRitualComponent` -> `AkbalRitualComponent.h/.cpp`

Use Epic conventions for `A/U/F/E/I/T/S` prefixes and `b` booleans.

## Content root

All first-party Unreal content lives under:

```text
Content/_Akbal/
```

Third-party and template content must remain outside `_Akbal` unless intentionally migrated.

## Asset pattern

```text
Prefix_BaseName[_Descriptor][_Variant]
```

Common prefixes:

| Asset | Prefix | Example |
|---|---|---|
| Blueprint | `BP_` | `BP_Alebrije_Jaguar` |
| Actor Component | `AC_` | `AC_RitualFeedback` |
| Widget Blueprint | `WBP_` | `WBP_HUD` |
| Animation Blueprint | `ABP_` | `ABP_Player` |
| Animation Sequence | `AS_` | `AS_Jaguar_Charge` |
| Montage | `AM_` | `AM_Player_Dash` |
| Static Mesh | `SM_` | `SM_Shrine` |
| Skeletal Mesh | `SK_` | `SK_Alebrije_Jaguar` |
| Material | `M_` | `M_AlebrijePattern_Master` |
| Material Instance | `MI_` | `MI_Jaguar_Restored` |
| Texture | `T_` | `T_Jaguar_Pattern_BC` |
| Niagara System | `NS_` | `NS_RitualPulse` |
| MetaSound Source | `MS_` | `MS_Jaguar_Encounter` |
| MetaSound Patch | `MSP_` | `MSP_RitualChannelMixer` |
| Sound Wave | `SW_` | `SW_Jaguar_Percussion_S01` |
| Primary/Data Asset | `DA_` | `DA_Alebrije_Jaguar` |
| Data Table | `DT_` | `DT_RhythmJudgments` |
| Curve | `Curve_` | `Curve_HeartbeatRecovery` |
| Level/Map | `L_` | `L_ZoneA_01` |
| Input Action | `IA_` | `IA_Dash` |
| Input Mapping Context | `IMC_` | `IMC_Ritual` |

If Epic's currently recommended prefix differs from a legacy example, prefer the project convention already established in this file unless an ADR changes it.

## Audio naming

```text
SW_<Composition>_<Channel>_<Section>[_Variant]
MS_<Composition>_<Purpose>
```

Examples:

- `SW_Jaguar_Percussion_S01`
- `SW_Jaguar_LowPitch_S02`
- `MS_Jaguar_Encounter`
- `MS_ZoneA_RestorationMix`

Do not use the temporary fourth-instrument display name in stable identifiers. Use `HighRegister` until the instrument is finalized.

## Gameplay Tags

Top-level roots:

```text
Ability
Alebrije
Attack
Channel
Encounter
Event
Instrument
Music
Player
Ritual
State
World
```

Examples:

```text
Alebrije.Archetype.Pulse
Alebrije.Archetype.Fractured
Alebrije.Archetype.Resonant
Alebrije.Archetype.Fragmented
Attack.Delivery.Physical
Attack.Delivery.Ranged
Attack.Effect.Corruption
Instrument.Teponaztli
Instrument.Tololoche
Instrument.Cuatro
Instrument.HighRegister
Ritual.Path.Berserk
Ritual.Path.Soothing
State.Alebrije.Stunned
State.Player.RitualCooldown
```

## Canonical terminology

Prefer:

- Alebrije
- Restore / Restored
- Synchronization / Sync
- Corruption
- Stun
- Ritual
- Shamanic Charge
- Ritual Composure

Avoid canonical identifiers such as:

- `EnemyHealth`
- `DamageEnemy`
- `KillEnemy`
- `EnemyDeath`

## Blueprint

- Variables: `PascalCase`; booleans begin with `b`.
- Functions: verb-first `PascalCase`.
- Dispatchers: `On<Subject><Event>`.
- Keep Event Graphs small; move reusable rules to functions/components/C++.
- Blueprint child classes configure content; they do not redefine project-wide invariants.

## Folder names

Use `PascalCase`. Never create folders named `Misc`, `Stuff`, `New`, `Assets`, or `Temp` in production content.
