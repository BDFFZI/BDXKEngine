//struct VOut
//{
//    float4 position : SV_POSITION;
//    float4 color : COLOR;
//};

//VOut main(float4 position : POSITION, float4 color : COLOR)
//{
//    VOut output;

//    output.position = position;
//    output.color = color;

//    return output;
//}

#include "BDXKEngine.hlsli"

struct Pixel
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

Pixel main(Vertex vertex)
{
    float4 position = float4(vertex.position, 1);
    position = mul(LocalToWorld, position);
    position = mul(WorldToCamera, position);
    
    position.xy *= 60 / position.z;
    position = mul(CameraToView, position);
    
    Pixel pixed;
    pixed.position = position;
    pixed.color = vertex.color;
    return pixed;
}