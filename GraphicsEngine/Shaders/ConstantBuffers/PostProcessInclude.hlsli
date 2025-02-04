

//Texture2D textureSlot1 : register(t10);
//Texture2D textureSlot2 : register(t11);
//Texture2D textureSlot3 : register(t12);
Texture2D textureSlot1 : register(t0);
Texture2D textureSlot2 : register(t1);
Texture2D textureSlot3 : register(t2);
Texture2D textureSlot8 : register(t8);
Texture2D textureSlot9 : register(t9);

struct PixelCopySource
{
    float2 UV       : TEXCOORD0;
};

struct PostProcessPixelOutput
{
    float4 color    : SV_TARGET;
};

struct PostProcessVS_to_PS
{
    float4 Position : SV_POSITION;
    float2 UV       : TEXCOORD0;
};