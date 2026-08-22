# Mini Graphics Engine (OpenGL)

This is an educational real-time renderer built from scratch using C++17 and OpenGL, constructing the mathematics and rendering pipelines from the ground up.

The goal of this project is not to create a versatile tool on the level of commercial engines (like Unity or Unreal Engine). Instead, it focuses on **deeply understanding the mathematical principles behind 3D graphics and designing the structure of the rendering pipeline directly**.

To concentrate on rendering core technologies and the implementation of graphics mathematics rather than expanding auxiliary engine features, the development scope is restricted as follows:

* **Custom Math Library Implementation:** Directly implementing linear algebra logic, such as matrix transformations and vector operations, without relying on general-purpose libraries (like GLM).
* **Focus on Single Model & Rendering Core:** Prioritizing the improvement of graphics quality—such as lighting, shadows, and materials for a single model—rather than complex scene management logic.
* **Lighting System:** Building a rendering pipeline utilizing Directional Light and Shadow Mapping.
* **Restricted Environment:** Windows-centric development (Cross-platform support and complex material systems are not implemented).

## Overview

While initially studying OpenGL, I wanted to see firsthand the internal processes that create the results visible on the screen. Therefore, I implemented as much as possible from scratch and used external libraries only for parts less directly related to learning graphics.

Here are the core questions I asked myself during this project:

* **Space Transformation:** Through what linear transformations do vertices in 3D space project onto a 2D screen using the MVP (Model-View-Projection) matrix?
* **Lighting Equation:** How is the physical reflection of light (Phong Shading) formulated using the dot product of the Light Vector and the Surface Normal?
* **Shadow Mapping:** Why is the world coordinate system transformed into Light Space to determine shadows, and how does a Depth Map work?

## Features

These are the main features currently implemented.

### Rendering

- 3D Model Rendering
- Texture Mapping: 3D model and texture loading pipeline using Assimp and stb_image
- Blinn-Phong Lighting
  - Ambient
  - Diffuse
  - Specular
- Material Support (Ka, Kd, Diffuse Texture)
- Directional Light
- Shadow Mapping: 2-Pass rendering pipeline utilizing FBO (Framebuffer Object)
- Shadow Bias
- 3×3 Percentage-Closer Filtering (PCF)
- Automatic Light-Space Fitting

### Graphics Math

To deeply understand the graphics pipeline, I implemented most of the necessary mathematical functions from scratch without using the GLM library.

- Vector2 / Vector3 / Vector4
- Matrix3 / Matrix4
- Dot / Cross Product
- Translation / Rotation / Scaling
- Perspective / Orthographic Projection
- LookAt
- Matrix Inverse / Transpose

### Model & Material

- Assimp-based 3D Model Loading
- Mesh / Submesh Processing
- Vertex Position / Normal / UV
- Material and Texture Loading

### Interface

Provides a simple Debug / Control Interface using Dear ImGui.

- Hierarchy
- Inspector
- Rendering Statistics
- Camera Parameters
- Lighting Controls
  
The GUI is intended for checking rendering results and adjusting parameters in real-time, rather than serving as a complete Scene Editor.

## Controls

Supports Free-fly camera controls to freely explore the 3D space.

* **`W` / `A` / `S` / `D`**: Move camera Forward/Backward/Left/Right
* **`↑` / `↓` (Arrow Keys)**: Move camera Up/Down (Vertical ascent/descent)
* **`Mouse Drag`**: Camera rotation
* **`UI Panel`**: Real-time adjustment of rendering parameters via on-screen ImGui windows

## Planned

I plan to implement the following features next:

- Normal Mapping
- Camera Improvements
- Material / Lighting Controls

## Tech Stack

### Core

- C++17
- OpenGL
- GLSL

### Libraries

Used the following libraries for window creation and asset loading:
- GLFW
- GLAD
- Assimp
- stb_image
- Dear ImGui

### Development

- Visual Studio Community
- Windows

The project is currently being developed in a Windows environment. Because it does not aim to be a multi-platform project, cross-OS compatibility is not a priority.
