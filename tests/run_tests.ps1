$ErrorActionPreference = "Stop"

$cc = if ($env:CC) { $env:CC } else { "gcc" }
$out = "tests/origami_tests.exe"

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

$suiteSources = Get-ChildItem -LiteralPath "tests" -Filter "test_*.c" |
    Sort-Object Name |
    ForEach-Object { $_.FullName }

$componentTestSources = Get-ChildItem -LiteralPath "tests/components" -Filter "test_*.c" |
    Sort-Object Name |
    ForEach-Object { $_.FullName }

$componentSources = Get-ChildItem -LiteralPath "src/components" -Filter "*.c" |
    ForEach-Object { $_.FullName }

$sources = $suiteSources + $componentTestSources + @(
    "src/origami.c"
) + $componentSources + @(
    "third_party/unity/src/unity.c"
)

Invoke-Checked {
    & $cc @sources `
    -Isrc `
    -Itests `
    -Ithird_party/unity/src `
    -std=c11 -Wall -Wextra -pedantic `
    -lm `
    -o $out
}

Invoke-Checked {
    & ".\$out"
}
