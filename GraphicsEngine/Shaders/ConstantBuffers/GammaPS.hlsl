#include "PostProcessInclude.hlsli"
#include "SamplerInclude.hlsli"
float4 main(PostProcessVS_to_PS aSource) : SV_TARGET
{
    float4 result;
    const float4 color = textureSlot1.Sample(defaultSampler, aSource.UV);
    result.rgb = pow(abs(color), 1.0f / 2.2f);
    result.a = 1;
    return result;
}