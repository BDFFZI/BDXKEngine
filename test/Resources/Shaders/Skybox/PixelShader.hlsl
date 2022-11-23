#include "VertexShader.hlsl"

float4 main(Pixel pixel) : SV_TARGET
{
    return float4(
        Skybox.Sample(SkyboxSampler, pixel.worldPosition - vector0.xyz).xyz,
        1
    );
}