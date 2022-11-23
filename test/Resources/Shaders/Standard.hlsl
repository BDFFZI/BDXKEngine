#include "BDXKEngine.hlsli"

struct Pixel
{
    float4 svPosition : SV_POSITION;
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

Pixel VertexPass(Vertex vertex)
{
    Pixel pixed;
    float4 svPosition = float4(vertex.position, 1);
    
    svPosition = mul(ObjectToWorld, svPosition);
    pixed.position = svPosition.xyz;
    
    svPosition = mul(WorldToView, svPosition);
    svPosition = mul(ViewToClip, svPosition);
    pixed.svPosition = svPosition;
    
    pixed.normal = ObjectToWorldVector(vertex.normal);//TODO 会受缩放影响
    pixed.uv = vertex.uv;
    return pixed;
}

float4 PixelPass(Pixel pixel) : SV_TARGET
{
    pixel.normal = normalize(pixel.normal);
    //外部数据
    float4 albedo = Texture2D0.Sample(SamplerState0, pixel.uv);
    float metallic = float0_3.x;
    float smoothness = float0_3.y;
    //计算数据
    float3 environment = 0;
    float3 lightNormal = 0;
    float3 lightColor = 0;
    if (LightType == 0)//平行光
    {
        lightNormal = LightNormal.xyz;
        lightColor = LightColor.rgb;
    }
    else if (LightType == 1)//点光
    {
        lightNormal = normalize(pixel.position - LightPosition.xyz);
        lightColor = LightColor.rgb * clamp(1 / distance(pixel.position, LightPosition.xyz), 0, 1);
    }
    lightColor *= DecodeShadowMap(pixel.position);
    environment = LightOrder == 0 ? Environment.rgb : 0;
    //渲染
    
    //漫射光+环境光
    float diffusion = max(0, dot(-pixel.normal, lightNormal));
    float3 diffusionColor = albedo.rgb * (lightColor * diffusion + environment * (1 - metallic));
    //高光
    float specular = pow(max(0, dot(reflect(lightNormal, pixel.normal), normalize(CameraPosition.xyz - pixel.position))), smoothness * 128 + 1) * 2;
    float3 specularColor = albedo.rgb * lightColor * specular * smoothness;
    //混合
    return float4(diffusionColor * clamp(2 - metallic - smoothness, 0, 1) + specularColor, albedo.a);
}