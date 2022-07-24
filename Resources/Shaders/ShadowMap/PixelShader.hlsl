#include "VertexShader.hlsl"

float4 main(Pixel pixel) : SV_TARGET
{
    return float4(pixel.lightposition, 0.1f); //透明度设为0.1f纯粹是为了看阴影贴图方便
}