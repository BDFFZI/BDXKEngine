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
    
    svPosition = mul(ObjectToWorld, svPosition);
    pixed.position = svPosition.xyz;
    
    svPosition = mul(WorldToView, svPosition);
    svPosition = mul(ViewToClip, svPosition);
    pixed.svPosition = svPosition;
    
    pixed.normal = ObjectToWorldVector(vertex.normal);//TODO 会受缩放影响
    pixed.uv = vertex.uv;
    return pixed;
}