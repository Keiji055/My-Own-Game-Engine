#include "PostProcessInclude.hlsli"
#include "SamplerInclude.hlsli"
PostProcessPixelOutput main(PostProcessVS_to_PS input)
{
    PostProcessPixelOutput result;
    
    const float3 resource1 = textureSlot1.Sample(defaultSampler, input.UV).rgb;
    const float3 resource2 = textureSlot2.Sample(defaultSampler, input.UV).rgb;
    
    const float luminance = dot(resource1, float3(0.2126f, 0.7152f, 0.0722f));
    const float3 scaledResource = resource2 * (1.0f - luminance);
    
    result.color.rgb = resource1 + scaledResource;
    result.color.a = 1;
    return result;
}