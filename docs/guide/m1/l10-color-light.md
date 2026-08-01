# L10 — Color, lighting & vertex-color blending

::: tip Goal
Learn a **tiny** lighting vocabulary (**ambient**, **base color / albedo**) and the classic N64 trick: **multiply a texture (or base color) by vertex colors** so big surfaces get cheap variation without huge textures.
:::

## Why this lesson exists

By Starshard Cove you’ll want:

- Grass that isn’t one flat green stamp  
- Paths that look worn  
- Soft shadow-ish areas  

You *could* paint giant unique textures. On N64, **TMEM is only about 4 KiB** ([L05](/guide/m0/l05-hardware)). So artists combine:

1. A **small tiling** texture (or even a flat base color)  
2. **Vertex colors** painted on the mesh  
3. Hardware **multiply** (“modulate”)  

L10 shows the *idea* with two triangles side by side — no Blender yet.

## What you will see (ROM)

```bash
source scripts/env.sh
make -C lessons/l10-color-light
```

| Side | Story |
|------|--------|
| **Left** | One green “material,” only overall brightness changes |
| **Right** | Same green idea, but **each corner has a tint** — looks blotchy/painted on purpose |

| Input | Effect |
|-------|--------|
| Stick Y | **Ambient** brightness (brighter / darker overall) |

Push stick up/down: both sides get lighter/darker, but the **right** side keeps its spatial pattern. That’s “lighting × paint.”

---

## Terms in plain English

| Term | Plain English | Kitchen metaphor |
|------|----------------|------------------|
| **Albedo / base color** | The surface’s own color if light is neutral | The color of the frosting |
| **Ambient** | Fill light so caves aren’t pure black | Room light so you see *something* everywhere |
| **Directional light** | Sun/moon from a direction; needs **normals** | A window casting light from one side |
| **Vertex color** | Color stored at corners, blended across faces | Food coloring drops that blend into frosting |
| **Modulate / multiply** | `result = base * vertex * light…` | Tinting frosting by mixing (dark × color = darker color) |

::: tip Multiply intuition
In 0–1 math, white vertex color `(1,1,1)` leaves the base unchanged. Gray `(0.5,0.5,0.5)` darkens. Black zeros it out. That’s why vertex paint is great for dirt and fake AO.
:::

---

## Left triangle vs right triangle

### Left — “flat × ambient”

Every corner uses the same base green × ambient. Looks like a sticky note under room light. Fine for UI panels; boring for terrain.

### Right — “base × vertex tint × ambient”

Corners use different tints (bright / muddy / medium). Same “grass” identity, spatial variation for free. **This** is the N64 terrain mindset.

We fake it by baking the multiply into the vertex RGBA we send to the RDP. In Tiny3D + Fast64, the **color combiner** does this for real with textures.

---

## Lighting equation (friendly form)

You may see:

```text
lit = ambient + light_color * max(0, dot(normal, light_dir))
out = albedo * lit * vertex_color
```

| Piece | Need now? |
|-------|-----------|
| ambient | Yes — stick Y in the ROM |
| albedo | Yes — our green base |
| vertex_color | Yes — right triangle |
| directional + normals | Module 2 |

If `dot(normal, light_dir)` makes your eyes glaze over: it only means “how much does this face point toward the sun?” Faces toward the sun get brighter. You’ll *see* it when Tiny3D lights a spinning mesh.

---

## Why N64 loves vertex color (again, with budget)

```text
Small tiling grass texture
        ×
Vertex paint (path brown, shade blue, dry yellow)
        =
Big readable landscape without huge unique textures
```

Module 3: paint `Col` in Blender + Fast64.  
Module 2: draw meshes where that data survives the pipeline.

Write this in your notes if nothing else:

::: tip Starshard Cove art rule of thumb
**Terrain:** tiling texture × vertex colors.  
**Starshards:** bright / special material so pickups pop.  
**Player:** readable colors from behind the camera.
:::

---

## What this ROM is *not*

- Not real textured sampling yet  
- Not real directional lights / normals yet  
- Not the final combiner setup  

It’s a **concept sculpture** so Module 2–3 don’t introduce five ideas at once.

---

## Common noob confusions

| Feeling | Reality |
|---------|---------|
| “Vertex color replaces textures” | Often **multiplies** with them |
| “Ambient is a hack” | Games still use ambient or GI approximations; ambient is the simple cousin |
| “Left and right should match” | They match only if vertex tints are all 1 |
| “I’ll paint this in Photoshop on a 1024 texture” | You *can* on PC; on N64 you’ll hurt TMEM — prefer vertex paint for large-scale tints |

---

## Exercises

1. Stick Y all the way both directions — describe ambient in your own words.  
2. In source, set all right-side tints to `1.0` — sides should look much more alike.  
3. Change base RGB to sand; adjust tints so a “path” still reads.  
4. Answer without notes: why not a unique 256×256 texture for every path?  


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l10-color-light/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l10-color-light/Makefile) · [`lessons/l10-color-light/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l10-color-light/src/main.c)

::: details `lessons/l10-color-light/Makefile`
<<< ../../../lessons/l10-color-light/Makefile{make}
:::

::: details `lessons/l10-color-light/src/main.c`
<<< ../../../lessons/l10-color-light/src/main.c{c}
:::

## What you learned

- Ambient vs base color  
- Vertex color as spatial tint  
- Multiply/modulate as the N64 workhorse  
- Bridge to Tiny3D lights/textures and Blender paint  

---

## Module 1 checkpoint (self-test)

Try without scrolling up:

1. **Point vs vector** — one sentence each.  
2. In our `M = T × R × S`, what happens to the point **first**, scale or translate?  
3. If the camera moves left, which way do world props appear to slide (and why)?  
4. Name **three** things a vertex might store.  
5. Why multiply textures (or albedo) by vertex color on N64?  

### Comfortable answers (peek after trying)

1. Point = location; vector = displacement/direction+length.  
2. **Scale** first (right-to-left on the point).  
3. Appear to slide **right** — view uses world − camera (museum story).  
4. e.g. position, color, normal, UV.  
5. Cheap large-scale variation under **tiny TMEM** / texture budgets.  

If you mostly got those, you’re ready for [Module 2 — Tiny3D](/guide/m2/overview). If not, re-run the ROMs for the weak spot — thumbs beat rereading alone.

## Next

[Module 2 — Tiny3D first light](/guide/m2/overview): same vocabulary on the real 3D pipeline (viewport, depth, lights, loaded meshes, vertex-color terrain).
