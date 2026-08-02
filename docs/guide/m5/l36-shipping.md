# L36 — Shipping mindset & next steps

::: tip Goal
Know how to **finish**, test, credit third parties, and keep learning.
:::

## Pre-ship checklist

- [ ] Builds clean with `./scripts/build-all.sh`  
- [ ] Runs in **Ares** Homebrew mode  
- [ ] Title / play / pause / win all reachable  
- [ ] Audio not ear-splitting; music loops  
- [ ] No soft-lock if player falls (bounds / kill plane)  
- [ ] Credits: libdragon, Tiny3D, Quaternius (if using sample character), your name  
- [ ] `assets-src/ATTRIBUTION.md` updated for new art  

## Hardware (optional)

Flashcarts (SC64, EverDrive, 64drive) load the same `.z64`. Prefer USB loaders that support libdragon debug logging when hunting issues.

## Performance habits

- Conservative polycounts and texture sizes  
- Don’t thrash TMEM with huge unique maps  
- Measure frame ms ([L39](./l39-perf)); if under budget, stop optimizing  
- Profile by feel only after the meter looks fine  

## Persistence

- Best times / settings: [L38](./l38-save) + capstone EEPROM  
- Flashcart save type must match `N64_ROM_SAVETYPE`  

## Where to go next

- [N64brew Discord](https://discord.gg/WqFgNWf)  
- Tiny3D examples (particles, HDR, culling)  
- Fast64 deep materials  
- [Author feature roadmap](https://github.com/benthepoet/n64-educator/blob/master/design/feature-roadmap.md) (optional menu)  
- Your own sequel island  

## You shipped

If Starshard Cove runs end-to-end, you can honestly say you built a **3D N64 homebrew game** with authored art, animation, and audio using modern open tooling.

Congratulations.

## Related source

Shipping checklist — build and ship from [Starshard Cove](./starshard-cove) (`capstone/starshard-cove/`).
