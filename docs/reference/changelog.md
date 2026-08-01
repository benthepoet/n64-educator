# Changelog

The **canonical** changelog is the repository root file:

**`CHANGELOG.md`** at the repository root (also shown on GitHub’s Releases when you tag).

It follows [Keep a Changelog](https://keepachangelog.com/) and
[Semantic Versioning](https://semver.org/).

## Current version

See the root **`VERSION`** file and `package.json` `"version"` field (they should match).

## Release process (maintainers)

1. Update `CHANGELOG.md`: move `[Unreleased]` notes into a new `## [X.Y.Z] — YYYY-MM-DD` section.  
2. Set the same version in `VERSION` and `package.json`.  
3. Commit on **`master`**: `Release vX.Y.Z`.  
4. Tag: `git tag -a vX.Y.Z -m "vX.Y.Z"`.  
5. Push branch and tags: `git push origin master --tags`.  
6. Optionally create a GitHub Release from the tag.

Do **not** rewrite published changelog sections except for obvious typos.
