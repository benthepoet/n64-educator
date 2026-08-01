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
- Profile by feel first; then simplify art  

## Where to go next

- [N64brew Discord](https://discord.gg/WqFgNWf)  
- Tiny3D examples (particles, HDR, culling)  
- Fast64 deep materials  
- Your own sequel island  

## You shipped

If Starshard Cove runs end-to-end, you can honestly say you built a **3D N64 homebrew game** with authored art, animation, and audio using modern open tooling.

Congratulations.
