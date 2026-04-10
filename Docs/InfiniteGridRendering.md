요청하신 무한 그리드(Infinite Grid) 구현 내용을 정리한 Markdown 문서입니다. 아래 내용을 복사하여 `infiniteLine.md` 파일로 저장하거나 문서화에 사용하시기 바랍니다.

---

# Infinite Grid Shader Implementation Guide (DX11/HLSL)

이 문서는 별도의 대규모 메쉬 데이터 없이 **Ray-casting** 방식을 사용하여 $y=0$ 평면에 지평선 끝까지 펼쳐지는 무한 그리드를 구현하는 방법을 다룹니다.

## 1. 핵심 개념
* **Ray-Plane Intersection**: 화면의 각 픽셀(NDC)에서 카메라의 Near Plane과 Far Plane 지점을 월드 좌표로 역산한 뒤, 두 점을 잇는 레이와 $y=0$ 평면의 교점을 찾습니다.
* **Analytical Anti-aliasing**: `fwidth` 함수를 사용하여 카메라와의 거리에 상관없이 화면상에서 일정한 선 두께를 유지합니다.
* **No-Vertex-Buffer**: `SV_VertexID`를 활용해 CPU 측의 버퍼 바인딩 없이 4개의 정점(Triangle Strip)을 생성하여 화면 전체를 덮습니다.

---

## 2. HLSL 구현 (DirectX 11)

### A. Constant Buffer & 구조체
```hlsl
cbuffer SceneConstantBuffer : register(b0)
{
    float4x4 g_View;
    float4x4 g_Proj;
    float4x4 g_InvView;
    float4x4 g_InvProj;
    float3   g_CameraPos;
    float    g_Near;
    float    g_Far;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 NearPoint : TEXCOORD0;
    float3 FarPoint : TEXCOORD1;
};
```

### B. Vertex Shader
별도의 Vertex Buffer 없이 화면 전체를 채우는 쿼드를 생성합니다.

```hlsl
float3 UnprojectPoint(float x, float y, float z, float4x4 invView, float4x4 invProj)
{
    float4 viewSpace = mul(float4(x, y, z, 1.0), invProj);
    float4 worldSpace = mul(viewSpace / viewSpace.w, invView);
    return worldSpace.xyz;
}

PS_INPUT VSMain(uint vID : SV_VertexID)
{
    PS_INPUT output;
    
    // Triangle Strip: 0:(-1,1), 1:(1,1), 2:(-1,-1), 3:(1,-1)
    float2 uv = float2((vID << 1) & 2, vID & 2);
    float2 clipPos = uv * float2(2.0, -2.0) + float2(-1.0, 1.0);
    
    output.Pos = float4(clipPos, 0.0, 1.0);
    output.NearPoint = UnprojectPoint(clipPos.x, clipPos.y, 0.0, g_InvView, g_InvProj);
    output.FarPoint = UnprojectPoint(clipPos.x, clipPos.y, 1.0, g_InvView, g_InvProj);
    
    return output;
}
```

### C. Pixel Shader
평면과의 교점을 구하고 그리드 선을 그립니다.

```hlsl
struct PS_OUTPUT
{
    float4 Color : SV_Target;
    float  Depth : SV_Depth;
};

float4 Grid(float3 fragPos3D, float scale)
{
    float2 coord = fragPos3D.xz * scale;
    float2 derivative = fwidth(coord);
    float2 grid = abs(frac(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    
    float4 color = float4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    
    // 축 강조 (X축=Red, Z축=Blue)
    if(abs(fragPos3D.x) < 0.1 * derivative.x) color.z = 1.0; 
    if(abs(fragPos3D.z) < 0.1 * derivative.y) color.x = 1.0;
    
    return color;
}

PS_OUTPUT PSMain(PS_INPUT input)
{
    float t = -input.NearPoint.y / (input.FarPoint.y - input.NearPoint.y);
    clip(t > 0 ? 1 : -1); 

    float3 fragPos3D = input.NearPoint + t * (input.FarPoint - input.NearPoint);
    
    // Depth 계산
    float4 clipPos = mul(mul(float4(fragPos3D, 1.0), g_View), g_Proj);
    float depth = clipPos.z / clipPos.w;
    
    // 거리 기반 페이드 아웃
    float linearDepth = (2.0 * g_Near) / (g_Far + g_Near - depth * (g_Far - g_Near));
    float fading = max(0, (0.5 - linearDepth));

    PS_OUTPUT output;
    output.Depth = depth;
    output.Color = Grid(fragPos3D, 1.0);
    output.Color.a *= fading;

    return output;
}
```

---

## 3. 통합 가이드 (Engine Integration)

### 렌더 상태 (Render States)
* **Topology**: `D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP`
* **Blend State**: 
    * `BlendEnable = TRUE`
    * `SrcBlend = D3D11_BLEND_SRC_ALPHA`
    * `DestBlend = D3D11_BLEND_INV_SRC_ALPHA`
* **Depth Stencil State**: 
    * `DepthEnable = TRUE`
    * `DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL` (혹은 상황에 따라 Zero)

### 호출부 (C++ / C#)
```cpp
// 별도의 Vertex Buffer 바인딩 없이 실행
context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
context->Draw(4, 0);
```

---

## 4. 디버깅 및 주의사항
1.  **Inverse Matrices**: `g_InvView`와 `g_InvProj`가 최신 카메라 데이터로 매 프레임 업데이트되는지 확인하십시오.
2.  **Far Plane**: 레이와 평면이 평행할 경우(`direction.y == 0`) 분모가 0이 되는 현상을 방지하기 위해 `t` 계산 전 가드 코드를 추가하는 것이 좋습니다.
3.  **Coordinate System**: 위 코드는 $y=0$ 평면(Up vector = +Y) 기준입니다. 엔진의 좌표계에 따라 `xz` 평면이 아닌 다른 평면을 사용할 경우 스위즐링(Swizzling)을 수정하십시오.

---
**Tip:** 이 셰이더는 엔진 초기 단계에서 월드 공간의 원점과 스케일을 파악하는 데 매우 유용합니다. 앞서 진행하신 **용광로 테스트(White Furnace Test)** 환경 구축 시에도 바닥면의 존재 유무를 파악하기 위한 가이드로 활용해 보세요.