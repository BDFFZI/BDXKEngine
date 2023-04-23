#include "Lighting.hlsli"

struct Pixel
{
    float3 positionWS : POSITION;
    float3 normalWS:NORMAL;
    float2 uv : TEXCOORD;

    float4 positionCS : SV_POSITION;

};

Pixel VertexPass(Vertex vertex)
{
    Pixel pixed;

    pixed.positionWS = TransformObjectToWorld(vertex.position);
    pixed.normalWS = TransformObjectToWorldDir(vertex.normal);
    pixed.uv = vertex.uv;

    pixed.positionCS = TransformWorldToClip(pixed.positionWS);
    return pixed;
}

float4 PixelPass(Pixel pixel) : SV_TARGET
{
    //解包基本数据
    float4 albedo = SampleTex2D(Texture2D0,SamplerState0,pixel.uv) * vector0;
    float4 pbrFactor = SampleTex2D(Texture2D1,SamplerState1,pixel.uv);
    float metallic = pbrFactor.x * float0_3.x;
    float smoothness = pbrFactor.w * float0_3.y;
    float3 normalWS = pixel.normalWS;
    //解包灯光数据
    float3 environment = LightOrder == 0 ? Environment.rgb : 0;
    float3 lightColor = GetLightColor(pixel.positionWS);
    float3 light2Object = -GetLightDirection(pixel.positionWS);

    //渲染

    //漫射光+环境光
    float diffusion = max(0, dot(-normalWS, light2Object));
    float3 diffusionColor = albedo.rgb * (lightColor * diffusion + environment * (1 - metallic));
    //高光
    float specular = pow(max(0, dot(reflect(light2Object, normalWS), normalize(CameraPosition.xyz - pixel.positionWS))),
                         smoothness * 128 + 1) * 2;
    float3 specularColor = albedo.rgb * lightColor * specular * smoothness;
    //混合
    return float4(diffusionColor * clamp(2 - metallic - smoothness, 0, 1) + specularColor, albedo.a);
}
