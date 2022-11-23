#include "..\BDXKEngine.hlsli"

struct Pixel
{
    float4 svPosition : SV_POSITION;
    float2 uv : TEXCOORD;
};

Pixel main(Vertex vertex)
{
    Pixel pixel;
    
    pixel.svPosition = ObjectToClipPos(vertex.position);
    pixel.uv = vertex.uv;
    
    return pixel;
}