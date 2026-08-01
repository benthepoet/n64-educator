# Contributing to N64 Educator

Thanks for helping make N64 homebrew education better.

## Ways to contribute

- Fix typos or unclear steps in `docs/`
- Improve comments in lesson `main.c` files (aim for “noob guide” quality)
- Fix build issues on Linux / macOS / WSL
- Add missing troubleshooting tips
- Improve assets (with clear licensing — prefer CC0 / CC-BY)
- Fix bugs in Makefiles or scripts

## Ground rules

1. **Be kind.** This project exists for beginners. Assume good intent.
2. **Keep the spiral curriculum.** New material should build on earlier lessons, not dump advanced topics early.
3. **Comment for noobs.** If you change lesson code, update the teaching comments.
4. **Don’t break pins lightly.** Toolchain SHAs live in `docs/reference/versions.md`.
5. **License your work.** Code contributions are MIT; doc contributions CC BY 4.0 (see `LICENSE` / `LICENSE-DOCS`). Don’t submit content you can’t relicense this way.
6. **No copyrighted ROMs or proprietary Nintendo SDKs.**

## Development setup

```bash
./scripts/install-toolchain.sh   # or your existing N64_INST
source scripts/env.sh
npm install
npm run docs:dev                 # curriculum site
make l01                         # smoke-test a lesson
./scripts/build-all.sh           # all ROMs (takes a bit)
```

## Branching

- Default branch: **`master`**
- Open PRs against `master`
- Do not force-push `master` unless coordinated with maintainers

## Versioning & changelog

We use **Semantic Versioning** and [Keep a Changelog](https://keepachangelog.com/).

| File | Role |
|------|------|
| `VERSION` | Single-line course version (`X.Y.Z`) |
| `package.json` → `"version"` | Must match `VERSION` |
| `CHANGELOG.md` | Human-readable history |
| `README.md` | “currently **X.Y.Z**” line should match |

### Pull requests

When your PR is user-visible (docs, lessons, scripts):

1. Add a bullet under `## [Unreleased]` in `CHANGELOG.md` (`Added` / `Changed` / `Fixed` / `Removed`).  
2. Leave version numbers alone on the PR branch (maintainers bump on merge to `master`).

### Landing on `master` (maintainers)

**Every commit/push to `master` that ships user-visible work must adjust version
and changelog together:**

1. Choose SemVer bump (patch / minor / major — see table in `CHANGELOG.md`).  
2. Set the same version in `VERSION` and `package.json` (and README “currently”).  
3. Move `[Unreleased]` bullets into a new `## [X.Y.Z] — YYYY-MM-DD` section.  
4. Commit, tag `vX.Y.Z`, push `master` and tags.

Do not push docs/feature work to `master` with only an `[Unreleased]` note.

Release steps are also in `docs/reference/changelog.md`.

## Pull request checklist

- [ ] `./scripts/build-all.sh` passes (or explain why a lesson is skipped)
- [ ] `npm run docs:build` passes if you touched docs
- [ ] `CHANGELOG.md` `[Unreleased]` updated when the change is user-visible
- [ ] New assets listed in `assets-src/ATTRIBUTION.md` with licenses
- [ ] Lesson comments updated when behavior changes
- [ ] No secrets, no absolute home-directory paths in committed files

## Code style (light)

- Match existing C style in nearby lessons (libdragon / Tiny3D-friendly)
- Prefer clear names over clever macros
- Keep each lesson focused; put shared helpers in `common/`

## Reporting issues

Include:

- OS and how you installed the toolchain (`N64_INST`)
- libdragon / Tiny3D commits if known
- Full error log from `make`
- Whether Ares Homebrew mode is on (for runtime bugs)

## Security

This is an educational offline build system. Still: don’t open untrusted `.blend` / script payloads from strangers without review. Report tooling supply-chain concerns via GitHub issues.
