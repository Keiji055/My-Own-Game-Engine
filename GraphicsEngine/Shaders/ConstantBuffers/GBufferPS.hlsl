#include "DefaultMaterialInclude.hlsli"
#include "SamplerInclude.hlsli"

struct GBufferOutput
{
    float4 Albedo           : SV_TARGET0;
    float4 PixelNormal      : SV_TARGET1;
    float4 Material         : SV_TARGET2;
    float4 VertexNormal     : SV_TARGET3;
    float4 WorldPosition    : SV_Target4;
    float4 FXTexture        : SV_Target5;
};

GBufferOutput main(DefaultMaterial_VStoPS input)
{
    GBufferOutput result;
    
    const float2 scaledUV = input.UV0 * Material.UVTiling;
    
    const float4 albedoMap = albedoTexture.Sample(defaultSampler, scaledUV);
    const float2 normalMap = normalTexture.Sample(defaultSampler, scaledUV).rg;
    const float4 materialMap = materialTexture.Sample(defaultSampler, scaledUV);
    const float4 FXMap = FXTexture.Sample(defaultSampler, scaledUV);
    
    const float metalMap = materialMap.b;
    const float roughMap = materialMap.g;
    const float occlusionMap = materialMap.r;
    
    const float4 albedoColor = Material.AlbedoColor * albedoMap * input.VertexColor;
    
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
   
    result.Albedo = albedoColor;
    result.Material.r = occlusionMap;
    result.Material.g = roughMap;
    result.Material.b = metalMap;
    result.Material.a = 0;
    result.VertexNormal.r = input.Normal.r;
    result.VertexNormal.g = input.Normal.g;
    result.VertexNormal.b = input.Normal.b;
    result.VertexNormal.a = 0;
    result.WorldPosition = input.WorldPosition;
    result.PixelNormal.rgb = pixelNormal;
    result.PixelNormal.a = 0;
    result.FXTexture = FXMap;
    
    return result;
}