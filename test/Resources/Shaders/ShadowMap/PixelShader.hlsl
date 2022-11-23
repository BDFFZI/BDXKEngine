#include "VertexShader.hlsl"

float4 main(Pixel pixel) : SV_TARGET
{
    if(LightFactorInt.x == 1)
        return float4(0, 0, length(pixel.lightposition), 0.3f);
    else
        return float4(pixel.lightposition, 0.3f); 
    
    //͸������Ϊ0.3f������Ϊ�˿���Ӱ��ͼ����
}