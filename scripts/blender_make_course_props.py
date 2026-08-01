"""
Blender batch script: create simple Module 3 teaching props.
Run:
  blender -b -P scripts/blender_make_course_props.py -- \\
    --out-dir assets-src/blender/generated
"""
from __future__ import annotations

import argparse
import math
import sys
from pathlib import Path

import bpy


def parse_args():
    argv = sys.argv
    if "--" in argv:
        argv = argv[argv.index("--") + 1 :]
    else:
        argv = []
    ap = argparse.ArgumentParser()
    ap.add_argument("--out-dir", required=True)
    return ap.parse_args(argv)


def reset_scene():
    bpy.ops.wm.read_factory_settings(use_empty=True)


def ensure_object_mode():
    if bpy.context.mode != "OBJECT":
        bpy.ops.object.mode_set(mode="OBJECT")


def link_vertex_color_material(obj, attr_name="Col"):
    """Make glTF export include COLOR_0 by using the attribute in the material graph."""
    mat = bpy.data.materials.new(obj.name + "Mat")
    mat.use_nodes = True
    nodes = mat.node_tree.nodes
    links = mat.node_tree.links
    nodes.clear()
    out = nodes.new("ShaderNodeOutputMaterial")
    bsdf = nodes.new("ShaderNodeBsdfPrincipled")
    vcol = nodes.new("ShaderNodeVertexColor")
    vcol.layer_name = attr_name
    links.new(vcol.outputs["Color"], bsdf.inputs["Base Color"])
    links.new(bsdf.outputs["BSDF"], out.inputs["Surface"])
    if obj.data.materials:
        obj.data.materials[0] = mat
    else:
        obj.data.materials.append(mat)


def add_vertex_colors(obj, color_rgba, attr_name="Col"):
    mesh = obj.data
    # Remove existing
    while mesh.color_attributes:
        mesh.color_attributes.remove(mesh.color_attributes[0])
    attr = mesh.color_attributes.new(name=attr_name, type="BYTE_COLOR", domain="CORNER")
    r, g, b, a = color_rgba
    for i in range(len(attr.data)):
        attr.data[i].color = (r, g, b, a)
    link_vertex_color_material(obj, attr_name)


def paint_plane_gradient(obj, attr_name="Col"):
    mesh = obj.data
    while mesh.color_attributes:
        mesh.color_attributes.remove(mesh.color_attributes[0])
    attr = mesh.color_attributes.new(name=attr_name, type="BYTE_COLOR", domain="CORNER")
    # Map by vertex Y in object space for a path-like strip
    for i, loop in enumerate(mesh.loops):
        v = mesh.vertices[loop.vertex_index].co
        # dirt path near x=0, greener outward
        t = min(1.0, abs(v.x) / 4.0)
        dirt = (0.35, 0.22, 0.12, 1.0)
        grass = (0.25, 0.65, 0.28, 1.0)
        col = tuple(dirt[j] * (1 - t) + grass[j] * t for j in range(4))
        attr.data[i].color = col


def export_glb(path: Path):
    path.parent.mkdir(parents=True, exist_ok=True)
    bpy.ops.export_scene.gltf(
        filepath=str(path),
        export_format="GLB",
        use_selection=False,
        export_apply=True,
        export_extras=True,
        export_animations=True,
    )


def make_starshard(out: Path):
    reset_scene()
    bpy.ops.mesh.primitive_ico_sphere_add(subdivisions=1, radius=0.6, location=(0, 0, 0.6))
    obj = bpy.context.active_object
    obj.name = "Starshard"
    # Squash into crystal-ish
    obj.scale = (0.7, 0.7, 1.3)
    bpy.ops.object.transform_apply(scale=True)
    add_vertex_colors(obj, (0.35, 0.85, 1.0, 1.0))
    export_glb(out / "starshard.glb")
    bpy.ops.wm.save_as_mainfile(filepath=str(out / "starshard.blend"))


def make_platform(out: Path):
    reset_scene()
    bpy.ops.mesh.primitive_cube_add(size=2, location=(0, 0, 0.25))
    obj = bpy.context.active_object
    obj.name = "Platform"
    obj.scale = (2.5, 2.5, 0.25)
    bpy.ops.object.transform_apply(scale=True)
    add_vertex_colors(obj, (0.55, 0.5, 0.45, 1.0))
    export_glb(out / "platform.glb")
    bpy.ops.wm.save_as_mainfile(filepath=str(out / "platform.blend"))


def make_island(out: Path):
    reset_scene()
    bpy.ops.mesh.primitive_grid_add(x_subdivisions=12, y_subdivisions=12, size=12, location=(0, 0, 0))
    obj = bpy.context.active_object
    obj.name = "Island"
    # Mild dome
    for v in obj.data.vertices:
        r = math.sqrt(v.co.x * v.co.x + v.co.y * v.co.y)
        v.co.z = max(0.0, 1.2 - r * 0.18)
    paint_plane_gradient(obj)
    link_vertex_color_material(obj, "Col")
    export_glb(out / "island.glb")
    bpy.ops.wm.save_as_mainfile(filepath=str(out / "island.blend"))


def make_player_block(out: Path):
    """Ultra-simple capsule-ish player (static) for early lessons before rigging."""
    reset_scene()
    bpy.ops.mesh.primitive_cylinder_add(radius=0.35, depth=1.0, location=(0, 0, 0.7))
    body = bpy.context.active_object
    body.name = "PlayerBody"
    bpy.ops.mesh.primitive_uv_sphere_add(radius=0.32, location=(0, 0, 1.35))
    head = bpy.context.active_object
    head.name = "PlayerHead"
    # Join
    ensure_object_mode()
    body.select_set(True)
    head.select_set(True)
    bpy.context.view_layer.objects.active = body
    bpy.ops.object.join()
    obj = bpy.context.active_object
    obj.name = "Player"
    add_vertex_colors(obj, (0.9, 0.75, 0.35, 1.0))
    export_glb(out / "player_static.glb")
    bpy.ops.wm.save_as_mainfile(filepath=str(out / "player_static.blend"))


def main():
    args = parse_args()
    out = Path(args.out_dir)
    out.mkdir(parents=True, exist_ok=True)
    make_starshard(out)
    make_platform(out)
    make_island(out)
    make_player_block(out)
    print("Generated course props in", out)


if __name__ == "__main__":
    main()
