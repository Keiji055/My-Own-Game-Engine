#include "FrameBuffer.hlsli"
#include "SamplerInclude.hlsli"
#include "PostProcessInclude.hlsli"

float4 GetViewPosition(float2 uv)
{
    const float4 worldPosition = float4(textureSlot1.Sample(defaultSampler, uv).rgb, 1);
    const float4 viewPosition = mul(FB_InvView, worldPosition);
    return viewPosition;
}

float4 GetViewNormal(float2 uv)
{
    const float4 worldNormal = float4(textureSlot2.Sample(defaultSampler, uv).rgb, 0);
    const float4 viewNormal = mul(FB_InvView, worldNormal);
    return viewNormal;
}

float SSAO(float2 screenUV, float2 uv, float3 pxViewPos, float3 pxViewNorm, float scale, float bias, float intensity)
{
    const float3 diff = GetViewPosition(screenUV + uv).xyz - pxViewPos;
    const float3 v = normalize(diff);
    const float d = length(diff) * scale;
    const float occ = max(0.0f, dot(pxViewNorm, v) - bias) * 1.0f / (1.0f + d) * intensity;
    return occ;
}

float2 GetRandom(float2 uv, float2 uvScale)
{
    const float3 random = 2.0f * textureSlot8.Sample(pointWrapSampler, uv * uvScale).rgb - 1.0f;
    return random.xy;
}

PostProcessPixelOutput main(PostProcessVS_to_PS input)
{
    PostProcessPixelOutput result;
    
    const float intensity = 0.35f;
    const float scale = 0.05f;
    const float bias = 0.05f;
    const float radius = 0.002f;
    const float offset = 0.707f;
    
    int noiseWidth = 0;
    int noiseHeight = 0;
    int numMips = 0;
    textureSlot8.GetDimensions(0, noiseWidth, noiseHeight, numMips);
    
    const float2 randomUVScale = FB_Resolution / float2(noiseWidth, noiseHeight);
    const float2 random = GetRandom(input.UV, randomUVScale);
    
    const float4 pxPos = GetViewPosition(input.UV);
    const float4 pxNrm = GetViewNormal(input.UV);
    
    const float2 frustumCorners[4] = { float2(1, 0), float2(-1, 0), float2(0, 1), float2(0, -1) };
    
    float occlusion = 0.0f;
    for (uint i = 0; i < 4; i++)
    {
        const uint idx = i % 4;
        const float2 coord1 = reflect(frustumCorners[idx], random) * radius;
        const float2 coord2 = float2(coord1.x * offset - coord1.y * offset, coord1.x * offset + coord1.y * offset);
        
        occlusion += SSAO(input.UV, coord1 * 0.25, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
        occlusion += SSAO(input.UV, coord2 * 0.5, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
        occlusion += SSAO(input.UV, coord1 * 0.75, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
        occlusion += SSAO(input.UV, coord2, pxPos.xyz, pxNrm.xyz, scale, bias, intensity);
    }
    result.color.rgb = 1 - occlusion;
    result.color.a = 1;
    return result;
}