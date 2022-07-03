#include "VertexShader.hlsl"

float4 main(Pixel pixel) : SV_TARGET
{
    pixel.normal = normalize(pixel.normal);
    float3 albedo = texture2D0.Sample(texture2DSampler, pixel.uv);
    float metallic = Parameter0.x;
    float smoothness = Parameter0.y;
    float3 lightNormal;
    float3 lightColor;
    if (LightInfos.x == 0)//平行光
    {
        lightNormal = LightNormal.xyz;
        lightColor = LightColor.rgb;
    }
    else if (LightInfos.x == 1)//点光
    {
        lightNormal = normalize(pixel.position - LightPosition.xyz);
        lightColor = LightColor.rgb * clamp(1 / distance(pixel.position, LightPosition.xyz), 0, 1);
    }
    
    float diffusion = max(0, dot(-pixel.normal, lightNormal));
    float specular = max(0, dot(reflect(lightNormal, pixel.normal), normalize(CameraPosition.xyz - pixel.position)));
    specular = pow(specular, smoothness * 128 + 1) * 2;
        
    float3 diffusionColor = albedo * (lightColor * diffusion + Environment.rgb * (1 - metallic));
    float3 specularColor = albedo * lightColor * specular * smoothness;
        
    return float4(diffusionColor * clamp(2 - metallic - smoothness, 0, 1) + specularColor, 1);
}