
struct BRDF_VStoPS
{
    float4 Position : SV_POSITION;
    float2 UV       : TEXCOORD0;
};

struct BRDF_Result
{
    float4 Color    : SV_TARGET;
};