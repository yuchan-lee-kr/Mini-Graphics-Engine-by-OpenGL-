# Mini Graphics Engine (OpenGL)

C++17과 OpenGL을 활용하여 밑바닥부터(From Scratch) 수학 및 렌더링 파이프라인을 구축한 학습용 실시간 렌더러입니다.
본 프로젝트의 목표는 상용 엔진(Unity, Unreal Engine 등) 수준의 범용성을 갖춘 툴을 만드는 것이 아닙니다.

**3D 그래픽스 이면의 수학적 원리와 렌더링 파이프라인의 구조를 직접 설계하며 깊이 있게 이해하는 것**에 초점을 맞추고 있습니다.

엔진의 부가적인 기능 확장보다는, 렌더링 코어 기술과 그래픽스 수학의 구현에 집중하기 위해 다음과 같이 개발 범위를 제한했습니다.
* **수학 라이브러리 직접 구현:** 범용 라이브러리(GLM 등)에 의존하지 않고 행렬 변환과 벡터 연산 등 선형대수학 로직을 직접 구현
* **단일 모델 및 렌더링 코어 집중:** 복잡한 씬(Scene) 관리 로직 대신, 하나의 모델에 대한 조명, 그림자, 재질 등 그래픽 퀄리티 향상에 집중
* **조명 시스템:** Directional Light와 Shadow Mapping을 활용한 렌더링 파이프라인 구축
* **제한적 환경:** Windows 환경 중심 (Cross-platform 및 복잡한 Material System 미구현)

##  개요
처음 OpenGL을 공부하면서 화면에 보이는 결과가 내부에서 어떤 과정을 거쳐 만들어지는지를 직접 확인해보고 싶었습니다.
그래서 가능한 부분은 직접 구현하고,
그래픽스 학습과 직접적인 관련이 적은 부분은 외부 라이브러리를 사용했습니다.

이 프로젝트에서 제게 스스로 던진 질문들입니다.

* **공간 변환 (Space Transformation):** 3D 공간의 정점(Vertex)은 MVP (Model-View-Projection) 행렬을 통해 어떤 선형 변환을 거쳐 2D 화면으로 투영되는가?
* **조명 방정식 (Lighting Equation):** Light Vector와 Surface Normal의 내적(Dot Product)을 활용해 물리적인 빛의 반사(Phong Shading)를 어떻게 수식화하는가?
* **그림자 렌더링 (Shadow Mapping):** 그림자를 판별하기 위해 월드 좌표계를 빛의 시점(Light Space)으로 변환하는 이유는 무엇이며, 깊이 맵(Depth Map)은 어떻게 작동하는가?


## 기능
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

## 조작 방법 (Controls)

3D 공간을 자유롭게 탐색할 수 있도록 Free-fly 카메라 조작을 지원합니다.

* **`W` / `A` / `S` / `D`**: 카메라 전후좌우 이동
* **`↑` / `↓` (방향키)**: 카메라 수직 상승/하강
* **`Mouse Drag`**: 마우스을 통한 카메라 회전
* **`UI 패널`**: 화면 내 ImGui 윈도우를 통해 렌더링 파라미터 실시간 조절

## Planed

다음 기능들을 우선적으로 구현할 계획입니다.

- Normal Mapping
- Camera Improvements
- Material / Lighting Controls

## Tech Stack

### Core

- C++17
- OpenGL
- GLSL

### Libraries

윈도우 생성 및 에셋 로딩을 위해 다음 라이브러리들을 활용했습니다.
- GLFW
- GLAD
- Assimp
- stb_image
- Dear ImGui

### Development

- Visual Studio Community
- Windows

현재는 Windows 환경에서 개발하고 있습니다.

다중 플랫폼을 목표로 하는 프로젝트는 아니기 때문에
다른 운영체제에 대한 호환성은 우선순위에 두고 있지 않습니다.
