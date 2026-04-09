#!/usr/bin/env bash
set -euo pipefail

required_docs=(
  "doc/test_report_v1.md"
  "doc/test_report_v1_excel.csv"
  "doc/quality_factor_matrix_v1.md"
  "doc/Delta_Design_Implementation_Report.md"
  "doc/evidence_media_v1.md"
  "doc/traceability_matrix_v1.md"
  "doc/uml/class.puml"
  "doc/uml/component.puml"
  "doc/uml/sequence_frame.puml"
  "doc/uml/deployment.puml"
)

for doc in "${required_docs[@]}"; do
  if [[ ! -f "$doc" ]]; then
    echo "Missing required documentation artifact: $doc" >&2
    exit 1
  fi
done

report_ids="$(grep -Eo 'TC-[0-9]+' doc/test_report_v1.md | sort -u || true)"
trace_ids="$(grep -Eo 'TC-[0-9]+' doc/traceability_matrix_v1.md | sort -u || true)"

missing=0
while IFS= read -r id; do
  [[ -z "$id" ]] && continue
  if ! grep -qx "$id" <<<"$report_ids"; then
    echo "Traceability references test ID missing from report: $id" >&2
    missing=1
  fi
done <<<"$trace_ids"

if [[ "$missing" -ne 0 ]]; then
  exit 1
fi

echo "DOC_CHECKS_OK"
