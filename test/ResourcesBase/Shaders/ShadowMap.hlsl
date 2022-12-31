#include "Lighting.hlsli"

struct Pixel
{
    float4 positionCS : SV_POSITION;
    float4 positionVS : POSITION;
    float2 uv : TEXCOORD;
};

Pixel VertexPass(Vertex vertex)
{
    Pixel pixed;

    pixed.positionVS = TransformObjectToView_Light(vertex.position);
    pixed.positionCS = TransformViewToClip_Light(pixed.positionVS);
    pixed.uv = vertex.uv;

    return pixed;
}

float4 PixelPass(Pixel pixel) : SV_TARGET
{
    if (LightType == 1)
        return float4(0, 0, length(pixel.positionVS), 0.3f);
    else
        return float4(pixel.positionVS.xyz, 0.3f);

    //透明度设为0.3f纯粹是为了看阴影贴图方便
}