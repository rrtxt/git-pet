<#
.SYNOPSIS
    git-pet installer for Windows.

.DESCRIPTION
    Installs git-pet.exe (with its bundled DLLs) and the default pet assets,
    then sets the GIT_PET_PATH user environment variable and adds git-pet to
    the user PATH.

    Installs from this folder if it contains git-pet.exe (an extracted release
    package), otherwise downloads the latest GitHub release.

.PARAMETER Uninstall
    Remove git-pet, its assets and the environment variables it set.

.PARAMETER InstallDir
    Install root. Default: %LOCALAPPDATA%\Programs\git-pet
      Binary -> <InstallDir>\git-pet.exe
      Assets -> <InstallDir>\pets\<pet-id>\

.EXAMPLE
    .\install.ps1
.EXAMPLE
    .\install.ps1 -Uninstall
.EXAMPLE
    irm https://raw.githubusercontent.com/rrtxt/git-pet/main/install.ps1 | iex
#>
[CmdletBinding()]
param(
    [switch]$Uninstall,
    [string]$InstallDir = (Join-Path $env:LOCALAPPDATA 'Programs\git-pet')
)

$ErrorActionPreference = 'Stop'

$Repo    = 'rrtxt/git-pet'
$PetId   = 'cat'
$PetsDir = Join-Path $InstallDir 'pets'
$Stages  = @('egg', 'baby', 'teen', 'adult')

function Write-Info($msg) { Write-Host '==> ' -ForegroundColor Blue -NoNewline; Write-Host $msg }
function Write-Warn($msg) { Write-Host 'warning: ' -ForegroundColor Yellow -NoNewline; Write-Host $msg }

function Get-UserPath {
    $p = [Environment]::GetEnvironmentVariable('Path', 'User')
    if ($null -eq $p) { return @() }
    return @($p -split ';' | Where-Object { $_ -ne '' })
}

function Set-UserPath([string[]]$entries) {
    [Environment]::SetEnvironmentVariable('Path', ($entries -join ';'), 'User')
}

# Find a directory containing git-pet.exe + assets (an extracted release package).
function Find-LocalSource {
    $scriptDir = $PSScriptRoot
    if ($scriptDir -and (Test-Path (Join-Path $scriptDir 'git-pet.exe')) -and
        (Test-Path (Join-Path $scriptDir 'assets'))) {
        return $scriptDir
    }
    return $null
}

function Get-Release([string]$tmp) {
    if (-not [Environment]::Is64BitOperatingSystem) {
        throw 'No prebuilt package for 32-bit Windows. Build from source (see README.md).'
    }
    $url = "https://github.com/$Repo/releases/latest/download/git-pet-windows-x86_64.zip"
    $zip = Join-Path $tmp 'git-pet.zip'
    Write-Info "Downloading $url"
    [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
    $ProgressPreference = 'SilentlyContinue'  # makes Invoke-WebRequest much faster on PS 5.1
    Invoke-WebRequest -Uri $url -OutFile $zip -UseBasicParsing
    Expand-Archive -Path $zip -DestinationPath $tmp -Force
    return (Join-Path $tmp 'git-pet')
}

function Install-GitPet {
    $tmp = $null
    $src = Find-LocalSource
    if ($src) {
        Write-Info "Installing from $src"
    } else {
        $tmp = Join-Path ([IO.Path]::GetTempPath()) ("git-pet-" + [Guid]::NewGuid())
        New-Item -ItemType Directory -Path $tmp | Out-Null
        $src = Get-Release $tmp
    }

    try {
        # Binary and the MinGW DLLs it needs (they must sit next to the exe)
        New-Item -ItemType Directory -Force -Path $InstallDir | Out-Null
        Copy-Item (Join-Path $src 'git-pet.exe') $InstallDir -Force
        Get-ChildItem -Path $src -Filter '*.dll' | Copy-Item -Destination $InstallDir -Force
        Write-Info "Installed binary to $InstallDir\git-pet.exe"

        # Assets: GIT_PET_PATH\<pet-id>\{pet.toml,egg\,baby\,...}
        $petDir = Join-Path $PetsDir $PetId
        New-Item -ItemType Directory -Force -Path $petDir | Out-Null
        foreach ($stage in $Stages) {
            $stageSrc = Join-Path $src "assets\$stage"
            if (Test-Path $stageSrc) {
                $stageDst = Join-Path $petDir $stage
                if (Test-Path $stageDst) { Remove-Item $stageDst -Recurse -Force }
                Copy-Item $stageSrc $stageDst -Recurse
            }
        }

        # Keep a user-edited pet.toml; only write the default if missing.
        $petToml = Join-Path $petDir 'pet.toml'
        $srcToml = Join-Path $src 'assets\pet.toml'
        if (-not (Test-Path $petToml)) {
            if (Test-Path $srcToml) {
                Copy-Item $srcToml $petToml
            } else {
                $toml = @'
name = "Cat"
stages = ["egg", "baby", "teen", "adult"]

[egg]
fps = 5

[baby]
fps = 8

[teen]
fps = 10

[adult]
fps = 10
'@
                # Write UTF-8 without BOM so the TOML parser is happy
                [IO.File]::WriteAllText($petToml, $toml, (New-Object Text.UTF8Encoding $false))
            }
        }
        Write-Info "Installed pet assets to $petDir"
    } finally {
        if ($tmp) { Remove-Item $tmp -Recurse -Force -ErrorAction SilentlyContinue }
    }

    # Environment: GIT_PET_PATH + PATH (user scope, persists across sessions)
    $oldPetPath = [Environment]::GetEnvironmentVariable('GIT_PET_PATH', 'User')
    if ($oldPetPath -and $oldPetPath -ne $PetsDir) {
        Write-Warn "Replacing existing GIT_PET_PATH ($oldPetPath). Copy any custom pets into $PetsDir."
    }
    [Environment]::SetEnvironmentVariable('GIT_PET_PATH', $PetsDir, 'User')
    $env:GIT_PET_PATH = $PetsDir
    Write-Info "Set GIT_PET_PATH=$PetsDir"

    $userPath = Get-UserPath
    if ($userPath -notcontains $InstallDir) {
        Set-UserPath ($userPath + $InstallDir)
        Write-Info "Added $InstallDir to your PATH"
    }
    if (($env:Path -split ';') -notcontains $InstallDir) {
        $env:Path = "$env:Path;$InstallDir"
    }

    Write-Host ''
    Write-Info 'Done! Open a new terminal and run ''git-pet'' inside any git repository.'
}

function Uninstall-GitPet {
    if (Test-Path $InstallDir) { Remove-Item $InstallDir -Recurse -Force }
    [Environment]::SetEnvironmentVariable('GIT_PET_PATH', $null, 'User')
    Set-UserPath (Get-UserPath | Where-Object { $_ -ne $InstallDir })
    Write-Info 'git-pet uninstalled. Open a new terminal to clear GIT_PET_PATH.'
}

if ($Uninstall) { Uninstall-GitPet } else { Install-GitPet }
