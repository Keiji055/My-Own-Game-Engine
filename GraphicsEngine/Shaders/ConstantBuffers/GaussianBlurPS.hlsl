#include "PostProcessInclude.hlsli"
#include "SamplerInclude.hlsli"
#include "FrameBuffer.hlsli"

PostProcessPixelOutput main(PostProcessVS_to_PS input)
{
    PostProcessPixelOutput result;
    
    const float Pi2 = 6.28318530718f;
    
    const float Directions = 16.0f;
    const float Quality = 4.0f;
    const float Radius = 2.0f;
    
    const float2 rad = Radius / FB_Resolution;
    
    float4 color = textureSlot3.Sample(defaultSampler, input.UV);
    
    if (color.a < 0.05f)
    {
        discard;
        result.color = 0;
        return result;
    }
    
    for (float d = 0.0f; d < Pi2; d += Pi2 / Directions)
    {
        for (float i = 1.0f / Quality; i <= 1.0f; i += 1.0f / Quality)
        {
            color += textureSlot3.Sample(defaultSampler, input.UV + float2(cos(d), sin(d)) * rad * i);
        }
    }
    
    color /= Quality * Directions - 15.0f;
    
    result.color.rgb = color.rgb;
    result.color.a = 1.0f;
    return result;
}