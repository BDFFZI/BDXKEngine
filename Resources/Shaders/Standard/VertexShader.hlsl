#include "..\BDXKEngine.hlsli"

struct Pixel
{
    float4 svPosition : SV_POSITION;
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

Pixel main(Vertex vertex)
{
    Pixel pixed;
    float4 svPosition = float4(vertex.position, 1);
    
    svPosition = mul(LocalToWorld, svPosition);
    pixed.position = svPosition.xyz;
    
    svPosition = mul(WorldToCamera, svPosition);
    svPosition = mul(CameraToView, svPosition);
    pixed.svPosition = svPosition;
    
    pixed.normal = LocalToWorldVector(vertex.normal);
    pixed.uv = vertex.uv;
    return pixed;
}