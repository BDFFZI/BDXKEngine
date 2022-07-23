#include "VertexShader.hlsl"

float4 main(Pixel pixel) : SV_TARGET
{
    return 1;
    //return float4(pixel.position, 1.0f);
}