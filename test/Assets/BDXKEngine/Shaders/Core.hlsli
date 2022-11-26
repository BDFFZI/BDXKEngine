struct Vertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

//当前材质球信息
cbuffer MaterialInfo : register(b0)
{
float4 float0_3;//metallic,smoothness
float4 float4_7;
float4 vector0;
float4 vector1;
float4 vector2;
float4 vector3;
float4 vector4;
float4 vector5;
float4x4 matrix0;//objectToWorld
float4x4 matrix1;
float4x4 matrix2;
float4x4 matrix3;
};
Texture2D Texture2D0 : register(t0);//albedo
SamplerState SamplerState0 : register(s0);
Texture2D Texture2D1 : register(t1);
SamplerState SamplerState1 : register(s1);
Texture2D Texture2D2 : register(t2);
SamplerState SamplerState2 : register(s2);
Texture2D Texture2D3 : register(t3);
SamplerState SamplerState3 : register(s3);
//当前相机信息
cbuffer CameraInfo : register(b1)
{
float4x4 WorldToView;
float4x4 ViewToClip;
float4 CameraPosition;
float4 Environment;
float4 Time;
};
TextureCube Skybox : register(t4);
SamplerState SkyboxSampler : register(s4);

float4 TransformObjectToWorld(float3 positionOS)
{
    return mul(matrix0, float4(positionOS, 1));
}
float4 TransformWorldToClip(float3 positionWS)
{
    float4 positionCS = float4(positionWS, 1);
    positionCS = mul(WorldToView, positionCS);
    positionCS = mul(ViewToClip, positionCS);
    return positionCS;
}
float4 TransformObjectToClip(float3 positionOS)
{
    float4 positionCS = float4(positionOS, 1);
    positionCS = mul(matrix0, positionCS);
    positionCS = mul(WorldToView, positionCS);
    positionCS = mul(ViewToClip, positionCS);
    return positionCS;
}
float2 TransformClipToUV(float4 clipPosition)
{
    float2 result = clipPosition.xy / clipPosition.w;
    result.x = result.x / 2 + 0.5f;
    result.y = result.y / -2 + 0.5f;
    return result;
}

float3 TransformObjectToWorldVector(float3 vectorWS)
{
    float4x4 ObjectToWorldVector = matrix0;
    ObjectToWorldVector._14_24_34_44 = float4(0, 0, 0, 1);

    return (float3)mul(ObjectToWorldVector, float4(vectorWS, 1));
}

//float3 DecodeSkybox(float3 worldPosition)
//{
//    return ;
//}
