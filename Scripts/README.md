# Scripts

Repeatable build and test commands for humans and agents. Set `UE_ROOT` to your engine install (for example `C:\Program Files\Epic Games\UE_5.7`).

## Commands

```powershell
# Build editor target
.\Scripts\BuildEditor.ps1

# Run all Akbal automation tests (build first if needed)
.\Scripts\RunAutomationTests.ps1 -BuildFirst

# Run a subset
.\Scripts\RunAutomationTests.ps1 -Filter "Akbal.UI"
.\Scripts\RunAutomationTests.ps1 -Filter "Akbal.Rhythm"
```

Logs are written to `Saved/Logs/AutomationTests-<Filter>.log`.

## Test groups

| Filter | What it covers |
|---|---|
| `Akbal.Rhythm` | Judgment math, chart builder, note preview |
| `Akbal.UI` | Harness/HUD widget smoke tests, viewport layout, config defaults |

## Recommended workflow

1. Change code.
2. `.\Scripts\BuildEditor.ps1`
3. `.\Scripts\RunAutomationTests.ps1 -Filter "Akbal"`
4. Manual PIE only for feel/timing checks not yet automated.
