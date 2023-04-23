#include "Lighting.hlsli"
#include "BRDF.hlsli"

struct Pixel
{
    float3 positionWS : POSITION;
    float2 uv : TEXCOORD;
    float3x3 tSpace : NORMAL;

    float4 positionCS : SV_POSITION;
};

Pixel VertexPass(Vertex vertex)
{
    Pixel pixed;

    pixed.positionWS = TransformObjectToWorld(vertex.position);
    pixed.uv = vertex.uv;
    float3 normalWS = TransformObjectToWorldDir(vertex.normal);
    float3 tangentWS = TransformObjectToWorldDir(vertex.tangent.xyz);
    float3 bitangentWS = cross(normalWS, tangentWS) * vertex.tangent.w;
    pixed.tSpace = float3x3(
        tangentWS.x, bitangentWS.x, normalWS.x,
        tangentWS.y, bitangentWS.y, normalWS.y,
        tangentWS.z, bitangentWS.z, normalWS.z);

    pixed.positionCS = TransformWorldToClip(pixed.positionWS);
    return pixed;
}

float4 PixelPass(Pixel pixel) : SV_TARGET
{
    // 材质数据
    float4 albedo = SampleTex2D(Texture2D0, SamplerState0, pixel.uv) * vector0;
    float4 pbrFactor = SampleTex2D(Texture2D1, SamplerState1, pixel.uv);
    float metallic = pbrFactor.x * float0_3.x;
    float smoothness = pbrFactor.w * float0_3.y;

    // 物体数据
    float3 normalTex = mul(pixel.tSpace, UnpackNormal(SampleTex2D(Texture2D2, SamplerState2, pixel.uv)));
    float3 normalWS = normalize(lerp(pixel.tSpace._13_23_33, normalTex, float0_3.z));
    float3 viewDir = normalize(CameraPosition.xyz - pixel.positionWS);

    // 灯光数据
    float3 environment = Environment.rgb;
    float3 lightColor = GetLightColor(pixel.positionWS);
    float3 lightDirection = GetLightDirection(pixel.positionWS);

    return BRDF(lightDirection, viewDir, normalWS, albedo, lightColor, LightOrder == 0 ? environment : 0, metallic, smoothness);
    // return BRDF1_Unity_PBS(albedo*(1-metallic),lightColor,0,smoothness,normalWS,viewDir,lightDirection,lightColor,LightType==0?environment:0,0);
}
