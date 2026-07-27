# Akbal Glossary

| Term | Canonical meaning |
|---|---|
| **Alebrije** | A creature/entity the player restores. Not canonically called an enemy in design-facing language, although code may use generic Unreal AI concepts internally. |
| **Restoration** | Successful resolution of an alebrije encounter through synchronization. |
| **Synchronization / Sync** | Alebrije restoration progress shown in the segmented Sync bar. |
| **Sync Section** | Authored interval of the Sync bar associated with a ritual chart section. |
| **Temporary Sync** | Incomplete progress that passively drains when ritual ends. |
| **Solidified Sync** | A completed threshold, or a Soothing-created anchor, protected from passive drain but not ritual mistakes. |
| **Stun Counter** | Internal accumulated stun potential earned before ritual. Communicated visually through alebrije body patterns, not a dedicated bar. |
| **Guaranteed Stun** | Initial stun duration calculated from prepared stun counters. |
| **Heartbeat Recovery** | Probabilistic recovery checks after guaranteed stun expires; recovery probability rises on each heartbeat. |
| **Shamanic Charge** | Continuous player resource spent by active abilities and replenished at checkpoints and through successful ritual inputs. |
| **Corruption** | Player condition that reduces effective maximum health. Ritual mistakes and some attacks increase it; ritual success and checkpoints cleanse it. Full corruption causes failure. |
| **Ritual Composure** | Hidden ritual stability value. Successful inputs recover it; misses reduce it increasingly through `MissStreak`. Depletion forces ritual exit. |
| **Miss Streak** | Consecutive misses used to escalate Ritual Composure loss. Resets on a successful passing note. |
| **Chart Fidelity** | How closely required player inputs represent the audible musical events in a section. Higher difficulty generally increases fidelity/layering rather than only tempo. |
| **Ritual** | Rhythm interaction used to increase synchronization. It can overlap with active alebrije threats. |
| **Ritual Cooldown** | Period after certain ritual exits where ritual cannot be re-entered. Player-body patterns become opaque. |
| **Berserk** | Path ritual ability selected at game start. At boss encounter start, it pushes/stuns the boss and sustains stun while every required note remains Perfect. |
| **Soothing** | Alternative path ritual ability selected at game start. Solidifies current Sync, then ends ritual, removes stun, and starts ritual cooldown. |
| **Pulse** | Alebrije archetype based on stable/cyclic rhythm. |
| **Fractured** | Alebrije archetype representing one rhythmic identity that has broken apart. |
| **Resonant** | Alebrije archetype involving linked entities/layers and mutual influence. |
| **Fragmented** | Composite archetype/central-zone identity constructed from multiple rhythmic identities. |
| **Channel Archetype** | Broad playable register/role unlocked by an instrument: Percussion, Low Pitch, Medium Pitch, High Register. |
| **Normal Zone** | Outer exploration zone whose music is rebuilt by restoring resident alebrijes. |
| **Boss Zone** | Separate musical space associated with a boss; restoring it restores that zone and checkpoint but does not add its music to the normal-zone mix. |
| **Fragmented Center** | Central zone connected to all three outer zones; combines/breaks their musical identities. |
