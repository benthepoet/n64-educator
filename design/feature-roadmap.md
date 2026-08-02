# N64 Educator — Feature Roadmap (Proposal)

Status: **proposal** · Audience: course author(s) · Source: curriculum gap review
against v1.1.9 (all L01–L36 lessons build and produce working ROMs).

This document lists candidate topics not currently covered by the curriculum,
ranked by learner impact. Each item suggests placement, scope, and rationale.
Nothing here is committed — treat it as a menu, not a mandate.

## Guiding constraints

- Keep the course's scope discipline: one shippable game (Starshard Cove),
  no assembly, no engine-building.
- New lessons must follow the existing pattern: buildable ROM under
  `lessons/`, docs page with VitePress `<<<` source imports, exercises.
- Prefer extending existing lesson ROMs over new standalone projects where
  the topic is small (rumble, debug logging).

---

## Priority 1 — Strong candidates

### 1. Save data / persistence

- **Gap:** Only mentioned as a capstone stretch goal ("EEPROM high score").
  No lesson covers `eepromfs`, raw EEPROM, SRAM, or Controller Pak (`mempak`).
- **Why it matters:** The course promise is "ship a real game." Persistence
  (high score, settings, unlock flags) is the feature students reach for first
  when building their own game after the capstone.
- **Suggested placement:** New lesson in Module 5 between L34 and L35
  (e.g. "Save & high scores"), persisting the best time rank in Toy Cove;
  capstone then saves the best Starshard Cove rank.
- **Scope notes:** `eepromfs` is the gentle on-ramp (file-like API on top of
  EEPROM). Cover save-type selection for flashcarts/emulators briefly.
  Controller Pak can be a stretch exercise.

### 2. Debugging workflow

- **Gap:** No lesson on `debugf()` / USB or emulator logging, libdragon's
  exception handler, assert habits, or reading a crash screen in Ares. The
  docs themselves show an assert failure (L32 audio sample rate) but never
  teach how to diagnose one.
- **Why it matters:** Beginners hit black screens and asserts constantly;
  right now the course gives them no systematic recourse.
- **Suggested placement:** Short lesson at the end of Module 0 or Module 2
  ("When it crashes"), plus a `reference/debugging.md` cheat sheet.
- **Scope notes:** `debugf` + `debug_init_usblogs`/emulator logging,
  deliberate-crash demo to read the exception handler output, common
  pitfalls table (null DMA, unaligned access, sample-rate assert, TMEM
  overflow symptoms).

### 3. Rumble / controller accessories

- **Gap:** `joypad_get_accessory_type` appears once as an optional stretch
  in L03; never taught.
- **Why it matters:** Rumble-on-collect is cheap, high-impact juice with a
  tiny API surface.
- **Suggested placement:** Extend L34 (Juice) — rumble pulse on shard
  pickup — or a short section within it. No new lesson needed.

### 4. Performance measurement

- **Gap:** L05 gives frame-budget intuition; L36 says "profile by feel
  first." Nobody teaches *measuring*: frame-time timing, tri counts,
  RSP/RDP load, overdraw.
- **Why it matters:** Makes the performance advice in L36 actionable and
  pre-answers the universal student question "why is my scene slow?"
- **Suggested placement:** Module 5, after L34 ("Measuring performance") or
  as a reference page plus exercises in L05/L36.
- **Scope notes:** `get_ticks`/`get_ticks_ms` frame-time overlay on the HUD,
  scene tri counting, simple A/B: heavy vs light scene. Keep it empirical —
  no RSP internals.

---

## Priority 2 — Moderate candidates

### 5. Memory management

- **Gap:** RDRAM sizes are named in L05, but nothing on heap discipline,
  `malloc` lifetime, asset load/free patterns, or the Expansion Pak
  (`is_memory_expanded`, 4 vs 8 MiB).
- **Suggested placement:** Short section in L05 or a reference page
  (`reference/memory.md`); exercises in a Module 5 lesson.
- **Scope notes:** Load-at-boot vs load-per-screen, why per-frame `malloc`
  is a bug, Expansion Pak detection one-liner.

### 6. Transparency, fog, and z-fighting

- **Gap:** Course renders are opaque vertex-color throughout. Students hit
  alpha/draw-order/fog questions immediately when authoring their own art.
- **Suggested placement:** Module 2 addendum lesson (after L16) or a
  "rendering gotchas" reference page.
- **Scope notes:** Alpha blending + draw order, `t3d_fog` for atmosphere
  (ties into "Night lighting" stretch goal), z-fighting causes and fixes.

### 7. Textured 3D in code

- **Gap:** L19 teaches UVs in Blender and L05 explains the TMEM budget, but
  no lesson draws a textured 3D model in Tiny3D — the course aesthetic is
  vertex-color end to end.
- **Note:** This may be a deliberate scope call. If so, close the loop
  explicitly: one short lesson ("swap the shard for a textured version")
  connecting L19 to runtime, covering CI4/CI8 formats and TMEM limits in
  practice. Otherwise state the vertex-color choice in L19 so students know
  textures-in-engine is left as self-study.

---

## Priority 3 — Minor / optional

### 8. Multiplayer input

- Everything is single-controller. A second joypad is nearly free to demo;
  a stretch exercise in L03 or L30 would suffice. Not a lesson.

### 9. Real-hardware testing

- L36 name-drops flashcarts. A half-page on EverDrive/SC64 gotchas (save
  types, USB debug capture, RTC) would strengthen the "ship it" moment.
  Pairs naturally with item 1 (save types differ per flashcart).

### 10. PAL/NTSC and VI modes

- Mentioned in passing in L05. Fine as-is unless resolution switching or
  PAL support becomes a course goal; then a reference page suffices.

---

## Suggested sequencing

If adopted in full, the least disruptive order:

1. **Rumble** (edit into L34 — no new files) and **debugging** (new short
   lesson + reference page) — low cost, immediate payoff.
2. **Persistence** (new Module 5 lesson + capstone high score) — biggest
   feature gap; also upgrades item 9 (flashcart save types).
3. **Performance measurement** (new short Module 5 lesson).
4. **Memory** (reference page + L05 section).
5. **Transparency/fog** and **textured 3D** (Module 2 addenda) — largest
   authoring effort; schedule last, or defer to a v1.3 "going further"
   module.

Numbering note: current lesson numbers run contiguously L01–L36 across
modules. New lessons can append (L37+) or renumber within modules —
appending avoids breaking existing links and the `lessons/` directory
names.
