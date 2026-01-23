# SuperSimpleEngine

![alt tag](https://github.com/ssogal00/SuperSimpleEngine/blob/RenderingRefactoring/ScreenShot/screenshot.png)


**SuperSimpleEngine**은 성능과 유연성을 동시에 확보하기 위해 설계된 DirectX 11 기반의 3D 게임 엔진입니다. 데이터 중심 설계(Data-Driven)와 멀티쓰레드 아키텍처를 통해 현대적인 렌더링 파이프라인의 기초를 학습하고 구현하는 것을 목표로 합니다.

## 🚀 Key Features

### 1. DirectX 11 Rendering Pipeline


### 2. Data-Driven Rendering Architecture

* 엔진 로직과 렌더링 데이터를 분리하여, 데이터 구조의 변경이 코드 전체에 미치는 영향을 최소화했습니다.


### 3. Multi-Threaded Design (Thread Separation)

* **Game Thread**와 **Render Thread**를 물리적으로 분리하여 하드웨어 성능을 최대한 활용합니다.
* 로직 계산(Update)과 화면 출력(Render)이 병렬로 처리되어 프레임 드랍을 최소화하고 안정적인 FPS를 유지합니다.---

## 🛠 Technical Highlights (Refinement)

엔진의 완성도를 높이기 위해 다음과 같은 기술적 요소들이 포함되어 있습니다.

* **Render Data Synchronization**: 게임 쓰레드에서 생성된 데이터를 렌더 쓰레드로 안전하게 전달하기 위한 **Producer-Consumer 패턴** 기반의 큐잉 시스템 혹은 데이터 스냅샷 메커니즘을 적용했습니다.

## 💻 Tech Stack

* **Language**: C++
* **Graphics API**: DirectX 11
* **Build System**: CMake (or Visual Studio Solution)
* **Target Platform**: Windows (x64)

## 🏗 Project Architecture

```text
SuperSimpleEngine/
├── Core/               # Engine Framework & Threading
├── Graphics/           # DX11 Wrapper & Renderer
├── Resources/          # Material, Mesh, Shader Data
├── Shaders/            # HLSL Source Codes
└── Common/             # Math & Utilities

```

---



# How to build
1. CMake required
2. Run VSProjectGen_VS2017.bat file for Visual Studio 2017 or VSProjectGen_VS2019 for Visual Studio 2019 to make solution file


