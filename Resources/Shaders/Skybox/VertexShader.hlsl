#include "..\BDXKEngine.hlsli"

struct Pixel
{
    float4 clipPosition : SV_POSITION;
    float3 worldPosition : POSITION;
};

Pixel main(Vertex vertex)
{
    Pixel pixed;
    
    vertex.position.z = 1;
    float4 position = float4(vertex.position, 1);
    position = mul(ObjectToWorld, position);
    position = mul(WorldToView, position);
    position = mul(ViewToClip, position);
    pixed.clipPosition = position;
    pixed.worldPosition = mul(matrix0, position);
    
    return pixed;
}