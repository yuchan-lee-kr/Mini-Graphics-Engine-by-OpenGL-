<p align="right">
  <a href="./README_ENG.md">English</a> | 한국어 | <a href="./README_JA.md">日本語</a>
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

C++17과 OpenGL을 사용해 렌더링 파이프라인의 핵심 요소를 직접 구현한 학습용 실시간 3D 렌더러입니다.
이 프로젝트는 범용 게임 엔진보다 **렌더링 코어의 직접 구현**에 초점을 맞추었습니다. 따라서 복잡한 Scene Editor나 다중 플랫폼 지원보다는, 그래픽스 수학과 렌더링 파이프라인을 직접 구현하는 데 개발 범위를 집중했습니다.

![Alt Text](/demo.gif)

## ✨개요
처음 OpenGL을 공부하면서 화면에 보이는 결과가 내부에서 어떤 과정을 거쳐 만들어지는지를 직접 확인해보고 싶었습니다.
그래서 가능한 부분은 직접 구현하고,
그래픽스 학습과 직접적인 관련이 적은 부분은 외부 라이브러리를 사용했습니다.

이 프로젝트는 다음 질문을 직접 구현을 통해 확인하는 것을 목표로 했습니다.

- 3D 공간의 정점은 MVP 행렬을 거쳐 어떻게 2D 화면 좌표로 변환되는가?
- Surface Normal과 Light Direction의 내적은 조명 계산에서 어떤 의미를 가지는가?
- Shadow Mapping에서 왜 월드 좌표를 Light Space로 변환해야 하는가?
- Normal Map의 tangent-space normal은 어떻게 world-space normal로 변환되는가?
- Gamma Correction은 조명 결과의 시각적 밝기에 어떤 영향을 주는가?

## ⚙️ 기능
현재 구현된 주요 기능입니다.


### Rendering

- 3D Model Rendering
- Texture Mapping : Assimp와 stb_image를 활용한 3D 모델 및 텍스처 로딩 파이프라인 구축
- Blinn-Phong Lighting
  - Ambient
  - Diffuse
  - Specular
- Material Support (Ka, Kd, Diffuse Texture)
- Directional Light
- Shadow Mapping: FBO(Framebuffer Object)를 활용한 2-Pass 렌더링 파이프라인 구축
- Shadow Bias
- 3×3 Percentage-Closer Filtering (PCF)
- Automatic Light-Space Fitting
- Tangent-space Normal Mapping
  - Tangent calculation
  - TBN matrix
  - Normal strength control
- Gamma Correction
  
### Graphics Math

그래픽스 파이프라인을 이해하기 위해 glm라이브러리를 사용하지 않고 필요한 수학 기능의 상당 부분을 직접 구현했습니다.

- Vector2 / Vector3 / Vector4
- Matrix3 / Matrix4
- Dot / Cross Product
- Translation / Rotation / Scaling
- Perspective / Orthographic Projection
- LookAt
- Matrix Inverse / Transpose


### Model & Material

- Assimp 기반 3D Model Loading
- Mesh / Submesh 처리
- Vertex Position / Normal / UV
- Material 및 Texture Loading


### Interface

Dear ImGui를 이용한 간단한 Debug / Control Interface를 제공합니다.

- Hierarchy
- Inspector
- Rendering Statistics
- Camera Parameters
- Lighting Controls
  
GUI는 완전한 Scene Editor보다는
렌더링 결과를 확인하고 Parameter를 실시간으로 조절하기 위한 용도로 사용합니다.

## ⚠️한계

이 프로젝트는 렌더링 파이프라인과 그래픽스 수학의 직접 구현에 초점을 둔 학습용 렌더러입니다. 따라서 다음과 같은 제한이 있습니다.

- 현재 조명 모델은 Blinn-Phong 기반이며 PBR 파이프라인은 구현하지 않았습니다.
- Directional Light 하나를 중심으로 렌더링하며, Point Light / Spot Light 및 Cubemap Shadow는 구현하지 않았습니다.
- 실내 장면은 Global Illumination이나 IBL이 없기 때문에 간접광 표현이 제한적이며, Ambient Term으로 근사합니다.
- 단일 imported scene/model을 대상으로 하며, 복수 모델에 대한 Scene Graph와 per-object transform은 구현 범위에서 제외했습니다.
- Normal Map은 material에 연결된 경우 또는 파일명 규칙에 따라 로드하며, asset 품질에 따라 결과가 달라질 수 있습니다.

 ## 🚧향후 개선 방향

- Point Light / Spot Light
- Cubemap Shadow Mapping for Point Lights
- HDR Rendering and Tone Mapping
- Physically Based Rendering (PBR)
- SSAO

## 🎮조작 방법 

3D 공간을 자유롭게 탐색할 수 있도록 Free-fly 카메라 조작을 지원합니다.

* **`W` / `A` / `S` / `D`**: 카메라 전후좌우 이동
* **`↑` / `↓` (방향키)**: 카메라 수직 상승/하강
* **`Mouse Drag`**: 마우스을 통한 카메라 회전
* **`UI 패널`**: 화면 내 ImGui 윈도우를 통해 렌더링 파라미터 실시간 조절

## 🧱 기술 스택

### Core

- C++17
- OpenGL
- GLSL

### 라이브러리

윈도우 생성 및 에셋 로딩을 위해 다음 라이브러리들을 활용했습니다.
- GLFW
- GLAD
- Assimp
- stb_image
- Dear ImGui

### 개발 툴

- Visual Studio Community
- Windows

### 테스트 모델 
Morgan McGuire, Computer Graphics Archive, July 2017 (https://casual-effects.com/data)


현재는 Windows 환경에서 개발하고 있습니다.

다중 플랫폼을 목표로 하는 프로젝트는 아니기 때문에
다른 운영체제에 대한 호환성은 우선순위에 두고 있지 않습니다.
