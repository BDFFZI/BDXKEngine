#include "VertexShader.hlsl"

float4 main(Pixel pixel) : SV_TARGET
{
    return float4(pixel.lightposition, 0.1f); //͸������Ϊ0.1f������Ϊ�˿���Ӱ��ͼ����
}