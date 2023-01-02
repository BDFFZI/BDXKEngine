#include "Core.hlsli"

float4 VertexPass(Vertex vertex):SV_POSITION
{
    return TransformObjectToClip(vertex.position);
}

float4 PixelPass() : SV_TARGET
{
    return float4(1,0,1,1);
}