#include "VertexShader.hlsl"

float4 main(Pixel pixel) : SV_TARGET
{
    pixel.normal = normalize(pixel.normal);
    //�ⲿ����
    float4 albedo = Texture2D0.Sample(Texture2DSampler, pixel.uv);
    float metallic = Parameter0.x;
    float smoothness = Parameter0.y;
    //��������
    float3 lightNormal = 0;
    float3 lightColor = 0;
    if (LightFactorInt.x == 0)//ƽ�й�
    {
        lightNormal = LightNormal.xyz;
        lightColor = LightColor.rgb;
    }
    else if (LightFactorInt.x == 1)//���
    {
        lightNormal = normalize(pixel.position - LightPosition.xyz);
        lightColor = LightColor.rgb * clamp(1 / distance(pixel.position, LightPosition.xyz), 0, 1);
    }
    lightColor *= DecodeShadowMap(pixel.position);
    //��Ⱦ
    
    //�����+������
    float diffusion = max(0, dot(-pixel.normal, lightNormal));
    float3 diffusionColor = albedo.rgb * (lightColor * diffusion + Environment.rgb * (1 - metallic));
    //�߹�
    float specular = pow(max(0, dot(reflect(lightNormal, pixel.normal), normalize(CameraPosition.xyz - pixel.position))), smoothness * 128 + 1) * 2;
    float3 specularColor = albedo.rgb * lightColor * specular * smoothness;
    //���
    return float4(diffusionColor * clamp(2 - metallic - smoothness, 0, 1) + specularColor, albedo.a);
}