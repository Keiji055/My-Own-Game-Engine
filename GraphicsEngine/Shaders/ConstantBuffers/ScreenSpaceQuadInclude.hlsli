struct ScreenSpaceQuad_VStoPS
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

struct ScreenSpaceQuad_Result
{
    float4 Color : SV_TARGET;
};