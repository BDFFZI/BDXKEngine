#include "Core.hlsli"

struct Pixel
{
    float4 positionCS : SV_POSITION;
    float3 positionWS : POSITION;
};

Pixel VertexPass(Vertex vertex)
{
    Pixel pixed;

    pixed.positionWS = TransformObjectToWorld(vertex.position);
    pixed.positionCS = TransformWorldToClip(pixed.positionWS);

    return pixed;
}

float4 PixelPass(Pixel pixel) : SV_TARGET
{
    return float4(
        Skybox.Sample(SkyboxSampler, pixel.positionWS - vector0.xyz).xyz,
        1
    );
}
