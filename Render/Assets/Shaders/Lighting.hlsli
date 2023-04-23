#include "Core.hlsli"

//当前灯光信息
cbuffer LightInfo : register(b2)
{
float4 LightPosition;//平行光：灯光照射方向，其他光：灯光位置
float4 LightColor;
float4x4 WorldToViewLight;
float4x4 ViewToClipLight;
int LightOrder;
int LightType;
}
Texture2D ShadowMap : register(t6);
SamplerState ShadowMapSampler : register(s6);
TextureCube ShadowMapCube : register(t7);
SamplerState ShadowMapCubeSampler : register(s7);


bool IsInShadow(float3 worldPosition)
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
        depthOfMap = SampleTex2DLinear(ShadowMap,ShadowMapSampler,TransformClipToUV(clip)).z;
        depth = view.z;
    }

    if (depth > depthOfMap + 0.05f)
        return 1;
    else
        return 0;
}
//面向光源的方向
float3 GetLightDirection(float3 positionWS)
{
    if (LightType == 0) //平行光
        return -LightPosition.xyz;
    else
        return normalize(LightPosition.xyz - positionWS);
}
//获取灯光颜色，会自动处理灯光强弱和阴影的问题
float3 GetLightColor(float3 positionWS)
{
    if(IsInShadow(positionWS) == true)
        return 0;

    if (LightType == 0) //平行光
        return LightColor.rgb;
    else if (LightType == 1) //点光
        return LightColor.rgb * clamp(1 / distance(positionWS, LightPosition.xyz), 0, 1);

    return 0;
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