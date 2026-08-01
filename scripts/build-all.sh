#!/usr/bin/env bash
# Build every lesson (and later capstone) that has a Makefile.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

if [[ -z "${N64_INST:-}" ]]; then
  echo "error: N64_INST is not set. See docs/guide/setup.md" >&2
  exit 1
fi

shopt -s nullglob
failed=0
built=0

for mk in lessons/*/Makefile capstone/*/Makefile; do
  dir="$(dirname "$mk")"
  echo "==> Building $dir"
  if make -C "$dir"; then
    built=$((built + 1))
  else
    echo "error: build failed in $dir" >&2
    failed=$((failed + 1))
  fi
done

echo "Built $built project(s); $failed failure(s)."
exit "$failed"
