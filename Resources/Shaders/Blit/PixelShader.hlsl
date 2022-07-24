#include "VertexShader.hlsl"

float4 main(Pixel pixel) : SV_TARGET
{
    return Texture2D0.Sample(Texture2DSampler, pixel.uv);
}