---
id: TECH-011
title: Build and Source Control
status: proposed
last_updated: 2026-08-10
---

# Build and Source Control

## Build

UnrealBuildTool is authoritative; IDE solution/project files are generated conveniences.

Use scripts that receive the Unreal installation path through environment/configuration rather than hard-coding one developer's machine path.

Recommended environment variable:

```text
UE_ROOT=<path to UE_5.7>
```

### Scripts (implemented)

| Script | Purpose |
|---|---|
| `Scripts/BuildEditor.ps1` | Build `AkbalEditor` Win64 Development |
| `Scripts/RunAutomationTests.ps1` | Headless `Automation RunTests` via `UnrealEditor-Cmd.exe` |

Examples:

```powershell
.\Scripts\BuildEditor.ps1
.\Scripts\RunAutomationTests.ps1 -Filter "Akbal" -BuildFirst
.\Scripts\RunAutomationTests.ps1 -Filter "Akbal.Rhythm"
```

Logs: `Saved/Logs/Akbal.log` (test summary parsed from this file).

## Git

Git LFS tracks binary Unreal packages. Use short branches and LFS locks for shared assets.

## CI

GitHub Free minutes/storage are limited. Initial CI should be narrow:

- Text validation/docs links if cheap.
- C++ build only when runners/toolchain are practical.
- Avoid downloading the full LFS corpus unnecessarily in every workflow.

A local pre-merge build/test routine is required even before CI is comprehensive. **Minimum pre-merge for timing changes:** `.\Scripts\RunAutomationTests.ps1 -Filter "Akbal" -BuildFirst`.
