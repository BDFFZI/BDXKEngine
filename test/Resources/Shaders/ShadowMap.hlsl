#include "BDXKEngine.hlsli"

struct Pixel
{
    float4 clipPosition : SV_POSITION;
    float3 lightposition : POSITION;
    float2 uv : TEXCOORD;
};

Pixel VertexPass(Vertex vertex)
{
    Pixel pixed;
    
    float4 clipPosition = float4(vertex.position, 1);
    clipPosition = mul(ObjectToWorld, clipPosition);
    clipPosition = mul(WorldToLightView, clipPosition);
    pixed.lightposition = clipPosition.xyz;
    clipPosition = mul(ViewToLightClip, clipPosition);
    pixed.clipPosition = clipPosition;
    pixed.uv = vertex.uv;
    
    return pixed;
}

float4 PixelPass(Pixel pixel) : SV_TARGET
{
    if(LightType == 1)
        return float4(0, 0, length(pixel.lightposition), 0.3f);
    else
        return float4(pixel.lightposition, 0.3f); 
    
    //透明度设为0.3f纯粹是为了看阴影贴图方便
}



