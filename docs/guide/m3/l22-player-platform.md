# L22 — Player blockout & platform piece

::: tip Goal
Block out a **readable player** and a **modular platform** without sculpting a masterpiece.
:::

## Player (static first)

1. Cylinder body + sphere head (or a single capsule).  
2. Slightly larger head = silhouette from behind (follow cam).  
3. Origin at feet.  
4. Apply transforms.  
5. Vertex color a friendly palette.  

Course file: `player_static`.

Animation comes in L24–L25; static is enough to place in a scene now.

## Platform

1. Flattened cube or cylinder.  
2. Origin center or top-center — pick one and stick to it.  
3. Roughly 2–5 units across vs player height ~2.  

## Check in asset lab

Rebuild props, open M3 lab, D-pad to `platform` / `player_static`.

## Next

[L23 — Level layout](./l23-level).
