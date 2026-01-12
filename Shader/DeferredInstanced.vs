//////////////////////
// CONSTANT BUFFERS //
//////////////////////
cbuffer Model : register(b0)
{
    float4x4 model;  
};

cbuffer View : register(b1)
{
    float4x4 view; 
};

cbuffer Proj : register(b2)
{
    float4x4 proj;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float4 tangent : TANGENT;
    
    // 인스턴스마다 다른 데이터를 받기 위해 추가 (시맨틱 이름은 편의상 TEXCOORD1 혹은 INSTANCE_POS 사용)
    float3 instanceOffset : INSTANCE_POS; 
};

struct PixelInputType
{
    float4 OutPosition : SV_POSITION;
    float4 OutViewPosition : COLOR0;
    float2 OutTexCoord : TEXCOORD0;
    float3 OutNormal : NORMALWS;
    float3 OutTangent : TANGENTWS;
    float3 OutBitangent : BITANGENTWS;
};



////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType VSMain(VertexInputType input)
{
    PixelInputType output;

    // 1. 인스턴스 오프셋이 적용된 로컬 위치 계산
    // float4(input.position.xyz + input.instanceOffset, 1.0f)와 같습니다.
    float4 localPosWithOffset = float4(input.position.xyz + input.instanceOffset, 1.0f);

    // 2. 행렬 미리 계산 (성능을 위해 VS 내부보다 외부에서 넘겨주는 것이 좋지만, 현재 로직 유지)
    float4x4 ModelView = mul(model, view);
    float4x4 MVP = mul(ModelView, proj);

    // 3. 최종 출력 위치 계산 (오프셋이 적용된 위치에 MVP를 곱함)
    // 기존 코드에서 output.OutPosition을 다시 덮어쓰던 부분을 제거했습니다.
    output.OutPosition = mul(localPosWithOffset, MVP);

    // 4. 기타 위치 정보 계산 (조명이나 이펙트용)
    // View 공간에서의 위치도 인스턴스 오프셋이 반영되어야 합니다.
    output.OutViewPosition = mul(localPosWithOffset, ModelView);
    
    // 텍스처 좌표 전달
    output.OutTexCoord = input.tex;

    // 5. 법선(Normal) 및 접선(Tangent) 계산
    // 이동(Translation)은 방향 벡터에 영향을 주지 않으므로 offset을 더하지 않습니다.
    float3 normalWS = normalize(mul(input.normal, (float3x3)ModelView));
    output.OutNormal = normalWS;
    
    float3 tangentWS = normalize(mul(input.tangent.xyz, (float3x3)ModelView));
    output.OutTangent = tangentWS;

    // 종법선(Bitangent) 계산
    float3 bitangentWS = normalize(cross(normalWS, tangentWS)) * input.tangent.w;
    output.OutBitangent = bitangentWS;

    return output;
}