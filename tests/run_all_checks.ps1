$ErrorActionPreference = "Stop"

function Invoke-Checked {
    param(
        [Parameter(Mandatory = $true)]
        [scriptblock]$Command
    )

    & $Command
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

Write-Output "[1/4] Unity unit + integration tests"
Invoke-Checked {
    powershell -ExecutionPolicy Bypass -File "tests/run_tests.ps1"
}

Write-Output "[2/4] Strict compile check (GCC)"
New-Item -ItemType Directory -Force "tests/.obj" | Out-Null
$sources = @("src/origami.c") + (Get-ChildItem "src/components" -Filter "*.c" |
    ForEach-Object { $_.FullName })
foreach ($s in $sources) {
    $name = [IO.Path]::GetFileNameWithoutExtension($s)
    Invoke-Checked {
        gcc -std=c11 -Wall -Wextra -pedantic -Isrc -c $s -o ("tests/.obj/" + $name + ".o")
    }
}
Write-Output "STRICT_COMPILE_OK"

Write-Output "[3/4] Documentation consistency checks"
Invoke-Checked {
    powershell -ExecutionPolicy Bypass -File "tests/run_doc_checks.ps1"
}

Write-Output "[4/4] Demo build smoke check"

$sdlFlags = @()
if (Test-Path "C:\msys64\mingw64\include\SDL2\SDL.h") {
    $sdlFlags += "-IC:/msys64/mingw64/include"
    $sdlFlags += "-IC:/msys64/mingw64/include/SDL2"
    $sdlFlags += "-LC:/msys64/mingw64/lib"
    $sdlFlags += "-lmingw32"
    $sdlFlags += "-lSDL2main"
    $sdlFlags += "-lSDL2"
}

Invoke-Checked {
    gcc demo/main.c demo/renderer.c src/origami.c src/components/*.c -Isrc -Wall -Wextra -pedantic -std=c11 @sdlFlags -lm -o tests/demo_smoke.exe
}
