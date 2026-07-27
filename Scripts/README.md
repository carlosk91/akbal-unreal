# Scripts

Keep repeatable build/test commands here so humans and agents use the same workflow.

Recommended scripts to add after confirming local engine installation:

- `BuildEditor.ps1`
- `RunAutomationTests.ps1`
- `OpenEditor.ps1`
- `ValidateRepo.ps1`

Scripts should read Unreal location from an environment variable such as `UE_ROOT` rather than embedding `C:\Program Files\...` paths.
