Texture2D       Texture1        : register(t0);
Texture2D       NormalTexture   : register(t1);

struct DefaultVertexInput
{
	float4	Position		: POSITION;
	float4	VxColor			: COLOR;
    float2	UV				: UV;
    uint4	BoneIDs			: BONEIDS;
    float4	BoneWeights		: BONEWEIGHTS;
    float3	Normal		    : NORMAL;
    float3	Tangent         : TANGENT;
    
    //Instansing properties
    float4x4 World          : WORLD;
    uint InstanceId         : SV_InstanceID;
};

struct DefaultVertexToPixel
{
    float4  Position        : SV_POSITION;
    float4  WorldPosition   : WORLDPOSITION;
	float4	VxColor			: COLOR;
    float2	UV				: UV;
    float3  Normal          : NORMAL;
    float3  Tangent         : TANGENT;
    float3  BiNormal        : BINORMAL;
};

struct DefaultPixelOutput
{
	float4 Color		    : SV_TARGET;
};