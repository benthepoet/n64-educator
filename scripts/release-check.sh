#!/usr/bin/env bash
# Verify VERSION, package.json, and CHANGELOG agree before tagging a release.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

ver_file="$(tr -d '[:space:]' < VERSION)"
ver_pkg="$(node -p "require('./package.json').version")"

if [[ "$ver_file" != "$ver_pkg" ]]; then
  echo "error: VERSION ($ver_file) != package.json version ($ver_pkg)" >&2
  exit 1
fi

if ! grep -qE "^## \[${ver_file//./\\.}\]" CHANGELOG.md; then
  echo "error: CHANGELOG.md has no section ## [$ver_file]" >&2
  exit 1
fi

if [[ "$(git branch --show-current 2>/dev/null || true)" != "master" ]]; then
  echo "warning: current branch is not master (got: $(git branch --show-current 2>/dev/null || echo '?'))"
fi

echo "Release check OK: v$ver_file"
echo "Next: git tag -a v$ver_file -m \"v$ver_file\" && git push origin master --tags"
