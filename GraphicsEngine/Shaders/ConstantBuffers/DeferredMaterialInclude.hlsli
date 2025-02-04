#include "ScreenSpaceQuadInclude.hlsli"

////Default sampler to read textures with:
//SamplerState defaultSampler             : register(s0);
//SamplerComparisonState shadowCmpSampler : register(s14);
//SamplerState BRDF_LUT_Sampler           : register(s15);

//Slots for our GBuffer Textures
Texture2D albedoTexture             : register(t0);
Texture2D normalTexture             : register(t1);
Texture2D materialTexture           : register(t2);
Texture2D vertexNormalTexture       : register(t3);
Texture2D worldPositionTexture      : register(t4);
Texture2D FXTexture                 : register(t5);
//There will be more GBuffer textures later

TextureCube environmentCube         : register(t50);

Texture2D dirLightShadowMap         : register(t110);
Texture2D BRDF_LUT_Texture          : register(t127);

