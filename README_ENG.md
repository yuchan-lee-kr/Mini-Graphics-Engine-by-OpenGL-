<p align="right">
  English | <a href="./README.md">한국어</a> | <a href="./README_JA.md">日本語</a>
</p>

<h1 align="center">Mini Graphics Engine</h1>

<p align="center">
  <b>A real-time OpenGL renderer built to understand graphics math and rendering pipelines from scratch.</b>
</p>

<p align="center">
  <img src="images/main_preview.png" width="850">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C++-17-blue">
  <img src="https://img.shields.io/badge/OpenGL-Renderer-green">
  <img src="https://img.shields.io/badge/GLSL-Shaders-purple">
  <img src="https://img.shields.io/badge/Dear%20ImGui-Editor-orange">
</p>

This is a learning-oriented real-time 3D renderer built with C++17 and OpenGL.

The goal of this project is not to build a general-purpose game engine like Unity or Unreal Engine. Instead, it focuses on directly implementing the core parts of a rendering pipeline, including graphics math, lighting, shadows, texture mapping, normal mapping, and runtime rendering controls.

![Demo](/demo.gif)

## ✨ Overview

When I first started learning OpenGL, I wanted to understand how the final image on the screen is actually produced internally. For that reason, I implemented the core graphics-related parts myself and used external libraries only for areas less directly related to graphics learning, such as window creation and asset loading.

This project was built around the following questions:

- How are 3D vertices transformed into 2D screen coordinates through MVP matrices?
- What does the dot product between a surface normal and a light direction mean in lighting calculations?
- Why does Shadow Mapping require transforming world-space positions into light space?
- How is a tangent-space normal from a normal map transformed into world space?
- How does Gamma Correction affect the perceived brightness of lighting results?

## ⚙️ Features

### Rendering

- 3D Model Rendering
- Texture Mapping using Assimp and stb_image
- Blinn-Phong Lighting
  - Ambient
  - Diffuse
  - Specular
- Material Support
  - Ka / Kd
  - Diffuse Texture
  - Normal Map
- Directional Light
- Shadow Mapping using a 2-pass rendering pipeline with FBO
- Shadow Bias
- 3×3 Percentage-Closer Filtering (PCF)
- Automatic Light-Space Fitting
- Tangent-space Normal Mapping
  - Tangent calculation
  - TBN matrix
  - Normal strength control
- Gamma Correction

### Graphics Math

To better understand the graphics pipeline, I implemented a significant part of the required math functionality without relying on GLM.

- Vector2 / Vector3 / Vector4
- Matrix3 / Matrix4
- Dot / Cross Product
- Translation / Rotation / Scaling
- Perspective / Orthographic Projection
- LookAt
- Matrix Inverse / Transpose

### Model & Material

- 3D model loading with Assimp
- Mesh / Submesh processing
- Vertex Position / Normal / UV
- Material and texture loading

### Interface

A simple debug and control interface is provided using Dear ImGui.

- Hierarchy
- Inspector
- Rendering Statistics
- Camera Parameters
- Lighting Controls

The GUI is not intended to be a full scene editor. It is mainly used to inspect rendering results and adjust rendering parameters in real time.

## ⚠️ Limitations

This project is a learning-oriented renderer focused on directly implementing graphics math and the rendering pipeline. Therefore, it has the following limitations:

- The current lighting model is based on Blinn-Phong shading, not a PBR pipeline.
- The renderer mainly uses a single directional light. Point lights, spot lights, and cubemap shadows are not implemented.
- Indoor scenes have limited indirect illumination because Global Illumination and IBL are not implemented. Ambient lighting is used as an approximation.
- The renderer targets a single imported scene/model. Multi-model scene graphs and per-object transforms are outside the current scope.
- Normal maps are loaded when connected to a material or detected through a filename convention. The final result may vary depending on asset quality.

## 🚧 Future Work

- Point Light / Spot Light
- Cubemap Shadow Mapping for Point Lights
- HDR Rendering and Tone Mapping
- Physically Based Rendering (PBR)
- SSAO

## 🎮 Controls

The renderer supports free camera navigation in 3D space.

- **`W` / `A` / `S` / `D`**: Move camera forward, left, backward, and right
- **`↑` / `↓`**: Move camera vertically
- **`Mouse Drag`**: Rotate camera
- **`UI Panel`**: Adjust rendering parameters in real time through ImGui windows

## 🧱 Tech Stack

### Core

- C++17
- OpenGL
- GLSL

### Libraries

The following libraries are used for window creation and asset loading:

- GLFW
- GLAD
- Assimp
- stb_image
- Dear ImGui

### Development Tools

- Visual Studio Community
- Windows

### Test Model
Morgan McGuire, Computer Graphics Archive, July 2017 (https://casual-effects.com/data)

This project is currently developed on Windows.  
Cross-platform compatibility is not a priority because the project is not intended to be a multi-platform engine.
