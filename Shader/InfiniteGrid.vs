// =============================================================================
// InfiniteGrid Vertex Shader
// =============================================================================
// Generates a fullscreen quad using SV_VertexID (no vertex buffer needed).
// Unprojects clip-space corners to world space at near and far planes,
// passing these as rays to the pixel shader for ray-plane intersection.
// =============================================================================

cbuffer InvView : register(b0)
{
    float4x4 g_InvView;
};

cbuffer InvProj : register(b1)
{
    float4x4 g_InvProj;
};

struct VS_OUTPUT
{
    float4 Pos       : SV_POSITION;
    float3 NearPoint : TEXCOORD0;
    float3 FarPoint  : TEXCOORD1;
};

// Helper: unproject a clip-space position to world space
float3 UnprojectPoint(float2 clipPos, float depth, float4x4 invProj, float4x4 invView)
{
    // Transform from clip space [-1,1] to NDC view space
    float4 ndcPos = float4(clipPos, depth, 1.0);

    // Unproject using inverse projection
    float4 viewPos = mul(ndcPos, invProj);
    viewPos /= viewPos.w;

    // Transform from view space to world space
    float4 worldPos = mul(viewPos, invView);
    return worldPos.xyz;
}

VS_OUTPUT VSMain(uint vertexID : SV_VertexID)
{
    VS_OUTPUT output;

    // Generate a fullscreen triangle strip:
    // Vertex 0: (-1, +1) -- top-left
    // Vertex 1: (+1, +1) -- top-right
    // Vertex 2: (-1, -1) -- bottom-left
    // Vertex 3: (+1, -1) -- bottom-right
    float2 clipPos = float2(
        (vertexID & 1) ? 1.0 : -1.0,
        (vertexID & 2) ? -1.0 : 1.0
    );

    output.Pos = float4(clipPos, 1.0, 1.0);

    // Unproject to world space at near plane (z = 0 in NDC)
    output.NearPoint = UnprojectPoint(clipPos, 0.0, g_InvProj, g_InvView);

    // Unproject to world space at far plane (z = 1 in NDC)
    output.FarPoint = UnprojectPoint(clipPos, 1.0, g_InvProj, g_InvView);

    return output;
}
