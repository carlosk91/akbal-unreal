# Proposed Akbal Project Structure

## Current layout (as implemented)

Repository root **is** the Unreal project root (Phase 0 complete).

```text
repo/
├── Akbal.uproject
├── Config/
├── Content/
│   └── _Akbal/              # First-party assets (maps/MetaSounds TBD for F001)
├── Source/
│   ├── Akbal.Target.cs
│   ├── AkbalEditor.Target.cs
│   └── Akbal/
│       ├── Akbal.Build.cs
│       ├── Public/
│       │   ├── Audio/       # UAkbalMusicConductorSubsystem, musical position
│       │   ├── Input/       # Judgment, chart types, ritual config
│       │   ├── Spike/       # F001 spike game mode / PC / character
│       │   └── UI/
│       │       ├── Ritual/  # UAkbalRitualInputHud, session, viewport layout
│       │       └── Dev/     # UAkbalRhythmHarnessWidget
│       └── Private/
│           ├── Audio/
│           ├── Input/
│           ├── Spike/
│           ├── UI/Ritual/
│           ├── UI/Dev/
│           ├── UI/Spike/     # Legacy UAkbalRhythmSpikeWidget (deprecated)
│           └── Tests/       # Akbal.Rhythm.*, Akbal.UI.* automation tests
├── Docs/
├── Scripts/                 # BuildEditor.ps1, RunAutomationTests.ps1
├── SourceAssets/
├── AGENTS.md
└── README.md
```

Planned domain folders (`Ability/`, `AI/`, `Encounter/`, `Player/`, `Ritual/`, `Save/`, `World/`) remain valid targets for Phase 2+; they are not all populated yet.

## Recommended change from the original skeleton

Original:

```text
repo/
  .gitattributes
  .gitignore
  Akbal/
    Akbal.uproject
    Config/
    Source/
```

Recommended for this single-project repository:

```text
repo/
├── Akbal.uproject
├── Config/
├── Content/
│   └── _Akbal/
│       ├── Core/
│       ├── Player/
│       ├── Alebrijes/
│       │   ├── Pulse/
│       │   ├── Fractured/
│       │   ├── Resonant/
│       │   └── Fragmented/
│       ├── Encounters/
│       ├── Instruments/
│       ├── Music/
│       │   ├── Songs/
│       │   ├── Channels/
│       │   ├── ZoneMixes/
│       │   └── MetaSounds/
│       ├── World/
│       │   ├── Zones/
│       │   ├── Checkpoints/
│       │   └── Props/
│       ├── UI/
│       ├── VFX/
│       ├── Audio/
│       ├── Data/
│       ├── Maps/
│       └── Tests/
├── Source/
│   ├── Akbal.Target.cs
│   ├── AkbalEditor.Target.cs
│   └── Akbal/
│       ├── Akbal.Build.cs
│       ├── Public/
│       │   ├── Ability/
│       │   ├── AI/
│       │   ├── Audio/
│       │   ├── Encounter/
│       │   ├── Input/
│       │   ├── Player/
│       │   ├── Ritual/
│       │   ├── Save/
│       │   ├── World/
│       │   └── Akbal.h
│       └── Private/
│           ├── Ability/
│           ├── AI/
│           ├── Audio/
│           ├── Encounter/
│           ├── Input/
│           ├── Player/
│           ├── Ritual/
│           ├── Save/
│           ├── Tests/
│           ├── World/
│           └── Akbal.cpp
├── Docs/
├── Scripts/
├── SourceAssets/
├── AGENTS.md
├── README.md
├── CONTRIBUTING.md
├── .editorconfig
├── .gitattributes
├── .gitignore
├── .cursorignore
├── .aiignore
├── .cursor/
│   ├── rules/
│   └── skills/
└── .aiassistant/
    └── rules/
```

## Why flatten the current `Akbal/` directory

For one Unreal project per repository, repository root = Unreal project root reduces friction for:

- `AGENTS.md` and AI-rule discovery.
- Build scripts and CI paths.
- Git LFS patterns.
- Documentation references.
- Rider/Cursor workspace opening.

Keep the nested project only if you expect multiple Unreal projects in the same repository.

## Modules

Do **not** create many runtime modules now. A single `Akbal` module with domain folders is enough for two developers and an early codebase.

Add `AkbalEditor` only when editor-only C++ tooling is actually needed. Unreal's Editor target does not itself require a custom `AkbalEditor` module.

## Content organization

Organize first by game feature/domain, not exclusively by asset type. A boss's mesh, animation, Data Asset and VFX may live close to its feature content when that improves ownership and discoverability.

`_Akbal` protects first-party content from template/marketplace content and sorts it predictably.

## SourceAssets

Store only curated source files required to reproduce imports. Raw DAW sessions, takes, caches, videos and disposable exports should remain outside GitHub LFS unless the storage strategy changes.
