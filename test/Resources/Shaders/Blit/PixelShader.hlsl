#include "VertexShader.hlsl"

float4 main(Pixel pixel) : SV_TARGET
{
    return Texture2D0.Sample(SamplerState0, pixel.uv);
}