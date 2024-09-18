float4 BRDF(float3 lightDir,float3 viewDir,float3 normal,float4 albedo,float3 lightColor,float3 environment,float metallic,float smoothness)
{
	// 漫射光+环境光
	float diffusion = max(0, dot(normal, lightDir));
	float3 diffusionColor = albedo.rgb * (lightColor * diffusion + environment * (1 - metallic));
	// 高光
	float specular = pow(max(0, dot(reflect(-lightDir, normal), viewDir)),smoothness * 128 + 1) *2;
	float3 specularColor = lightColor * specular * smoothness;
	// 混合
	return float4(diffusionColor * clamp(2 - metallic - smoothness, 0, 1) + specularColor, albedo.a);
}


#define UNITY_PI 3.14159265359f
#define UNITY_INV_PI 0.31830988618f
inline half Pow5(half x)
{
	return x * x * x * x * x;
}

// Ref: http://jcgt.org/published/0003/02/03/paper.pdf
inline float SmithJointGGXVisibilityTerm(float NdotL, float NdotV, float roughness)
{
#if 0
    // Original formulation:
    //  lambda_v    = (-1 + sqrt(a2 * (1 - NdotL2) / NdotL2 + 1)) * 0.5f;
    //  lambda_l    = (-1 + sqrt(a2 * (1 - NdotV2) / NdotV2 + 1)) * 0.5f;
    //  G           = 1 / (1 + lambda_v + lambda_l);

    // Reorder code to be more optimal
    half a          = roughness;
    half a2         = a * a;

    half lambdaV    = NdotL * sqrt((-NdotV * a2 + NdotV) * NdotV + a2);
    half lambdaL    = NdotV * sqrt((-NdotL * a2 + NdotL) * NdotL + a2);

    // Simplify visibility term: (2.0f * NdotL * NdotV) /  ((4.0f * NdotL * NdotV) * (lambda_v + lambda_l + 1e-5f));
    return 0.5f / (lambdaV + lambdaL + 1e-5f);  // This function is not intended to be running on Mobile,
                                                // therefore epsilon is smaller than can be represented by half
#else
	// Approximation of the above formulation (simplify the sqrt, not mathematically correct but close enough)
	float a = roughness;
	float lambdaV = NdotL * (NdotV * (1 - a) + a);
	float lambdaL = NdotV * (NdotL * (1 - a) + a);

#if defined(SHADER_API_SWITCH)
	return 0.5f / (lambdaV + lambdaL + UNITY_HALF_MIN);
#else
	return 0.5f / (lambdaV + lambdaL + 1e-5f);
#endif

#endif
}
inline float GGXTerm(float NdotH, float roughness)
{
	float a2 = roughness * roughness;
	float d = (NdotH * a2 - NdotH) * NdotH + 1.0f; // 2 mad
	return UNITY_INV_PI * a2 / (d * d + 1e-7f);	   // This function is not intended to be running on Mobile,
												   // therefore epsilon is smaller than what can be represented by half
}

// Note: Disney diffuse must be multiply by diffuseAlbedo / PI. This is done outside of this function.
inline half DisneyDiffuse(half NdotV, half NdotL, half LdotH, half perceptualRoughness)
{
	half fd90 = 0.5 + 2 * LdotH * LdotH * perceptualRoughness;
	// Two schlick fresnel term
	half lightScatter = (1 + (fd90 - 1) * Pow5(1 - NdotL));
	half viewScatter = (1 + (fd90 - 1) * Pow5(1 - NdotV));

	return lightScatter * viewScatter;
}

inline float3 SafeNormalize(float3 inVec)
{
	float dp3 = max(0.001f, dot(inVec, inVec));
	return inVec * rsqrt(dp3);
}

inline half3 FresnelTerm(half3 F0, half cosA)
{
	half t = Pow5(1 - cosA); // ala Schlick interpoliation
	return F0 + (1 - F0) * t;
}
inline half3 FresnelLerp(half3 F0, half3 F90, half cosA)
{
	half t = Pow5(1 - cosA); // ala Schlick interpoliation
	return lerp(F0, F90, t);
}

half4 BRDF1_Unity_PBS(half3 diffColor, half3 specColor, half oneMinusReflectivity, half smoothness,
					  float3 normal, float3 viewDir,
					  float3 lightDir, float3 lightColor,
					  float3 indirectDiffuse, float3 indirectSpecular)
{
	float perceptualRoughness = 1 - smoothness;
	float3 halfDir = SafeNormalize(float3(lightDir) + viewDir);

	half nv = abs(dot(normal, viewDir)); // This abs allow to limit artifact
	float nl = saturate(dot(normal, lightDir));
	float nh = saturate(dot(normal, halfDir));
	half lh = saturate(dot(lightDir, halfDir));

	// Diffuse term
	half diffuseTerm = DisneyDiffuse(nv, nl, lh, perceptualRoughness) * nl;

	float roughness = perceptualRoughness * perceptualRoughness;
	roughness = max(roughness, 0.002);
	float V = SmithJointGGXVisibilityTerm(nl, nv, roughness);
	float D = GGXTerm(nh, roughness);

	float specularTerm = V * D * UNITY_PI;
	specularTerm = max(0, specularTerm * nl);
	half surfaceReduction = 1.0 / (roughness * roughness + 1.0); // fade \in [0.5;1]
	specularTerm *= any(specColor) ? 1.0 : 0.0;

	half grazingTerm = saturate(smoothness + (1 - oneMinusReflectivity));
	half3 color = diffColor * (indirectDiffuse + lightColor * diffuseTerm) + specularTerm * lightColor * FresnelTerm(specColor, lh) + surfaceReduction * indirectSpecular * FresnelLerp(specColor, grazingTerm, nv);

	return half4(color, 1);
}