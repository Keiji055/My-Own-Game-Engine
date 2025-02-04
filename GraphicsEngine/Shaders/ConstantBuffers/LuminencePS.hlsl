#include "PostProcessInclude.hlsli"
#include "SamplerInclude.hlsli"

PostProcessPixelOutput main(PostProcessVS_to_PS input)
{
    PostProcessPixelOutput result;
    const float4 color = textureSlot1.Sample(defaultSampler, input.UV);
    
    if (color.a < 0.05f)
    {
        discard;
        result.color.rgb = 0;
    }
    const float3 resource = color.rgb;
    
    const float luminence = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
    const float cutOff = 0.8f;
    
    if (luminence >= cutOff)
    {
        result.color.rgb = resource;
    }
    else if (luminence >= cutOff * 0.5f)
    {
        float fade = luminence / cutOff;
        fade = pow(fade, 5);
        result.color.rgb = resource * fade;
    }
    else
    {
        result.color.rgb = 0;
    }
    result.color.a = 1;
    return result;
}