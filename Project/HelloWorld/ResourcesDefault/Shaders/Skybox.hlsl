#include "Core.hlsli"

struct Pixel
{
    float4 positionCS : SV_POSITION;
    float3 positionWS : POSITION;
};

Pixel VertexPass(Vertex vertex)
{
    Pixel pixed;

    pixed.positionCS = TransformWorldToClip(vertex.position);
    pixed.positionWS = mul(matrix0, float4(pixed.positionCS.xy,1,1));

    return pixed;
}

float4 PixelPass(Pixel pixel) : SV_TARGET
{
    return float4(
        Skybox.Sample(SkyboxSampler, pixel.positionWS - vector0.xyz).xyz,
        1
    );
}
