// GaussianBlurH.cs
// Separable Gaussian blur — horizontal pass
// Reads from InputTex (SRV, t0) and writes to OutputTex (UAV, u0).
// Dispatch with ceil(width/8) x ceil(height/8) x 1 groups.

Texture2D<float4>   InputTex  : register(t0);
RWTexture2D<float4> OutputTex : register(u0);

cbuffer CBBlurParams : register(b0)
{
    int   gWidth;       // texture width  in pixels
    int   gHeight;      // texture height in pixels
    int   gBlurRadius;  // kernel half-width (total taps = 2*radius+1)
    float gSigma;       // Gaussian standard deviation
};

[numthreads(8, 8, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
    int2 coord = (int2)DTid.xy;
    if (coord.x >= gWidth || coord.y >= gHeight)
        return;

    float  weightSum = 0.0f;
    float4 result    = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float invTwoSigmaSq = 1.0f / (2.0f * gSigma * gSigma);

    for (int offset = -gBlurRadius; offset <= gBlurRadius; ++offset)
    {
        int sampleX = clamp(coord.x + offset, 0, gWidth - 1);
        float w = exp(-(float)(offset * offset) * invTwoSigmaSq);
        result    += InputTex[int2(sampleX, coord.y)] * w;
        weightSum += w;
    }

    OutputTex[coord] = result / weightSum;
}
