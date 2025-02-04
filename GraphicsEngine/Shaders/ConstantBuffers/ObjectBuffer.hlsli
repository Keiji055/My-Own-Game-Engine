cbuffer ObjectBuffer : register(b1)
{
	float4x4        OB_Transform;
    bool            OB_HasBones;
    unsigned int    OB_ObjectID;
    bool            OB_IsInstanced = false;
    float           OB_Padding;
    float4x4        OB_BoneTransforms[128];
}