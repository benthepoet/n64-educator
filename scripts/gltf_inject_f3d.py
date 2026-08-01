#!/usr/bin/env python3
"""
Inject Fast64-compatible f3d_mat extras into a glTF/GLB so Tiny3D's
gltf_to_t3d accepts it without the Fast64 Blender addon.

This is a course helper for beginners. Production art should still use Fast64
when possible (better materials, vertex-color blend presets, etc.).

Usage:
  python3 scripts/gltf_inject_f3d.py input.glb output.glb [--shade-only]
"""
from __future__ import annotations

import argparse
import json
import struct
import sys
from pathlib import Path

# Donor-style f3d_mat (shade/vertex color friendly, lighting on).
# Modeled after Tiny3D sample exports that gltf_to_t3d accepts.
F3D_MAT_SHADE = {
    "name": "",
    "presetName": "Custom",
    "combiner1": {
        "name": "",
        "A": 8,
        "B": 8,
        "C": 16,
        "D": 4,
        "A_alpha": 7,
        "B_alpha": 7,
        "C_alpha": 7,
        "D_alpha": 6,
    },
    "combiner2": {
        "name": "",
        "A": 8,
        "B": 8,
        "C": 16,
        "D": 4,
        "A_alpha": 7,
        "B_alpha": 7,
        "C_alpha": 7,
        "D_alpha": 6,
    },
    "tex0": {"tex_set": 0, "name": "", "tex": {}},
    "tex1": {"tex_set": 0, "name": "", "tex": {}},
    "set_prim": 0,
    "set_lights": 0,
    "set_env": 0,
    "set_blend": 0,
    "set_key": 1,
    "set_k0_5": 1,
    "set_combiner": 1,
    "use_default_lighting": 1,
    "blend_color": [0.0, 0.0, 0.0, 1.0],
    "prim_color": [1.0, 1.0, 1.0, 1.0],
    "env_color": [1.0, 1.0, 1.0, 1.0],
    "key_center": [1.0, 1.0, 1.0, 1.0],
    "key_scale": [0.0, 0.0, 0.0],
    "key_width": [0.0, 0.0, 0.0],
    "k0": 0.686274528503418,
    "k1": -0.16862745583057404,
    "k2": -0.3490196168422699,
    "k3": 0.8705882430076599,
    "k4": 0.4470588266849518,
    "k5": 0.16470588743686676,
    "prim_lod_frac": 0.0,
    "prim_lod_min": 0.0,
    "default_light_color": [1.0, 1.0, 1.0, 1.0],
    "ambient_light_color": [0.25, 0.25, 0.25, 1.0],
    "f3d_light1": None,
    "f3d_light2": None,
    "f3d_light3": None,
    "f3d_light4": None,
    "f3d_light5": None,
    "f3d_light6": None,
    "f3d_light7": None,
    "set_ao": 0,
    "set_fresnel": 0,
    "set_attroffs_st": 0,
    "set_attroffs_z": 0,
    "fog_color": [0.0, 0.0, 0.0, 1.0],
    "fog_position": [985, 1000],
    "set_fog": 0,
    "use_global_fog": 1,
    "rdp_settings": {
        "name": "",
        "g_zbuffer": 1,
        "g_shade": 1,
        "g_ambocclusion": 0,
        "g_attroffset_z_enable": 0,
        "g_attroffset_st_enable": 0,
        "g_cull_front": 0,
        "g_cull_back": 1,
        "g_packed_normals": 0,
        "g_lighttoalpha": 0,
        "g_lighting_specular": 0,
        "g_fresnel_color": 0,
        "g_fresnel_alpha": 0,
        "g_fog": 0,
        "g_lighting": 1,
        "g_tex_gen": 0,
        "g_tex_gen_linear": 0,
        "g_lod": 0,
        "g_shade_smooth": 1,
        "g_clipping": 0,
        "g_mdsft_alpha_dither": 2,
        "g_mdsft_rgb_dither": 0,
        "g_mdsft_combkey": 0,
        "g_mdsft_textconv": 2,
        "g_mdsft_text_filt": 2,
        "g_mdsft_textlut": 0,
        "g_mdsft_textlod": 0,
        "g_mdsft_textdetail": 0,
        "g_mdsft_textpersp": 1,
        "g_mdsft_cycletype": 0,
        "g_mdsft_color_dither": 1,
        "g_mdsft_pipeline": 1,
        "g_mdsft_alpha_compare": 0,
        "g_mdsft_zsrcsel": 0,
        "clip_ratio": 1,
        "set_rendermode": 1,
        "rendermode_advanced_enabled": 0,
        "rendermode_preset_cycle_1": 8,
        "rendermode_preset_cycle_2": 1,
        "aa_en": 0,
        "z_cmp": 0,
        "z_upd": 0,
        "im_rd": 0,
        "clr_on_cvg": 0,
        "cvg_dst": 0,
        "zmode": 0,
        "cvg_x_alpha": 0,
        "alpha_cvg_sel": 0,
        "force_bl": 0,
        "blend_p1": 3,
        "blend_p2": 3,
        "blend_m1": 0,
        "blend_m2": 0,
        "blend_a1": 2,
        "blend_a2": 2,
        "blend_b1": 0,
        "blend_b2": 0,
    },
    "draw_layer": {"sm64": 1, "oot": 0, "name": ""},
    "tex_scale": [1.0, 1.0],
}


def read_glb(path: Path):
    data = path.read_bytes()
    magic, version, length = struct.unpack_from("<4sII", data, 0)
    if magic != b"glTF":
        raise SystemExit(f"not a GLB: {path}")
    offset = 12
    chunks = []
    while offset < length:
        clen, ctype = struct.unpack_from("<I4s", data, offset)
        offset += 8
        chunks.append((ctype, data[offset : offset + clen]))
        offset += clen
    return version, chunks


def write_glb(path: Path, version: int, chunks):
    out = []
    for ctype, payload in chunks:
        pad = (4 - (len(payload) % 4)) % 4
        if pad:
            payload = payload + (b" " if ctype == b"JSON" else b"\x00") * pad
        out.append((ctype, payload))
    total = 12 + sum(8 + len(p) for _, p in out)
    with path.open("wb") as f:
        f.write(struct.pack("<4sII", b"glTF", version, total))
        for ctype, payload in out:
            f.write(struct.pack("<I4s", len(payload), ctype))
            f.write(payload)


def inject(gltf: dict) -> dict:
    mats = gltf.setdefault("materials", [])
    if not mats:
        mats.append({"name": "CourseDefault", "pbrMetallicRoughness": {}})
        gltf["materials"] = mats
        # assign material 0 to primitives missing one
        for mesh in gltf.get("meshes", []):
            for prim in mesh.get("primitives", []):
                prim.setdefault("material", 0)

    for mat in mats:
        extras = mat.setdefault("extras", {})
        # Don't clobber richer Fast64 exports
        if "f3d_mat" not in extras:
            extras["f3d_mat"] = json.loads(json.dumps(F3D_MAT_SHADE))
        extras.setdefault("rdp_settings", {})
        extras.setdefault("combiner1", {})
        extras.setdefault("tex0", {"tile_scroll": {}})
        extras.setdefault("UVanim", {"x": {}, "y": {}})
    return gltf


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("input")
    ap.add_argument("output")
    args = ap.parse_args()
    inp = Path(args.input)
    out = Path(args.output)

    if inp.suffix.lower() == ".glb":
        version, chunks = read_glb(inp)
        json_blob = None
        bin_blob = None
        for ctype, payload in chunks:
            if ctype == b"JSON":
                json_blob = payload
            else:
                bin_blob = payload
        gltf = json.loads(json_blob)
        inject(gltf)
        new_json = json.dumps(gltf, separators=(",", ":")).encode("utf-8")
        write_glb(out, version, [(b"JSON", new_json), (b"BIN\x00", bin_blob or b"")])
    else:
        gltf = json.loads(inp.read_text())
        inject(gltf)
        out.write_text(json.dumps(gltf, indent=2))
    print(f"wrote {out}")


if __name__ == "__main__":
    main()
