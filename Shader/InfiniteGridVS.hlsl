

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