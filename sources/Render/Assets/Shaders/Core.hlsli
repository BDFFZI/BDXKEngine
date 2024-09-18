struct Vertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
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
Texture2D Texture2D4 : register(t4);
SamplerState SamplerState4 : register(s4);
Texture2D Texture2D5 : register(t5);
SamplerState SamplerState5 : register(s5);
//当前相机信息
cbuffer CameraInfo : register(b1)
{
float4x4 WorldToView;
float4x4 ViewToClip;
float4 CameraPosition;
float4 Environment;
float4 Time;
};

float4 SampleTex2DLinear(Texture2D tex2D,SamplerState sampleState,float2 uv)
{
    return tex2D.Sample(sampleState, float2(uv.x,1-uv.y));//调整为OpenGL的UV坐标系，原点在左下角
}
float4 SampleTex2D(Texture2D tex2D,SamplerState sampleState,float2 uv)
{
    return pow(SampleTex2DLinear(tex2D,sampleState,uv),1/0.45);//默认图片保存的伽马值是1/2.2=0.45，我们将他转换线性空间
}

float3 TransformObjectToWorld(float3 positionOS)
{
    return mul(matrix0, float4(positionOS, 1)).xyz;
}
float3 TransformObjectToWorldDir(float3 vectorOS)//不受位置和缩放影响
{
    float4x4 ObjectToWorldVector = matrix0;
    ObjectToWorldVector._14_24_34_44 = float4(0, 0, 0, 1);

    return normalize((float3)mul(ObjectToWorldVector, float4(vectorOS, 1)));
}
float4 TransformObjectToClip(float3 positionOS)
{
    float4 positionCS = float4(positionOS, 1);
    positionCS = mul(matrix0, positionCS);
    positionCS = mul(WorldToView, positionCS);
    positionCS = mul(ViewToClip, positionCS);
    return positionCS;
}
float4 TransformWorldToClip(float3 positionWS)
{
    float4 positionCS = float4(positionWS, 1);
    positionCS = mul(WorldToView, positionCS);
    positionCS = mul(ViewToClip, positionCS);
    return positionCS;
}
float2 TransformClipToUV(float4 clipPosition)
{
    float2 result = clipPosition.xy / clipPosition.w;
    result.x = result.x / 2 + 0.5f;
    result.y = result.y / 2 + 0.5f;
    return result;
}
float3 UnpackNormal(float4 tex2DNormal)
{
    return tex2DNormal.xyz * 2 -1;
}



//float3 DecodeSkybox(float3 worldPosition)
//{
//    return ;
//}
