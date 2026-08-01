# L24 — Rigging for beginners (Tiny3D limits)

::: tip Goal
Add a **simple armature**, bind weights, and respect Tiny3D’s skinning rules so export won’t produce spaghetti.
:::

## Tiny3D constraints (read twice)

From Tiny3D’s design (Module 2 era):

- **Fake blend**: effectively **1 bone per vertex** influence in the ucode model  
- Up to **3 bones** can affect a triangle’s vertices combined  
- Keep bone counts **low** (root, spine, head, limbs — not a film rig)  

::: warning Don’t copy a Mixamo 100-bone rig
It will not map cleanly. Use the course **reference animated character** (`player_anim` / snake) to study a working setup.
:::

## Minimal humanoid steps

1. Object Mode: Add → Armature.  
2. Edit bones: root at feet, spine up, simple arms/legs.  
3. Select mesh, then armature → Parent → Armature Deform → **With Automatic Weights**.  
4. Weight Paint: fix obvious errors (hand weighted to foot, etc.).  
5. Pose Mode: smoke-test rotate a bone.

## Course shortcut

Use `player_anim` (Quaternius / Tiny3D sample lineage, CC0 via Tiny3D examples) as the **known-good** skinned mesh while you practice on a simpler block character.

## Next

[L25 — Idle & walk](./l25-animation).
