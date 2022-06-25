//float4 main(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
//{
//    return color;
//}

#include "VertexShader.hlsl"

float4 main(Pixel pixel) : SV_TARGET
{
    return pixel.color;
}