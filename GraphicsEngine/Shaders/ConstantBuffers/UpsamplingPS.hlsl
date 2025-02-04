#include "PostProcessInclude.hlsli"
#include "SamplerInclude.hlsli"

float4 main(PostProcessVS_to_PS aSource) : SV_TARGET
{
    const float4 color = textureSlot3.Sample(defaultSampler, aSource.UV);
    return color;
}