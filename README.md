# InstancePainter - UE 自定义笔刷实例绘制插件

## 功能
- 🖌️ 笔刷绘制 Static Mesh 实例（类似植被系统）
- 🎭 Alpha Mask 支持（Texture2D 红通道遮罩）
- 📐 缩放随机（Uniform / Non-Uniform，Min/Max）
- 🧭 法线对齐（可调强度）
- 🔄 旋转随机（Yaw/Pitch/Roll 独立 Min/Max）
- 🏗️ HISM 高性能渲染
- 🎯 三种模式：绘制 / 擦除 / 单点放置

## 编译方式

### 前提条件
- Unreal Engine 5.4（`InstancePainterHost.uproject` 的 `EngineAssociation` 默认设为 `5.4`，使用其他版本请修改该字段）
- Visual Studio 2022 或 JetBrains Rider

### 方式一：使用编译脚本（推荐）

1. 修改 `Build_Win64.bat` 中的 `UE_ROOT` 为你的引擎路径
2. 双击 `Build_Win64.bat`
3. 编译完成后双击 `InstancePainterHost.uproject` 打开编辑器

### 方式二：Visual Studio / Rider

1. 修改 `GenerateProjectFiles.bat` 中的 `UE_ROOT`
2. 双击 `GenerateProjectFiles.bat` 生成 .sln
3. 用 VS/Rider 打开 `InstancePainterHost.sln`
4. 设 `InstancePainterHostEditor` 为启动项目
5. 按 F5 编译并启动

### 方式三：独立编译插件（便携分发）

1. 修改 `BuildPlugin_Standalone.bat` 中的 `UE_ROOT`
2. 双击运行
3. 将 `BuiltPlugin/InstancePainter/` 复制到任何 UE 项目的 `Plugins/` 下

### 方式四：手动集成到已有项目

1. 将 `Plugins/InstancePainter/` 文件夹复制到你项目的 `Plugins/` 目录
2. 右键 `.uproject` → "Generate Visual Studio project files"
3. 在 VS/Rider 中编译
4. 或直接打开编辑器（会自动编译）

## 使用方法

1. 打开 UE 编辑器
2. 确认 Plugins 中 "Instance Painter" 已启用
3. 在场景中创建一个 Actor，添加 `InstancePainterComponent`
4. 在 Component 的 `PaintEntries` 中添加 Mesh 配置
5. 在 Modes 面板中切换到 "Instance Painter"
6. 选择 Paint/Erase/SinglePlace 工具
7. 在场景表面绘制

## 快捷键

| 快捷键 | 功能 |
|--------|------|
| `[` | 缩小笔刷 |
| `]` | 放大笔刷 |
| 左键拖动 | 绘制/擦除 |
| 左键单击 | 单点放置（SinglePlace 模式） |

---

## Features (English)

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