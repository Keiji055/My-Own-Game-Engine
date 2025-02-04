
struct DefaultVertexInput
{
    float4  Position            : POSITION;
    float4  VertexColor         : COLOR;
    float2  UV0                 : TEXCOORD0;
    float2  UV1                 : TEXCOORD1;
    uint4   BoneIDs             : BONEIDS;
    float4  BoneWeights         : BONEWEIGHTS;
    float3  Normal              : NORMAL;
    float3  Tangent             : TANGENT;
    float4x4 World              : WORLD;
    uint    InstanceID          : SV_InstanceID;
};

#ifndef FUNCTIONS_PROPERTIES
#define FUNCTIONS_PROPERTIES
float4x4 GetSkinMatrix(DefaultVertexInput input, float4x4 BoneTransforms[128])
{
    float4x4 skinmatrix = 0;
    skinmatrix += input.BoneWeights.x * BoneTransforms[input.BoneIDs.x];
    skinmatrix += input.BoneWeights.y * BoneTransforms[input.BoneIDs.y];
    skinmatrix += input.BoneWeights.z * BoneTransforms[input.BoneIDs.z];
    skinmatrix += input.BoneWeights.w * BoneTransforms[input.BoneIDs.w];
    return skinmatrix;
}

#endif