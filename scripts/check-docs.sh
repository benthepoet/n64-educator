#!/usr/bin/env bash
# Build the VitePress site (catches broken config / missing pages linked from build).
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

if [[ ! -d node_modules ]]; then
  npm install
fi

npm run docs:build
echo "Docs build OK."
