# Pinned versions

Reproduce the course with these pins. Update this page when the curriculum intentionally moves forward.

## Verified pins (2026-08-01)

Installed and used to build L01/L02 successfully on Linux x86_64 (user-local prefix, no root).

| Component | Pin | Notes |
|-----------|-----|--------|
| GCC toolchain | continuous prerelease **14.4.0** | [toolchain-continuous-prerelease](https://github.com/DragonMinded/libdragon/releases/tag/toolchain-continuous-prerelease) `.deb` extracted to `$N64_INST` |
| libdragon | **`preview`** @ `25b0d25e08362733d3b5a928dbaad5c587f8b6f0` | Built with `make && make install` into `$N64_INST` |
| Tiny3D | **`main`** @ `e84172f29f719680ac3213a7f408c2f721ef7b24` | `./build.sh` installs `t3d.mk` + `libt3d.a` into `$N64_INST` |
| Host layout | `$HOME/n64-dev/` | See [Setup](/guide/setup) and `scripts/install-toolchain.sh` |

### Environment

```bash
source scripts/env.sh
# sets N64_INST=$HOME/n64-dev/toolchain
#     T3D_INST=$HOME/n64-dev/tiny3d
#     PATH includes $N64_INST/bin
```

## Art tools (course target)

| Component | Pin | Notes |
|-----------|-----|--------|
| Blender | **4.2 LTS** (or course-tested 4.x) | Confirm Fast64 + glTF export before bumping |
| Fast64 | Latest stable for course Blender | [Fast-64/fast64](https://github.com/Fast-64/fast64) |
| glTF export | Enable **custom properties** | Required for Fast64 → Tiny3D |

## Emulator

| Component | Pin |
|-----------|-----|
| Ares | Recent stable with **Homebrew mode** |

## How we advance pins

1. Run `./scripts/install-toolchain.sh` (or update checkouts) against candidates.
2. `source scripts/env.sh && ./scripts/build-all.sh`
3. Record SHAs in this table and in the install script comments if needed.
4. Note upgrade steps in the commit message.

## Course version vs toolchain pins

- **Course version** (`VERSION` / `package.json` / [Changelog](/reference/changelog)): curriculum revisions.
- **Toolchain pins** (this page): which libdragon/Tiny3D commits the course was verified against.

Bump the course version when pin changes require learner action (rebuild toolchain, broken APIs).

## Related links

- [libdragon wiki — Installing](https://github.com/DragonMinded/libdragon/wiki/Installing-libdragon)
- [libdragon preview notes](https://github.com/DragonMinded/libdragon/wiki/Preview-branch)
- [Tiny3D](https://github.com/HailToDodongo/tiny3d)
