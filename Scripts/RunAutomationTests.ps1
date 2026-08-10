# Runs Akbal automation tests headlessly via UnrealEditor-Cmd.
# Examples:
#   .\RunAutomationTests.ps1
#   .\RunAutomationTests.ps1 -Filter "Akbal.UI"
#   .\RunAutomationTests.ps1 -Filter "Akbal.Rhythm" -BuildFirst

param(
    [string]$Filter = "Akbal",
    [switch]$BuildFirst
)

$ErrorActionPreference = "Stop"

$RepoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$UProject = Join-Path $RepoRoot "Akbal.uproject"

if (-not $env:UE_ROOT) {
    $DefaultUeRoot = "C:\Program Files\Epic Games\UE_5.7"
    if (Test-Path $DefaultUeRoot) {
        $env:UE_ROOT = $DefaultUeRoot
    }
}

if (-not $env:UE_ROOT) {
    throw "UE_ROOT is not set and the default UE 5.7 path was not found."
}

if ($BuildFirst) {
    & (Join-Path $PSScriptRoot "BuildEditor.ps1")
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

$EditorCmd = Join-Path $env:UE_ROOT "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
if (-not (Test-Path $EditorCmd)) {
    throw "UnrealEditor-Cmd.exe not found at $EditorCmd"
}

$LogFile = Join-Path $RepoRoot "Saved\Logs\AutomationTests-$Filter.log"
$LogDir = Split-Path $LogFile -Parent
if (-not (Test-Path $LogDir)) {
    New-Item -ItemType Directory -Path $LogDir | Out-Null
}

$FailCount = 0

$ExecCmd = "Automation RunTests $Filter; Quit"
Write-Host "Running: Automation RunTests $Filter"
Write-Host "(UnrealEditor-Cmd may take 30-90s with little console output. Chromium errors are harmless.)"

& $EditorCmd $UProject `
    -ExecCmds=$ExecCmd `
    -unattended `
    -nopause `
    -nosplash `
    -log `
    -ini:Engine:[/Script/EngineSettings.GameMapsSettings]:GameDefaultMap=/Engine/Maps/Entry.Entry `
    -ini:Engine:[/Script/EngineSettings.GameMapsSettings]:GlobalDefaultGameMode=/Script/Engine.GameModeBase

$ExitCode = $LASTEXITCODE
$LogFile = Join-Path $RepoRoot "Saved\Logs\Akbal.log"
Write-Host "Log: $LogFile"

if (Test-Path $LogFile) {
    $Failures = Select-String -Path $LogFile -Pattern "Test Completed\. Result=\{Fail\}" | ForEach-Object { $_.Line }
    $Successes = (Select-String -Path $LogFile -Pattern "Test Completed\. Result=\{Success\}" | Measure-Object).Count
    $FailCount = $Failures.Count

    if ($Successes -gt 0 -or $FailCount -gt 0) {
        Write-Host "--- Test summary: $Successes passed, $FailCount failed ---"
        if ($FailCount -gt 0) {
            $Failures | ForEach-Object { Write-Host $_ }
            Select-String -Path $LogFile -Pattern "LogAutomationController: Error:" | Select-Object -Last 15 | ForEach-Object { Write-Host $_.Line }
        }
    } else {
        Select-String -Path $LogFile -Pattern "LogAutomationCommandLine|Test Completed|tests completed" | Select-Object -Last 20 | ForEach-Object { Write-Host $_.Line }
    }
}

if ($FailCount -gt 0) {
    exit 1
}

exit $ExitCode
