#include "brdfLUTInclude.hlsli"

BRDF_VStoPS main(unsigned int aVertexIndex : SV_VertexID)
{
    const float4 pos[4] =
    {
        float4(-1, -1, 0, 1),
        float4(-1, 1, 0, 1),
        float4(1, -1, 0, 1),
        float4(1, 1, 0, 1)  
    };
    
    const float2 uv[4] =
    {
        float2(0.0f, 1.0f),
        float2(0.0f, 0.0f),
        float2(1.0f, 1.0f),
        float2(1.0f, 0.0f)
    };
    
    BRDF_VStoPS returnValue;
    returnValue.Position = pos[aVertexIndex];
    returnValue.UV = uv[aVertexIndex];
    return returnValue;
}