# Builds the Akbal Editor target.
# Set UE_ROOT to your engine install, e.g. C:\Program Files\Epic Games\UE_5.7

param(
    [ValidateSet("Development", "Debug", "DebugGame", "Shipping")]
    [string]$Configuration = "Development"
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

$BuildBat = Join-Path $env:UE_ROOT "Engine\Build\BatchFiles\Build.bat"
if (-not (Test-Path $BuildBat)) {
    throw "Build.bat not found at $BuildBat"
}

& $BuildBat AkbalEditor Win64 $Configuration "-Project=$UProject" -WaitMutex
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host "AkbalEditor $Configuration build succeeded."
