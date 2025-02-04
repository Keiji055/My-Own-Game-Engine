
static const float PI = 3.14159265f;

cbuffer FrameBuffer : register(b0)
{
	float4x4	FB_InvView;
	float4x4	FB_Projection;
    float3		FB_ViewPosition;
    float       padding;
    float2      FB_Resolution;
    float2      padding2;
}