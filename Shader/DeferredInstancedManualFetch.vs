//////////////////////
// CONSTANT BUFFERS //
//////////////////////
cbuffer Model : register(b0) { float4x4 model; };
cbuffer View : register(b1) { float4x4 view; };
cbuffer Proj : register(b2) { float4x4 proj; };

///////////////////////
// STRUCTURED BUFFERS //
///////////////////////
// 각 요소를 별도의 배열(SoA 형태)로 선언합니다.
StructuredBuffer<float3> PositionBuffer      : register(t0);
StructuredBuffer<float3> NormalBuffer   : register(t1);
StructuredBuffer<float2> TexBuffer      : register(t2);
StructuredBuffer<float4> TangentBuffer  : register(t3);
StructuredBuffer<float3> InstanceBuffer : register(t4); // 인스턴스 오프셋 데이터

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 OutPosition     : SV_POSITION;
    float4 OutViewPosition : COLOR0;
    float2 OutTexCoord     : TEXCOORD0;
    float3 OutNormal       : NORMALWS;
    float3 OutTangent      : TANGENTWS;
    float3 OutBitangent    : BITANGENTWS;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
// 이제 VertexInputType 대신 ID들을 직접 인자로 받습니다.
PixelInputType VSMain(uint vID : SV_VertexID)
{
    PixelInputType output;

    // 1. 배열 인덱싱을 통한 데이터 페칭 (SoA 방식 접근)
    float3 rawPos = PositionBuffer[vID];
    float3 rawNormal = NormalBuffer[vID];
    float2 rawTex = TexBuffer[vID];
    float4 rawTangent = TangentBuffer[vID];
    

    // 2. 인스턴스 오프셋이 적용된 로컬 위치 계산
    float4 localPosWithOffset = float4(rawPos, 1.0f);

    // 3. 행렬 계산
    float4x4 ModelView = mul(model, view);
    float4x4 MVP = mul(ModelView, proj);

    // 4. 최종 출력 위치 및 뷰 공간 위치
    output.OutPosition = mul(localPosWithOffset, MVP);
    output.OutViewPosition = mul(localPosWithOffset, ModelView);
    
    // 텍스처 좌표 전달
    output.OutTexCoord = rawTex;

    // 5. 법선(Normal) 및 접선(Tangent) 계산
    float3 normalWS = normalize(mul(rawNormal, (float3x3)ModelView));
    output.OutNormal = normalWS;
    
    float3 tangentWS = normalize(mul(rawTangent.xyz, (float3x3)ModelView));
    output.OutTangent = tangentWS;

    // 종법선(Bitangent) 계산
    float3 bitangentWS = normalize(cross(normalWS, tangentWS)) * rawTangent.w;
    output.OutBitangent = bitangentWS;

    return output;
}