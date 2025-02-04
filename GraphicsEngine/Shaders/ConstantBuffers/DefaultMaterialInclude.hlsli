Texture2D albedoTexture             : register(t0);
Texture2D normalTexture             : register(t1);
Texture2D materialTexture           : register(t2);
Texture2D FXTexture                 : register(t3);
Texture2D dirLightShadowMap         : register(t110);
Texture2D BRDF_LUT_Texture          : register(t127);

TextureCube environmentCube         : register(t50);

struct DefaultMaterial
{
    float4 AlbedoColor;
    float2 UVTiling;
    float NormalStrength;
    float Shininess;
};

cbuffer DefaultMaterialBuffer : register(b2)
{
    DefaultMaterial Material;
}

struct DefaultMaterial_VStoPS
{
    float4 Position        : SV_POSITION;
    float4 LocalPosition   : LOCALPOSITION;
    float4 WorldPosition   : WORLDPOSITION;
    float4 VertexColor     : COLOR;
    float2 UV0             : TEXCOORD0;
    float2 UV1             : TEXCOORD1;
    float3 Normal          : NORMAL;
    float3 Tangent         : TANGENT;
    float3 BiNormal        : BINORMAL;
};

struct DefaultMaterial_Result
{
    float4 Color                    : SV_TARGET;
};