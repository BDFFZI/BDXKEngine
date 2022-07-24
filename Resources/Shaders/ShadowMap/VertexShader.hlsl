#include "..\BDXKEngine.hlsli"

struct Pixel
{
    float4 clipPosition : SV_POSITION;
    float3 lightposition : POSITION;
};

Pixel main(Vertex vertex)
{
    Pixel pixed;
    
    float4 clipPosition = float4(vertex.position, 1);
    clipPosition = mul(ObjectToWorld, clipPosition);
    clipPosition = mul(WorldToLightView, clipPosition);
    pixed.lightposition = clipPosition.xyz;
    clipPosition = mul(ViewToLightClip, clipPosition);
    pixed.clipPosition = clipPosition;
    
    return pixed;
}