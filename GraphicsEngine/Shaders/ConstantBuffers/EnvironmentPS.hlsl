#include "DeferredMaterialInclude.hlsli"
#include "SamplerInclude.hlsli"
#include "PostProcessInclude.hlsli"
#include "FrameBuffer.hlsli"
#include "LightBuffer.hlsli"

float NormalDistributionFunction_GGX(float aRoughness, float aNDotH)
{
    float alpha = pow(pow(aRoughness, 2), 2);
    return alpha / (PI * pow(pow(aNDotH, 2) * (alpha - 1) + 1, 2));
}

float GeometricAttenuation_Schlick_GGX(float aDot, float aK)
{
    return aDot / (aDot * (1 - aK) + aK);
}

float GeometricAttenuation_Smith(float nDotV, float nDotL, float aRoughness)
{
    float k = (pow(aRoughness + 1, 2)) / 8;
    float G1l = GeometricAttenuation_Schlick_GGX(nDotL, k);
    float G1v = GeometricAttenuation_Schlick_GGX(nDotV, k);
    float G = G1l * G1v;
    return G;
}

float3 Fresnel_SphericalGaussianSchlick(float aVDotH, float3 aSpecularColor)
{
    float power = ((-5.55473 * aVDotH) - 6.98316) * aVDotH;
    float3 F = aSpecularColor + (1 - aSpecularColor) * pow(2, power);
    return F;
}

float3 SpecularBRDF(float aRoughness, float3 aNormal, float3 aH, float3 aV, float3 anL, float3 aSpecularColor)
{
    float vDotH = saturate(dot(aV, aH));
    float nDotH = saturate(dot(aNormal, aH));
    float nDotL = saturate(dot(aNormal, anL));
    float nDotV = saturate(dot(aNormal, aV));
    
    float D = NormalDistributionFunction_GGX(aRoughness, nDotH);
    float G = GeometricAttenuation_Smith(nDotV, nDotL, aRoughness);
    float3 F = Fresnel_SphericalGaussianSchlick(vDotH, aSpecularColor);
    
    float3 kS = (D * F * G) / (4 * nDotL * nDotV + 0.0001f);
    return kS;
}

float3 DiffuseBRDF(float3 aDiffuseColor)
{
    float3 kD = aDiffuseColor / PI;
    return kD;
}

int GetNumMips(TextureCube cubeTex)
{
    int iWidth = 0;
    int iHeight = 0;
    int numMips = 0;
    cubeTex.GetDimensions(0, iWidth, iHeight, numMips);
    return numMips;
}

float3 CalculateDiffuseIBL(float3 aNormal, TextureCube anEnvironmentCube, int aNumMips)
{
    const float3 iblDiffuse = anEnvironmentCube.SampleLevel(defaultSampler, aNormal, aNumMips).rgb;
    return iblDiffuse;
}

float3 CalculateSpecularIBL(float3 aSpecularColor, float3 aNormal, float3 aToView, float aRoughMap, TextureCube anEnvironmentCube, int aNumMips)
{
    const float3 R = normalize(reflect(-aToView, aNormal));
    const float3 envColor = anEnvironmentCube.SampleLevel(defaultSampler, R, aRoughMap * aNumMips).rgb;
    const float NdotV = saturate(dot(aNormal, aToView));
    const float2 brdfLUT = BRDF_LUT_Texture.Sample(BRDF_LUT_Sampler, float2(NdotV, aRoughMap)).rg;
    const float3 iblSpecular = envColor * (aSpecularColor * brdfLUT.x + brdfLUT.y);

    return iblSpecular;
}

float3 CalculateDirectLight(float3 aPixelNormal, float4 aPosition, float3 aSpecularColor, float3 aDiffuseColor, float aRoughness)
{
	float3 L = -DirectionalLight.Direction;
	float3 V = normalize(FB_ViewPosition - aPosition.xyz);
	float3 H = normalize(L + V);
    
	float nDotL = saturate(dot(aPixelNormal, L));
    
	float3 kS = SpecularBRDF(aRoughness, aPixelNormal, H, V, L, aSpecularColor);
	float3 kD = DiffuseBRDF(aDiffuseColor);
	kD *= (1.0f - kS);

	float4 lightSpacePos = mul(DirectionalLight.ViewMatrix, aPosition);
	lightSpacePos = mul(DirectionalLight.ProjectionMatrix, lightSpacePos);
    
	float3 lightSpaceUV = lightSpacePos.xyz / lightSpacePos.w;
	float shadowBias = 0.005f;
	float D = lightSpaceUV.z - shadowBias;
	lightSpaceUV.xy = lightSpaceUV.xy * 0.5f + 0.5f;
	lightSpaceUV.y = 1 - lightSpaceUV.y;
    
	float shadow = dirLightShadowMap.SampleCmpLevelZero(shadowCmpSampler, lightSpaceUV.xy, D).r;
    
	float3 lightColorAndIntensity = DirectionalLight.Color.rgb * DirectionalLight.Intensity * shadow;
    
	const float3 radiance = (kD + kS) * lightColorAndIntensity * nDotL;
    
	if (lightSpaceUV.x < 0 || lightSpaceUV.x > 1 || lightSpaceUV.y < 0 || lightSpaceUV.y > 1)
	{
		lightSpaceUV.xy = 0;
	}
    
	float3 test = float3(lightSpaceUV.xy, 0.0f);
	return radiance;
}


float4 main(ScreenSpaceQuad_VStoPS input) : SV_TARGET
{
    float4 result;
    
    const float4 albedo = albedoTexture.Sample(defaultSampler, input.UV);
    if (albedo.a < 0.05f)
    {
        discard;
    }
    const float3 normal = normalTexture.Sample(defaultSampler, input.UV).rgb;
    const float4 material = materialTexture.Sample(defaultSampler, input.UV);
    const float4 vxNormal = vertexNormalTexture.Sample(defaultSampler, input.UV);
    const float4 worldPos = worldPositionTexture.Sample(defaultSampler, input.UV);
    const float4 FXMap = FXTexture.Sample(defaultSampler, input.UV);
    
    const float metalMap = material.b;
    const float roughMap = material.g;
    const float occlusionMap = material.r;
    
    const float3 specularColor = lerp((float3) 0.04f, albedo.rgb, metalMap);
    const float3 diffuseColor = lerp((float3) 0.00f, albedo.rgb, 1 - metalMap);
    
    const int cubeMips = GetNumMips(environmentCube) - 1;
    
    const float3 V = normalize(FB_ViewPosition - worldPos.xyz);
    float3 diffuseIBL = CalculateDiffuseIBL(normal, environmentCube, cubeMips);
    float3 specularIBL = CalculateSpecularIBL(specularColor, normal, V, roughMap, environmentCube, cubeMips);
    
    float3 kA = (diffuseColor * diffuseIBL + specularIBL) * occlusionMap;
    
    const float ssaoValue = textureSlot9.Sample(defaultSampler, input.UV).r;

    kA *= ssaoValue/ 16;
    
    const float3 totalLightValue = CalculateDirectLight(normal, worldPos, specularColor, diffuseColor, roughMap) + kA;
    
    const float4 emission = FXMap.r * albedo;
    
    const float3 radiance = totalLightValue + emission.rgb;
    
    //result.rgb = pow(abs(radiance), 1.0f / 2.2f);
    result.rgb = radiance;
    result.a = 1;
    
	return result;
}