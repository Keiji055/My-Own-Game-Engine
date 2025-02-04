#include "DeferredMaterialInclude.hlsli"
#include "SamplerInclude.hlsli"
#include "PointLightBuffer.hlsli"
#include "FrameBuffer.hlsli"

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

float3 CalculatePointLight(float3 aPixelNormal, float3 aPosition, PointLightData aPointLight, float3 aSpecularColor, float3 aDiffuseColor, float aRoughness)
{
    float3 V = normalize(FB_ViewPosition - aPosition);
    float3 dist = aPointLight.Position.rgb - aPosition;
    float3 L = normalize(dist);
    float3 H = normalize(L + V);
    float3 lightColorAndIntensity = aPointLight.Color.rgb * aPointLight.Intensity;
    
    float3 kS = SpecularBRDF(aRoughness, aPixelNormal, H, V, L, aSpecularColor);
    float3 kD = DiffuseBRDF(aDiffuseColor);
    
    float r = max(aPointLight.Range, 0.00001f);
    float calculatedAtenuation = saturate(length(dist) * (1.0f / r));
    float Apl = 1 - calculatedAtenuation * calculatedAtenuation;
    
    float3 rp = (kD + kS) * lightColorAndIntensity * Apl;
    return rp;
}

float4 main(ScreenSpaceQuad_VStoPS input) : SV_TARGET
{
    float4 result;
    
    const float4 albedo = albedoTexture.Sample(defaultSampler, input.UV);
    const float3 normal = normalTexture.Sample(defaultSampler, input.UV).rgb;
    const float4 material = materialTexture.Sample(defaultSampler, input.UV);
    const float4 vxNormal = vertexNormalTexture.Sample(defaultSampler, input.UV);
    const float4 worldPos = worldPositionTexture.Sample(defaultSampler, input.UV);
    
    const float metalMap = material.b;
    const float roughMap = material.g;
    const float occlusionMap = material.r;
    
    const float4 albedoColor = albedo;
    const float3 specularColor = lerp((float3) 0.04f, albedoColor.rgb, metalMap);
    const float3 diffuseColor = lerp((float3) 0.00f, albedoColor.rgb, 1 - metalMap);
    
    const float3 totalLightValue = CalculatePointLight(normal, worldPos.xyz, PointLight, specularColor, diffuseColor, roughMap); 
    
    const float3 radiance = totalLightValue;
    
    //result.rgb = pow(abs(radiance), 1.0f / 2.2f);
    result.rgb = radiance;
    result.a = 1;
    
    return result;
}