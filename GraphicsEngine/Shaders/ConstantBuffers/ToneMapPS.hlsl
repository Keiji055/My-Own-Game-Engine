#include "PostProcessInclude.hlsli"
#include "SamplerInclude.hlsli"
#include "Tonemaps.hlsli"

PostProcessPixelOutput main(PostProcessVS_to_PS input)
{
    PostProcessPixelOutput result;
    
    const float3 color = textureSlot1.Sample(defaultSampler, input.UV).rgb;
    
    result.color.rgb = Tonemap_UnrealEngine(color);
    result.color.a = 1;
    
    return result;
}