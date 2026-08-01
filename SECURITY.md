# Security policy

N64 Educator is an **offline educational build system** for hobbyist homebrew.
It is not a networked service.

## Supported versions

The `master` branch is the only supported line for security-related fixes.
Published release tags (`vX.Y.Z`) may receive backports at maintainer discretion.

## Reporting a vulnerability

If you find a vulnerability in **our scripts** (e.g. unsafe handling of paths,
unexpected code execution when processing untrusted assets), please open a
GitHub issue titled `[SECURITY]` or contact the maintainers privately if
contact details are listed on the repository.

Please **do not** file public issues that include exploit chains against
third-party tools; report those upstream (libdragon, Tiny3D, Blender, etc.).

## Scope notes

- Opening untrusted `.blend`, `.glb`, or shell scripts from strangers can
  execute code inside those tools — treat assets like code.
- This project does not ship Nintendo proprietary SDKs or copyrighted game ROMs.
