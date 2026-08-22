<p align="right">
  <a href="./README_ENG.md">English</a> | <a href="./README.md">한국어</a> | 日本語
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

C++17 と OpenGL を用いて、レンダリングパイプラインの中核となる要素を直接実装した学習用リアルタイム 3D レンダラーです。

このプロジェクトは、Unity や Unreal Engine のような汎用ゲームエンジンを目指したものではありません。  
グラフィックス数学、ライティング、シャドウ、テクスチャマッピング、ノーマルマッピングなど、レンダリングコアを自分で実装しながら理解することを目的としています。

![Demo](/demo.gif)

## ✨ 概要

OpenGL を学び始めたとき、画面に表示される結果が内部でどのような過程を経て生成されるのかを自分で確認したいと思いました。  
そのため、グラフィックス学習と直接関係する部分は可能な限り自分で実装し、ウィンドウ生成やアセット読み込みなど、学習の中心ではない部分には外部ライブラリを使用しました。

このプロジェクトでは、以下の問いを実装を通して確認することを目標としました。

- 3D 空間上の頂点は、MVP 行列を通してどのように 2D 画面座標へ変換されるのか？
- Surface Normal と Light Direction の内積は、ライティング計算においてどのような意味を持つのか？
- Shadow Mapping では、なぜワールド座標を Light Space に変換する必要があるのか？
- Normal Map の tangent-space normal は、どのように world-space normal へ変換されるのか？
- Gamma Correction は、ライティング結果の見た目の明るさにどのような影響を与えるのか？

## ⚙️ 機能

### Rendering

- 3D Model Rendering
- Assimp と stb_image を用いた Texture Mapping
- Blinn-Phong Lighting
  - Ambient
  - Diffuse
  - Specular
- Material Support
  - Ka / Kd
  - Diffuse Texture
  - Normal Map
- Directional Light
- FBO を用いた 2-pass Shadow Mapping
- Shadow Bias
- 3×3 Percentage-Closer Filtering (PCF)
- Automatic Light-Space Fitting
- Tangent-space Normal Mapping
  - Tangent calculation
  - TBN matrix
  - Normal strength control
- Gamma Correction

### Graphics Math

グラフィックスパイプラインをより深く理解するため、GLM などの汎用数学ライブラリに依存せず、必要な数学機能の多くを自分で実装しました。

- Vector2 / Vector3 / Vector4
- Matrix3 / Matrix4
- Dot / Cross Product
- Translation / Rotation / Scaling
- Perspective / Orthographic Projection
- LookAt
- Matrix Inverse / Transpose

### Model & Material

- Assimp による 3D Model Loading
- Mesh / Submesh 処理
- Vertex Position / Normal / UV
- Material と Texture Loading

### Interface

Dear ImGui を用いて、簡単な Debug / Control Interface を実装しています。

- Hierarchy
- Inspector
- Rendering Statistics
- Camera Parameters
- Lighting Controls

GUI は完全な Scene Editor を目指したものではなく、レンダリング結果を確認し、各種パラメータをリアルタイムに調整するためのものです。

## ⚠️ 制限事項

このプロジェクトは、レンダリングパイプラインとグラフィックス数学の直接実装に焦点を当てた学習用レンダラーです。そのため、以下のような制限があります。

- 現在のライティングモデルは Blinn-Phong ベースであり、PBR パイプラインは実装していません。
- Directional Light を中心にレンダリングしており、Point Light / Spot Light および Cubemap Shadow は実装していません。
- 室内シーンでは Global Illumination や IBL がないため、間接光の表現には限界があり、Ambient Term で近似しています。
- 単一の imported scene/model を対象としており、複数モデルの Scene Graph や per-object transform は実装範囲外です。
- Normal Map は material に接続されている場合、またはファイル名規則によって検出された場合に読み込まれます。結果はアセットの品質に依存します。

## 🚧 Future Work

- Point Light / Spot Light
- Cubemap Shadow Mapping for Point Lights
- HDR Rendering and Tone Mapping
- Physically Based Rendering (PBR)
- SSAO

## 🎮 操作方法

3D 空間を自由に移動できるように、Free-fly Camera 操作をサポートしています。

- **`W` / `A` / `S` / `D`**: カメラの前後左右移動
- **`↑` / `↓`**: カメラの垂直移動
- **`Mouse Drag`**: カメラ回転
- **`UI Panel`**: ImGui ウィンドウからレンダリングパラメータをリアルタイムに調整

## 🧱 Tech Stack

### Core

- C++17
- OpenGL
- GLSL

### Libraries

ウィンドウ生成およびアセット読み込みのため、以下のライブラリを使用しています。

- GLFW
- GLAD
- Assimp
- stb_image
- Dear ImGui

### Development Tools

- Visual Studio Community
- Windows

現在は Windows 環境で開発しています。  
マルチプラットフォームを目的としたプロジェクトではないため、他の OS への互換性は優先していません。
