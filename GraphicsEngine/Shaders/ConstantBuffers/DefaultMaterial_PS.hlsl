#include "DefaultMaterialInclude.hlsli"
#include "SamplerInclude.hlsli"
#include "FrameBuffer.hlsli"
#include "ObjectBuffer.hlsli"

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
    const float3 iblDiffuse = environmentCube.SampleLevel(defaultSampler, aNormal, aNumMips).rgb;
    return iblDiffuse;
}

float3 CalculateSpecularIBL(float3 aSpecularColor, float3 aNormal, float3 aToView, float aRoughMap, TextureCube anEnvironmentCube, int aNumMips)
{
    const float3 R = reflect(-aToView, aNormal);
    const float3 envColor = anEnvironmentCube.SampleLevel(defaultSampler, R, aRoughMap * aNumMips).rgb;
    const float NdotV = saturate(dot(aNormal, aToView));
    const float2 brdfLUT = BRDF_LUT_Texture.Sample(BRDF_LUT_Sampler, float2(NdotV, aRoughMap)).rg;
    const float3 iblSpecular = envColor * (aSpecularColor * brdfLUT.x + brdfLUT.y);
    return iblSpecular;
}

DefaultMaterial_Result main(DefaultMaterial_VStoPS input)
{
    DefaultMaterial_Result result;
    result.Color = 0;
    result.Color.a = 1;
    
    const float2 scaledUV = input.UV0 * Material.UVTiling;
    
    const float4 albedoMap = albedoTexture.Sample(defaultSampler, scaledUV);
    const float2 normalMap = normalTexture.Sample(defaultSampler, scaledUV).rg;
    const float4 materialMap = materialTexture.Sample(defaultSampler, scaledUV);
    const float4 FXMap = FXTexture.Sample(defaultSampler, scaledUV);
    
    const float metalMap = materialMap.b;
    const float roughMap = materialMap.g;
    const float occlusionMap = materialMap.r;
    
    const float4 albedoColor = Material.AlbedoColor * albedoMap * input.VertexColor;
    const float3 specularColor = lerp((float3) 0.04f, albedoColor.rgb, metalMap);
    const float3 diffuseColor = lerp((float3) 0.00f, albedoColor.rgb, 1 - metalMap);
    
    float3 pixelNormal = float3(normalMap, 0);

    pixelNormal.rg = 2.0f * normalMap.rg - 1.0f;
    pixelNormal.b = sqrt(1 - saturate(dot(pixelNormal.rg, pixelNormal.rg)));
    
    pixelNormal = normalize(pixelNormal);
    pixelNormal.rg *= Material.NormalStrength;
    
    const float3x3 TBN = float3x3
    (
        normalize(input.Tangent),
        normalize(input.BiNormal),
        normalize(input.Normal)
    );
    
    pixelNormal = normalize(mul(pixelNormal, TBN));
    
    const int cubeMips = GetNumMips(environmentCube) - 1;
   
    const float3 V = normalize(FB_ViewPosition - input.WorldPosition.xyz);
    float3 diffuseIBL = CalculateDiffuseIBL(pixelNormal, environmentCube, cubeMips);
    float3 specularIBL = CalculateSpecularIBL(specularColor, pixelNormal, V, roughMap, environmentCube, cubeMips);
    
    const float3 kA = (diffuseColor * diffuseIBL + specularIBL) * occlusionMap; 
    
    const float4 emission = FXMap.r * albedoColor;
    
    const float3 radiance = kA + emission.rgb;
    
    result.Color.rgb = radiance;
    return result;
}


