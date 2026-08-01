# Module 4 checkpoint — Toy Cove

::: tip Goal
Play a complete loop: title → explore island → collect 3 starshards → win → return to title.
:::

## Build & run

```bash
source scripts/env.sh
make -C lessons/m4-toy-cove
# → m4_toy_cove.z64
```

| Control | Action |
|---------|--------|
| START | Title→Play, Play⇄Pause, Win→Title |
| Stick | Move (camera-relative) |
| C-left/right | Orbit camera |

## Skills checklist

- [ ] Camera-relative move  
- [ ] Idle/walk blend  
- [ ] Follow camera with lag  
- [ ] Sphere collect  
- [ ] Entity/spawn mindset  
- [ ] State machine  

## Next

[Module 5 — Polish & Starshard Cove](/guide/m5/overview): audio, HUD, juice, then the full [8-shard capstone](/guide/m5/l35-capstone).
