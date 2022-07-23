#include "..\BDXKEngine.hlsli"

struct Pixel
{
    float4 svPosition : SV_POSITION;
    float3 position : POSITION;
};

Pixel main(Vertex vertex)
{
    Pixel pixed;
    
    float4 svPosition = float4(vertex.position, 1);
    svPosition = mul(LocalToWorld, svPosition);
    svPosition = mul(WorldToCamera, svPosition);
    svPosition = mul(CameraToView, svPosition);
    //svPosition.y = 1 - svPosition.y;
    pixed.svPosition = svPosition;
    
    //pixed.position = svPosition / svPosition.w;
    //pixed.position.y *= -1;
    
    return pixed;
}