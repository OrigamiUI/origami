$ErrorActionPreference = "Stop"

$requiredDocs = @(
    "doc/test_report_v1.md",
    "doc/test_report_v1_excel.csv",
    "doc/quality_factor_matrix_v1.md",
    "doc/Delta_Design_Implementation_Report.md",
    "doc/evidence_media_v1.md",
    "doc/traceability_matrix_v1.md",
    "doc/uml/class.puml",
    "doc/uml/component.puml",
    "doc/uml/sequence_frame.puml",
    "doc/uml/deployment.puml"
)

foreach ($doc in $requiredDocs) {
    if (-not (Test-Path -LiteralPath $doc)) {
        Write-Error "Missing required documentation artifact: $doc"
    }
}

$report = Get-Content -Raw "doc/test_report_v1.md"
$trace = Get-Content -Raw "doc/traceability_matrix_v1.md"

$reportIds = [regex]::Matches($report, "TC-\d+") |
    ForEach-Object { $_.Value } |
    Sort-Object -Unique

$traceIds = [regex]::Matches($trace, "TC-\d+") |
    ForEach-Object { $_.Value } |
    Sort-Object -Unique

$missing = $traceIds | Where-Object { $_ -notin $reportIds }
if ($missing.Count -gt 0) {
    Write-Error ("Traceability references test IDs missing from report: " +
        ($missing -join ", "))
}

Write-Output "DOC_CHECKS_OK"
