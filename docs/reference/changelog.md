# Changelog

The **canonical** changelog is the repository root file:

**`CHANGELOG.md`** at the repository root (also shown on GitHub’s Releases when you tag).

It follows [Keep a Changelog](https://keepachangelog.com/) and
[Semantic Versioning](https://semver.org/).

## Current version

See the root **`VERSION`** file and `package.json` `"version"` field (they should match).

## Release process (maintainers)

**Every user-visible land on `master` is a versioned release** (patch, minor, or major).

1. Update `CHANGELOG.md`: move `[Unreleased]` notes into a new `## [X.Y.Z] — YYYY-MM-DD` section.  
2. Set the same version in `VERSION`, `package.json`, and the README “currently” line.  
3. Commit on **`master`**: `Release vX.Y.Z: …`.  
4. Tag: `git tag -a vX.Y.Z -m "vX.Y.Z"`.  
5. Push branch and tags: `git push origin master --tags`.  
6. Optionally create a GitHub Release from the tag.

Do **not** rewrite published changelog sections except for obvious typos.
Do **not** push curriculum changes to `master` without a version + changelog bump.
