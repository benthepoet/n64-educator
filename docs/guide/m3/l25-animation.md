# L25 — Idle & walk animation + export

::: tip Goal
Create **looping** idle and walk actions (or use the reference character) and play them in Tiny3D via named animations.
:::

## Concepts

| Term | Meaning |
|------|---------|
| **Action** | A clip (idle, walk) in Blender’s Dope Sheet / Action Editor |
| **Loop** | First and last frames match pose for seamless cycle |
| **In-place walk** | Character walks on a treadmill; game code moves root |

## Authoring tips

1. Idle: subtle breathing/sway, 20–40 frames.  
2. Walk: contact → passing → contact; even frame count helps.  
3. Name actions clearly: `Idle`, `Walk` (reference uses `Snake_Idle`, `Snake_Walk`).  
4. Export glTF with animations enabled + custom properties if Fast64.  

## Runtime (preview of Module 4)

```c
T3DSkeleton skel = t3d_skeleton_create_buffered(model, FB_COUNT);
T3DAnim idle = t3d_anim_create(model, "Snake_Idle");
t3d_anim_attach(&idle, &skel);
t3d_anim_update(&idle, dt);
t3d_skeleton_update(&skel);
t3d_skeleton_use(&skel);          /* required when bufferCount > 1 */
t3d_model_draw_skinned(model, &skel);
```

Asset lab: select `player_anim`, press **A** to blend toward walk.

## Exercises

1. In the lab, switch idle/walk.  
2. Open the reference `.blend` if provided in Tiny3D examples and inspect actions.  
3. (Stretch) Make a 2-bone lamp with a bob idle.

## Lesson code

Module 3 is Blender-first — there is no per-lesson C ROM for L17–L25.

Runnable code that loads the course meshes lives in the
[Module 3 asset lab checkpoint](./checkpoint) (`lessons/m3-asset-lab/`).

## Next

[Checkpoint — Asset lab](./checkpoint).
