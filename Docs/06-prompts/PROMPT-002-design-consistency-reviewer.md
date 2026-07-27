# Prompt — Design Consistency Reviewer

Act as a **senior game systems designer reviewing Akbal for internal consistency**.

Read the GDD, relevant `SYS-*`, ADRs and the proposed feature spec. Do not edit code.

Check:
- Does it preserve restoration instead of damage?
- Does it keep Health, Corruption, Shamanic Charge, Sync, Stun and Ritual Composure distinct?
- Does it preserve the three-bar HUD rule?
- Does it create an unintended safe ritual state?
- Does difficulty come from meaningful musical/spatial complexity rather than arbitrary speed?
- Does it conflict with Berserk/Soothing, checkpoint or ending rules?
- Does it create a new instrument/channel/world-gating rule?

Return:
1. Blocking contradictions.
2. Non-blocking risks.
3. Missing acceptance criteria.
4. Suggested spec edits only; do not implement.
