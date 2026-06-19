param(
    [string]$Configuration = "Release",
    [string]$Platform = "x64"
)

$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent $PSScriptRoot
$solution = Join-Path $repoRoot "GDIProject\GDIProject.sln"
$projectRoot = Join-Path $repoRoot "GDIProject"
$distRoot = Join-Path $repoRoot "dist\GDIProject"

function Find-MSBuild {
    $pathCommand = Get-Command msbuild.exe -ErrorAction SilentlyContinue
    if ($pathCommand) {
        return $pathCommand.Source
    }

    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $found = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
        if ($found) {
            return $found
        }
    }

    throw "MSBuild.exe was not found. Install Visual Studio Build Tools with MSBuild."
}

$msbuild = Find-MSBuild
& $msbuild $solution /t:Games /p:Configuration=$Configuration /p:Platform=$Platform /m
if ($LASTEXITCODE -ne 0) {
    throw "MSBuild failed for $Configuration|$Platform with exit code $LASTEXITCODE."
}

if (Test-Path $distRoot) {
    Remove-Item -LiteralPath $distRoot -Recurse -Force
}
New-Item -ItemType Directory -Force $distRoot | Out-Null

$exe = Get-ChildItem -Path $projectRoot -Recurse -Filter "Games.exe" |
    Where-Object { $_.FullName -match "\\$Platform\\$Configuration\\" } |
    Select-Object -First 1

if (-not $exe) {
    throw "Could not find built Games.exe for $Configuration|$Platform."
}

Copy-Item -LiteralPath $exe.FullName -Destination (Join-Path $distRoot "Games.exe") -Force
Copy-Item -LiteralPath (Join-Path $projectRoot "Resource") -Destination (Join-Path $distRoot "Resource") -Recurse -Force

Write-Host "Release package created at $distRoot"
