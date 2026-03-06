# UEFFTH

## Instance Painter Plugin

A custom Unreal Engine editor plugin that provides a brush-based tool for painting Static Mesh instances into a scene — similar to the built-in Foliage system but with enhanced mask support and customization options.

---

## Features

- **Brush Painting** — Paint Static Mesh instances directly onto Landscape, Static Mesh surfaces, and BSP geometry using a configurable circular brush.
- **Erase Tool** — Remove painted instances within the brush radius.
- **Single Place Tool** — Place individual mesh instances with a single click.
- **Brush Visualization** — Real-time green (paint) / red (erase) brush circle with falloff ring rendered in the viewport.
- **Brush Shortcuts** — `[` to shrink radius, `]` to enlarge radius.
- **Falloff** — Configurable linear falloff from the brush edge inward.
- **Alpha Mask Texture** — Sample a Texture2D's red channel as a probability mask; supports fixed and randomized rotation.
- **Per-Entry Configuration**
  - Uniform or per-axis random scale ranges.
  - Independent Yaw / Pitch / Roll rotation ranges (degrees).
  - Normal alignment with configurable strength (0 = vertical, 1 = fully aligned to surface normal).
  - Height offset range along the surface normal.
  - Minimum instance spacing (uses HISM overlap query).
  - Material overrides.
  - Weight-based random selection across multiple mesh types.
- **Surface Filters** — Enable/disable painting on Landscape, Static Mesh, and BSP surfaces.
- **Slope Filter** — Reject placement on surfaces steeper than a configurable angle.
- **Clear All** — Remove every painted instance in one click.

---

## Installation

1. Copy the `Plugins/InstancePainter/` folder into your project's `Plugins/` directory.
2. Rebuild the project (right-click the `.uproject` file → *Generate Visual Studio project files*, then build from the IDE).
3. Enable the plugin in *Edit → Plugins → Instance Painter*.

---

## Usage

### Setting up the target component

1. Select (or create) an Actor in the Level.
2. Add a **Instance Painter Component** (`UInstancePainterComponent`) to it via the *Details* panel → *Add Component*.
3. In the component's *Paint Entries* array, add one or more entries and assign Static Meshes.

### Activating the editor mode

- Open *Edit → Editor Modes* (or the Modes toolbar) and select **Instance Painter**, **or**
- Click the *Instance Painter* toolbar button added to the Level Editor toolbar.

### Tools

| Tool | Description |
|------|-------------|
| **Paint** | Hold LMB and drag to scatter instances within the brush. |
| **Erase** | Hold LMB and drag to remove instances within the brush. |
| **Single Place** | Click once to place a single instance at the cursor position. |

### Panel controls

| Control | Description |
|---------|-------------|
| Radius slider | Brush radius in world units (also `[` / `]`). |
| Falloff slider | Proportion of the brush that fades out (0 = hard edge). |
| Paint Density | Probability of a placement attempt per unit area per stroke. |
| Mask Texture | Optional Texture2D — red channel used as placement probability. |
| Mask Rotation | Fixed rotation applied to the mask UV. |
| Randomize Mask Rotation | Generate a fresh random rotation each stroke. |
| Landscape / Static Mesh / BSP | Toggle which surface types accept instances. |
| Max Slope Angle | Reject surfaces steeper than this (degrees from horizontal). |
| **Clear All Instances** | Removes every instance managed by the target component. |

---

## Screenshots

> *(Screenshots will be added once the plugin is tested inside a running UE project.)*

---

## License

MIT License

Copyright (c) 2024 Boos501

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.