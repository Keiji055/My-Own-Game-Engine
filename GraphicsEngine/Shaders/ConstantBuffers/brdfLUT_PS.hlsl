#include "brdfLUTInclude.hlsli"
#include "IBLBRDF.hlsli"

float2 main(BRDF_VStoPS input) : SV_TARGET
{
    float2 integratedBRDF = IntegrateBRDF(input.UV.x, input.UV.y);
    return integratedBRDF;
}