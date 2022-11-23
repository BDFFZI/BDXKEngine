#include "BDXKEngine.hlsli"

struct Pixel
{
    float4 positionCS : SV_POSITION;
    float2 uv : TEXCOORD;
};

Pixel VertexPass(Vertex vertex)
{
    Pixel pixel;
    
    pixel.positionCS = ObjectToClipPos(vertex.position);
    pixel.uv = vertex.uv;
    
    return pixel;
}

float4 PixelPass(Pixel pixel) : SV_TARGET
{
    return Texture2D0.Sample(SamplerState0, pixel.uv);
}
