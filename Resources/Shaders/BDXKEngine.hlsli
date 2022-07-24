//顶点
struct Vertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};
//世界
cbuffer WorldInfo : register(b0)
{
    float4 Environment;
    float4 Time;
};
//相机
cbuffer CameraInfo : register(b1)
{
    row_major float4x4 WorldToView;
    row_major float4x4 ViewToClip;
    float4 CameraPosition;
}
//灯光
cbuffer LightInfo : register(b2)
{
    float4 LightPosition;
    float4 LightNormal;
    float4 LightColor;
    float4 LightFactorFloat;
    int4 LightFactorInt; //x:LightType,y:lightRenderMode,z:lightOrder,w:空
};
//物体
cbuffer ObjectInfo : register(b3)
{
    row_major float4x4 ObjectToWorld;
}
//材质
cbuffer Parameters : register(b4)
{
    float4 Parameter0;
    float4 Parameter1;
    float4 Parameter2;
    float4 Parameter3;
};
//阴影
cbuffer ShadowInfo : register(b5)
{
    row_major float4x4 WorldToLightView;
    row_major float4x4 ViewToLightClip;
}
Texture2D ShadowMap : register(t4);


Texture2D Texture2D0 : register(t0);
Texture2D Texture2D1 : register(t1);
Texture2D Texture2D2 : register(t2);
Texture2D Texture2D3 : register(t3);
SamplerState Texture2DSampler : register(s0);


float3 ObjectToWorldVector(float3 worldVector)
{
    float4x4 ObjectToWorldVector = ObjectToWorld;
    ObjectToWorldVector._14_24_34_44 = float4(0, 0, 0, 1);
    
    return (float3) mul(ObjectToWorldVector, float4(worldVector, 1));
}
float4 ObjectToClipPos(float3 objectPosition)
{
    float4 svPosition = float4(objectPosition, 1);
    svPosition = mul(ObjectToWorld, svPosition);
    svPosition = mul(WorldToView, svPosition);
    svPosition = mul(ViewToClip, svPosition);
    return svPosition;
}
float2 ClipToUVPos(float4 clipPosition)
{
    float2 result = clipPosition.xy / clipPosition.w;
    result.x = result.x / 2 + 0.5f;
    result.y = result.y / -2 + 0.5f;
    return result;
}

float DecodeShadowMap(float3 worldPosition)
{
    float4 view = mul(WorldToLightView, float4(worldPosition, 1));
    float4 clip = mul(ViewToLightClip, view);
    float4 viewOfMap = ShadowMap.Sample(Texture2DSampler, ClipToUVPos(clip));

    if (view.z > viewOfMap.z + 0.01f)
        return 0;
    else
        return 1;
}