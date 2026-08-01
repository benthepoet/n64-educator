import { defineConfig } from 'vitepress'
import { readFileSync } from 'node:fs'
import { fileURLToPath } from 'node:url'
import { dirname, join } from 'node:path'

const __dirname = dirname(fileURLToPath(import.meta.url))
const courseVersion = readFileSync(join(__dirname, '../../VERSION'), 'utf8').trim()

export default defineConfig({
  title: 'N64 Educator',
  description:
    'Learn 3D N64 game development with libdragon and Tiny3D — from zero to Starshard Cove.',
  cleanUrls: true,
  lastUpdated: true,

  themeConfig: {
    nav: [
      { text: 'Guide', link: '/guide/intro' },
      { text: 'Setup', link: '/guide/setup' },
      { text: 'Reference', link: '/reference/versions' },
      {
        text: `v${courseVersion}`,
        items: [
          { text: 'Changelog', link: '/reference/changelog' },
          { text: 'Pinned toolchain', link: '/reference/versions' },
          { text: 'License', link: '/reference/license' },
        ],
      },
    ],

    sidebar: {
      '/guide/': [
        {
          text: 'Start here',
          items: [
            { text: 'Introduction', link: '/guide/intro' },
            { text: 'Setup & toolchain', link: '/guide/setup' },
            { text: 'How this course works', link: '/guide/how-it-works' },
          ],
        },
        {
          text: 'Module 0 — Foundations',
          collapsed: true,
          items: [
            { text: 'L01 — Hello ROM', link: '/guide/m0/l01-hello-rom' },
            { text: 'L02 — Game loop & display', link: '/guide/m0/l02-game-loop' },
            { text: 'L03 — Controllers', link: '/guide/m0/l03-controllers' },
            { text: 'L04 — Assets on ROM (DFS)', link: '/guide/m0/l04-dfs' },
            { text: 'L05 — N64 hardware tour', link: '/guide/m0/l05-hardware' },
            { text: 'Checkpoint', link: '/guide/m0/checkpoint' },
          ],
        },
        {
          text: 'Module 1 — 3D Literacy',
          collapsed: true,
          items: [
            { text: 'Overview', link: '/guide/m1/overview' },
            { text: 'L06 — Vectors', link: '/guide/m1/l06-vectors' },
            { text: 'L07 — Matrices', link: '/guide/m1/l07-matrices' },
            { text: 'L08 — Camera', link: '/guide/m1/l08-camera' },
            { text: 'L09 — Meshes', link: '/guide/m1/l09-meshes' },
            { text: 'L10 — Color & light', link: '/guide/m1/l10-color-light' },
          ],
        },
        {
          text: 'Module 2 — Tiny3D First Light',
          collapsed: true,
          items: [
            { text: 'Overview', link: '/guide/m2/overview' },
            { text: 'L11 — First quad', link: '/guide/m2/l11-t3d-quad' },
            { text: 'L12 — Depth + objects', link: '/guide/m2/l12-depth-objects' },
            { text: 'L13 — Orbit camera', link: '/guide/m2/l13-orbit-camera' },
            { text: 'L14 — Lighting', link: '/guide/m2/l14-lighting' },
            { text: 'L15 — Load model', link: '/guide/m2/l15-load-model' },
            { text: 'L16 — Vertex color', link: '/guide/m2/l16-vertex-color' },
          ],
        },
        {
          text: 'Module 3 — Blender for N64',
          collapsed: true,
          items: [
            { text: 'Overview', link: '/guide/m3/overview' },
            { text: 'L17 — Survival kit', link: '/guide/m3/l17-survival' },
            { text: 'L18 — Starshard', link: '/guide/m3/l18-starshard' },
            { text: 'L19 — UVs & textures', link: '/guide/m3/l19-uvs' },
            { text: 'L20 — Vertex paint', link: '/guide/m3/l20-vertex-paint' },
            { text: 'L21 — Fast64 & export', link: '/guide/m3/l21-fast64-export' },
            { text: 'L22 — Player & platform', link: '/guide/m3/l22-player-platform' },
            { text: 'L23 — Level layout', link: '/guide/m3/l23-level' },
            { text: 'L24 — Rigging', link: '/guide/m3/l24-rigging' },
            { text: 'L25 — Idle & walk', link: '/guide/m3/l25-animation' },
            { text: 'Checkpoint — Asset lab', link: '/guide/m3/checkpoint' },
          ],
        },
        {
          text: 'Module 4 — Gameplay Systems',
          collapsed: true,
          items: [
            { text: 'Overview', link: '/guide/m4/overview' },
            { text: 'L26 — Move', link: '/guide/m4/l26-move' },
            { text: 'L27 — Anim drive', link: '/guide/m4/l27-anim-drive' },
            { text: 'L28 — Follow cam', link: '/guide/m4/l28-follow-cam' },
            { text: 'L29 — Collision', link: '/guide/m4/l29-collision' },
            { text: 'L30 — Entities', link: '/guide/m4/l30-entities' },
            { text: 'L31 — Game state', link: '/guide/m4/l31-game-state' },
            { text: 'Toy Cove', link: '/guide/m4/toy-cove' },
          ],
        },
        {
          text: 'Module 5 — Polish & Capstone',
          collapsed: false,
          items: [
            { text: 'Overview', link: '/guide/m5/overview' },
            { text: 'L32 — Audio', link: '/guide/m5/l32-audio' },
            { text: 'L33 — HUD', link: '/guide/m5/l33-hud' },
            { text: 'L34 — Juice', link: '/guide/m5/l34-juice' },
            { text: 'L35 — Capstone', link: '/guide/m5/l35-capstone' },
            { text: 'L36 — Shipping', link: '/guide/m5/l36-shipping' },
            { text: 'Starshard Cove', link: '/guide/m5/starshard-cove' },
          ],
        },
      ],
      '/reference/': [
        {
          text: 'Reference',
          items: [
            { text: 'Pinned versions', link: '/reference/versions' },
            { text: 'Changelog', link: '/reference/changelog' },
            { text: 'Glossary', link: '/reference/glossary' },
            { text: 'Scale & conventions', link: '/reference/conventions' },
            { text: 'License & credits', link: '/reference/license' },
          ],
        },
      ],
    },

    socialLinks: [],

    search: {
      provider: 'local',
    },

    footer: {
      message: `N64 Educator v${courseVersion} — libdragon + Tiny3D · branch master`,
      copyright: 'Course material © contributors. N64 is a trademark of Nintendo.',
    },
  },
})
