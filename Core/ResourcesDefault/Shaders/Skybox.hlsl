#include "Core.hlsli"

struct Pixel
{
    float4 positionCS : SV_POSITION;
    float3 positionWS : POSITION;
};

Pixel VertexPass(Vertex vertex)
{
    Pixel pixed;

    pixed.positionCS = TransformWorldToClip(vertex.position);
    pixed.positionWS = mul(matrix0, float4(pixed.positionCS.xy,1,1)).xyz;

    return pixed;
}

float4 PixelPass(Pixel pixel) : SV_TARGET
{
    float3 direction = pixel.positionWS - vector0.xyz;
    float3 directionAbs = abs(direction);
    //return float4(normalize(direction),1);
    if(directionAbs.x > directionAbs.y && directionAbs.x > directionAbs.z)
    {
        direction /= direction.x;
        direction += float3(1,1,1);
        direction /= 2;
        if(direction.x > 0)
        {
            return float4(Texture2D0.Sample(SamplerState0, float2(1 - direction.z,direction.y)).xyz,1);
        }
        else
        {
            return float4(Texture2D1.Sample(SamplerState1, float2(-direction.z,direction.y)).xyz,1);
        }
    }
    else if(directionAbs.y > directionAbs.x && directionAbs.y > directionAbs.z)
    {
        direction /= direction.y;
        if(direction.y > 0)
        {
            return float4(Texture2D2.Sample(SamplerState2, float2(direction.x,-direction.z)).xyz,1);
        }
        else
        {
            return float4(Texture2D3.Sample(SamplerState3, float2(direction.x,direction.z)).xyz,1);
        }
    }
    else if(directionAbs.z > directionAbs.x && directionAbs.z > directionAbs.y)
    {
        direction /= direction.z;
        if(direction.z > 0)
        {
            return float4(Texture2D4.Sample(SamplerState4, float2(direction.x,direction.y)).xyz,1);
        }
        else
        {
            return float4(Texture2D5.Sample(SamplerState5, float2(-direction.x,direction.y)).xyz,1);
        }
    }

    return 0;
}
