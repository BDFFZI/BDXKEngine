#include "Lighting.hlsli"

struct Pixel
{
    float4 positionCS : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 positionWS : POSITION;
    float3 normalWS : NORMAL;
};

Pixel VertexPass(Vertex vertex)
{
    Pixel pixed;

    pixed.positionWS = TransformObjectToWorld(vertex.position);
    pixed.positionCS = TransformWorldToClip(pixed.positionWS);
    pixed.normalWS = normalize(TransformObjectToWorldVector(vertex.normal));
    pixed.uv = vertex.uv;
    return pixed;
}

float4 PixelPass(Pixel pixel) : SV_TARGET
{
    pixel.normalWS = normalize(pixel.normalWS);
    //外部数据
    float4 albedo = Texture2D0.Sample(SamplerState0, pixel.uv)*vector0;
    float metallic = float0_3.x;
    float smoothness = float0_3.y;
    //解码数据
    float3 environment = LightOrder == 0 ? Environment.rgb : 0;
    float3 lightColor = DecodeShadowMap(pixel.positionWS);
    float3 lightNormal = 0;
    if (LightType == 0) //平行光
    {
        lightColor *= LightColor.rgb;
        lightNormal = LightNormal.xyz;
    }
    else if (LightType == 1) //点光
    {
        lightColor *= LightColor.rgb * clamp(1 / distance(pixel.positionWS, LightPosition.xyz), 0, 1);
        lightNormal = normalize(pixel.positionWS - LightPosition.xyz);
    }

    //渲染

    //漫射光+环境光
    float diffusion = max(0, dot(-pixel.normalWS, lightNormal));
    float3 diffusionColor = albedo.rgb * (lightColor * diffusion + environment * (1 - metallic));
    //高光
    float specular = pow(max(0, dot(reflect(lightNormal, pixel.normalWS), normalize(CameraPosition.xyz - pixel.positionWS))),
                         smoothness * 128 + 1) * 2;
    float3 specularColor = albedo.rgb * lightColor * specular * smoothness;
    //混合
    return float4(diffusionColor * clamp(2 - metallic - smoothness, 0, 1) + specularColor, albedo.a);
}
