---
id: GDD
title: Akbal Game Design Document
status: working
last_updated: 2026-07-27
---

# Akbal — Game Design Document

## 1. High concept

Akbal is an isometric single-player adventure where the player survives aggressive corrupted alebrijes and restores them through rhythm instead of conventional offensive damage. Encounters combine **bullet-hell spatial pressure**, **hack-and-slash responsiveness**, and **rhythm ritual performance**. Music is not a detached minigame layer: it defines encounter choreography, progression, world restoration, and creature identity.

## 2. Player fantasy

The player begins vulnerable and incomplete. Growth comes from communion with restored alebrijes, new instruments, exploration upgrades, and visible alebrije-like patterns accumulating on the player's body.

The player should feel:

- Exposed but capable.
- Attentive to sound and motion.
- Increasingly fluent in the forest's musical language.
- Responsible for restoration rather than domination.

The Berserk path deliberately tests the opposite fantasy: mastery through control.

## 3. Design pillars

1. **Restore, do not kill.** No conventional player damage loop against alebrijes.
2. **The world is music.** Attack patterns, movement, ritual charts, zones, and progression are musically authored.
3. **Ritual is dangerous.** Threats can continue while the player performs rhythm gameplay.
4. **Growth is embodied.** Instruments, body patterns, restored environments, music layers, and shortcuts show progression.
5. **Difficulty is layered.** Chart fidelity, polyrhythm, simultaneous attention, attack patterns, and environmental interference drive challenge.

## 4. Core loop

```text
Explore
  -> discover corrupted alebrijes / traversal gates / upgrades
  -> encounter begins and its music starts
  -> survive rhythmic bullet-hell pressure
  -> build stun potential through non-offensive defensive/active interactions
  -> enter ritual
  -> synchronize while managing threats, stun, corruption, Composure and cooldown
  -> restore alebrije
  -> environment/music/progression changes
  -> return later with new instruments to open missing interactions
```

## 5. Combat feel

Akbal borrows **responsiveness** from hack-and-slash games, not their damage model.

Player verbs include:

- Free movement and positioning.
- Dash with invincibility frames.
- Defensive posture with partial damage mitigation.
- Contextual cover/avoidance where level design supports it.
- Resonant Reflection against specifically authored compatible threats.
- Non-ritual active abilities powered by Shamanic Charge.
- Ritual entry/exit under pressure.

Alebrijes create bullet-hell pressure using attacks synchronized to the encounter composition. Physical attacks can interrupt ritual; ranged attacks normally do not.

## 6. Player resources

### 6.1 Health

Physical attacks and hazards reduce current health. Current health is restored by checkpoints and an active healing ability.

### 6.2 Corruption

Corruption reduces effective maximum health rather than directly dealing physical damage. Ritual mistakes and selected attacks can add corruption. Successful ritual performance cleanses corruption more slowly than mistakes add it. Full corruption causes encounter failure. Checkpoints fully cleanse corruption.

### 6.3 Shamanic Charge

Shamanic Charge is continuous. Active abilities spend charge. Checkpoints refill it, exploration can improve its capacity/economy, and successful ritual inputs restore fractional charge.

A baseline perfect section is intended to restore approximately one charge before upgrades. For a section with `N` required player inputs:

```text
NoteWeight = C / N
```

`C` is an authored/global constant still `TBD`. Required player inputs are not necessarily every audible note.

## 7. Ritual system

### 7.1 Entry and stun

Before ritual, the player can accumulate stun counters. The alebrije's visual pattern communicates accumulated stun potential.

When ritual begins, prepared stun counters determine initial guaranteed stun time. Re-entering ritual never refreshes stun duration.

When guaranteed stun expires, heartbeat checks begin. Each failed heartbeat increases the probability that the next heartbeat will end stun. Exact probability is hidden from the player; alebrije pattern behavior gives an approximate read of remaining stun safety.

Outside Berserk, completing any synchronization section immediately ends stun even if time remained.

### 7.2 Synchronization bar

The alebrije has one segmented Sync bar.

- Successful ritual play fills the current section.
- A completed section solidifies.
- If ritual ends with incomplete progress, that temporary progress drains to the last solidified anchor.
- Ritual mistakes actively drain Sync and can break solidified progress.
- If errors occur in Section N, progress may deplete Section N, then the previous section, down to the midpoint of Section N-1 before forced ritual exit.
- Soothing can create a solidified anchor at the current Sync value, but future mistakes can break through it.

### 7.3 Ritual Composure and misses

Ritual Composure is a hidden ritual stability value, not player health and not a HUD bar.

Prototype behavior:

```cpp
int32 MissStreak = 0;

Hit:
{
    MissStreak = 0;
    RitualComposure += 0.015f;
}

Miss:
{
    ++MissStreak;
    RitualComposure -= 0.03f;
    RitualComposure -= MissStreak * 0.005f;
}
```

Values are prototype tuning only. Misses also add corruption and reduce Sync through their own tuning rules. Composure reaching its floor forces ritual exit and cooldown.

### 7.4 Chart fidelity

Easy ritual sections abstract the audible music into fewer meaningful inputs. Harder sections require increasingly faithful representation of the music's notes, accents, subdivisions, rests, and channel changes.

## 8. Attack interaction

Attacks are authored from composable properties rather than a single rigid type.

Typical physical attack:

- Health damage: yes
- Knockback: yes
- Ritual interrupt: yes
- Corruption: optional

Typical ranged attack:

- Health damage/effect: authored
- Ritual interrupt: no by default
- Corruption: optional

Some alebrijes may have attacks whose primary consequence is corruption.

## 9. Encounter escalation

Synchronization represents restoration progress and escalation phase.

As Sync rises:

- The corruption reacts more aggressively.
- Alebrije spatial pressure increases.
- New/denser attack patterns may appear.
- Ritual chart fidelity/layering increases.
- Polyrhythm and channel switching may increase.
- Stun can become less reliable through tuning.

Increasing BPM is not the default difficulty lever.

## 10. Music and instruments

Each boss has a stable musical piece/identity for its encounter. Arrangement, stems, channels, processing, and complexity may change, but it remains the same composition.

Each boss represents a musical genre. Its movement, attacks, visual rhythm, and ritual vocabulary should express that genre.

Current channel progression:

1. **Teponaztli** — Percussion channels.
2. **Tololoche** — Low-pitch channels.
3. **Cuatro** — Medium-pitch channels.
4. **High-register multi-tonal instrument** — final identity `TBD`; do not encode the temporary death-whistle concept in stable technical identifiers.

New instruments gate alebrijes, paths, secrets, and content in Metroidvania fashion.

## 11. Abilities and power growth

There are four fixed ritual active abilities. The player does not build a ritual loadout.

1. Grace — tolerate limited note errors.
2. Resonance Window — expand timing tolerance.
3. Echo Assist — automatically perform briefly.
4. Path ability — **Berserk or Soothing**, chosen at the beginning of the playthrough.

A separate fixed set of non-ritual active abilities exists; the complete roster is `TBD`, with active healing confirmed.

Passive power-ups can improve both ritual and non-ritual play. Lower-tier upgrades may be broadly acquired; high-tier specialization branches may become mutually exclusive where defined by `SYS-009`.

## 12. Berserk and Soothing

### Berserk

- Can only activate at the beginning of a boss encounter.
- Pushes and stuns the boss.
- While every required note receives `Perfect`, the boss remains stunned indefinitely.
- Heartbeat recovery and section-completion stun break are suppressed while Berserk remains valid.
- Any non-Perfect judgment ends Berserk, applies increased corruption, and the boss recovers immediately.
- Pause is completely disabled for the duration of a boss encounter on a Berserk playthrough.

Ending consequences track whether every mandatory story boss, including mandatory Fragmented-center bosses, remained continuously stunned for its full restoration encounter.

### Soothing

- Can activate while ritual is active.
- Solidifies current Sync even between normal thresholds.
- Immediately ends ritual.
- Removes stun.
- Starts ritual cooldown.
- Adds no extra negative effect beyond standard ability cost and lost momentum.
- Future ritual mistakes may deplete through its anchor.
- Boss encounters may be paused normally on this path.

## 13. World structure

Three outer zones form a triangle. Every outer zone connects to both other outer zones and to the central Fragmented zone.

```text
        A
       /|\
      / | \
     B--+--C
      \ | /
       \|/
     Center
```

Outer-to-outer shortcuts combine both zones' genres.

Connections from an outer zone to the center use a broken/corrupted version of that outer zone's genre, foreshadowing fragmentation.

The central Fragmented zone combines and breaks the musical identities and gameplay lessons of all three outer zones.

## 14. Zone restoration music

Normal zones begin without a complete musical theme. Restoring resident alebrijes adds their musical channels to the zone arrangement. Alebrijes that require future instruments leave their channels missing until the player returns.

The bestiary doubles as a music mixer: per zone, the player can inspect alebrijes and hear/mute their individual contribution or hear the completed theme.

Boss-zone music is separate from the normal-zone mix but shares motifs, genre language, and thematic material. Restoring a boss restores its boss zone and turns the arena into a checkpoint without modifying the normal-zone stem collection.

## 15. Failure and checkpoints

Failure occurs when:

- Current Health reaches zero, or
- Corruption fills the available maximum-health capacity.

On failure:

- Respawn at latest checkpoint.
- Restore health.
- Clear corruption.
- Refill Shamanic Charge.
- Reset failed encounter Sync/stun/phase state.

A checkpoint exists before each boss. A restored boss arena becomes a permanent checkpoint and receives restored visual/audio treatment.

## 16. Endings

Primary ending classification uses mandatory story bosses only.

- **Communion**: Soothing path.
- **Dominion**: Berserk path and every mandatory boss was restored without ever regaining agency after Berserk began.
- **Internal Corruption**: Berserk path but at least one mandatory boss regained agency during its restoration encounter.

Optional bosses may modify completion, achievements, or ending details but do not determine the primary ending category.

## 17. Alebrije archetypes

- **Pulse** — cyclic/stable/drifting rhythmic identity.
- **Fractured** — one identity broken into irregular/silent/unstable patterns.
- **Resonant** — linked entities/layers whose rhythms affect one another.
- **Fragmented** — composite identity built from multiple previously learned types/genres.

## 18. Visual feedback

Alebrije body patterns communicate stun buildup and approximate remaining stun condition. Exact heartbeat probability remains hidden.

Player body patterns communicate growth and ritual availability:

- More permanent pattern coverage as power-ups are acquired.
- Emissive patterns when ritual is available/active.
- Opaque patterns during ritual cooldown.

## 19. Open design content

These are intentionally unresolved, not implementation licenses:

- Outer zone genres and boss identities.
- Final high-register instrument.
- Exact universal sources of stun counters.
- Full non-ritual active ability roster.
- Numeric tuning: charge constant `C`, corruption rates, Composure values, heartbeat curves, stun counter duration, judgment windows.
- Exact story/lore details and cultural-art direction boundaries.
