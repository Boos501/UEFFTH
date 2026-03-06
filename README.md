# UEFFTH — InstancePainter UE Plugin

UE5 自定义笔刷实例绘制工具插件，功能类似植被系统（Foliage），支持 Mask 遮罩、缩放/旋转随机、法线对齐等。

## 📋 前置要求

- Unreal Engine 5.7（可在 .uproject 中修改 EngineAssociation 适配其他版本）
- Visual Studio 2022 + "使用 C++ 的游戏开发" 工作负载
- .NET 6.0 SDK

## 🚀 编译方式

### 方式一：一键编译（推荐）

1. 打开 `Build_Win64.bat`，修改第一行 `UE_ROOT` 为你的引擎路径：
   ```
   SET UE_ROOT=D:\Epic Games\UE_5.7
   ```
2. 双击运行 `Build_Win64.bat`
3. 编译成功后双击 `UEFFTH.uproject` 打开编辑器

### 方式二：Visual Studio / Rider

1. 修改 `GenerateProjectFiles.bat` 中的 `UE_ROOT`
2. 双击运行 `GenerateProjectFiles.bat`
3. 打开生成的 `UEFFTH.sln`
4. 设置 `UEFFTHEditor (Win64 Development)` 为启动项目
5. 按 F5 编译并启动

### 方式三：独立编译插件（可分发）

1. 修改 `BuildPlugin_Standalone.bat` 中的 `UE_ROOT`
2. 双击运行
3. 编译输出在 `BuiltPlugin/` 目录
4. 将 `BuiltPlugin/InstancePainter/` 复制到任意 UE 项目的 `Plugins/` 下

### 方式四：集成到已有项目

1. 复制 `Plugins/InstancePainter/` 到你项目的 `Plugins/` 目录
2. 右键 `.uproject` → "Generate Visual Studio project files"
3. 打开 `.sln` 编译 或 直接双击 `.uproject`

## 🔧 编译问题排查

| 问题 | 解决方案 |
|------|----------|
| 找不到引擎路径 | 修改 bat 脚本中的 `UE_ROOT` |
| VS 编译报错 | 确保安装了 "使用 C++ 的游戏开发" 工作负载 |
| 编译缓存问题 | 运行 `CleanBuild.bat` 后重新编译 |
| 版本不匹配 | 修改 `.uproject` 中 `EngineAssociation` 和 `.Target.cs` 中的版本号 |

## 🎯 使用方法

1. 编译成功后打开编辑器
2. 在场景中创建一个空 Actor
3. 给 Actor 添加 `InstancePainterComponent` 组件
4. 在组件的 `PaintEntries` 中配置要绘制的 Mesh、缩放、旋转等参数
5. 在编辑器顶部工具栏点击 "Instance Painter" 按钮进入绘制模式
6. 选择 Paint/Erase/SinglePlace 工具开始绘制
7. 使用 `[` `]` 快捷键调整笔刷大小

## 📁 项目结构

```
UEFFTH/
├── UEFFTH.uproject                  # 宿主工程文件
├── Source/
│   ├── UEFFTH.Target.cs             # 游戏编译目标
│   ├── UEFFTHEditor.Target.cs       # 编辑器编译目标
│   └── UEFFTH/                      # 宿主模块
│       ├── UEFFTH.Build.cs
│       ├── UEFFTH.h
│       └── UEFFTH.cpp
├── Plugins/
│   └── InstancePainter/             # 插件目录
│       ├── InstancePainter.uplugin
│       └── Source/
│           ├── InstancePainterRuntime/
│           └── InstancePainterEditor/
├── Build_Win64.bat                  # 一键编译
├── GenerateProjectFiles.bat         # 生成 VS 工程
├── BuildPlugin_Standalone.bat       # 独立编译插件
├── CleanBuild.bat                   # 清理缓存
└── .gitignore
```

---

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