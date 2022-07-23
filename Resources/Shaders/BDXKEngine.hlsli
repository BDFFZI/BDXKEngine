cbuffer WorldInfo : register(b0)
{
    float4 Environment;
};

cbuffer CameraInfo : register(b1)
{
    row_major float4x4 WorldToCamera;
    row_major float4x4 CameraToView;
    float4 CameraPosition;
}

cbuffer LightInfo : register(b2)
{
    float4 LightPosition;
    float4 LightNormal;
    float4 LightColor;
    float4 LightFactorFloat;
    int4 LightFactorInt; //x:LightType,y:lightRenderMode,z:lightOrder,w:Пе
};

cbuffer ObjectInfo : register(b3)
{
    row_major float4x4 LocalToWorld;
}

cbuffer Parameters : register(b4)
{
    float4 Parameter0;
    float4 Parameter1;
    float4 Parameter2;
    float4 Parameter3;
};

Texture2D Texture2D0 : register(t0);
Texture2D Texture2D1 : register(t1);
Texture2D Texture2D2 : register(t2);
Texture2D Texture2D3 : register(t3);
SamplerState Texture2DSampler : register(s0);

struct Vertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

float3 LocalToWorldVector(float3 verctor)
{
    float4x4 localToWorldVector = LocalToWorld;
    localToWorldVector._14_24_34_44 = float4(0, 0, 0, 1);
    
    return (float3) mul(localToWorldVector, float4(verctor, 1));
}