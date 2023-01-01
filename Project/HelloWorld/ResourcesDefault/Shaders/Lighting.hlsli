#include "Core.hlsli"

//当前灯光信息
cbuffer LightInfo : register(b2)
{
float4 LightPosition;
float4 LightNormal;
float4 LightColor;
float4x4 WorldToViewLight;
float4x4 ViewToClipLight;
int LightOrder;
int LightType;
}
Texture2D ShadowMap : register(t5);
SamplerState ShadowMapSampler : register(s5);
TextureCube ShadowMapCube : register(t6);
SamplerState ShadowMapCubeSampler : register(s6);


float DecodeShadowMap(float3 worldPosition)
{
    float depth;
    float depthOfMap;

    if (LightType == 1)
    {
        depthOfMap = ShadowMapCube.Sample(ShadowMapCubeSampler, worldPosition - LightPosition.xyz).z;
        depth = distance(worldPosition, LightPosition.xyz);
    }
    else
    {
        float4 view = mul(WorldToViewLight, float4(worldPosition, 1));
        float4 clip = mul(ViewToClipLight, view);
        depthOfMap = ShadowMap.Sample(ShadowMapSampler, TransformClipToUV(clip)).z;
        depth = view.z;
    }

    if (depth > depthOfMap + 0.01f)
        return 0;
    else
        return 1;
}
float4 TransformObjectToView_Light(float3 positionOS)
{
    float4 positionLS = float4(positionOS, 1);
    positionLS = mul(matrix0, positionLS);
    positionLS = mul(WorldToViewLight, positionLS);
    return positionLS;
}

float4 TransformViewToClip_Light(float4 positionVS)
{
    return mul(ViewToClipLight, positionVS);
}