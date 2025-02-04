#include "DefaultMaterialInclude.hlsli"
#include "DefaultVertexInput.hlsli"
#include "FrameBuffer.hlsli"
#include "ObjectBuffer.hlsli"

DefaultMaterial_VStoPS main(DefaultVertexInput input)
{
    float4 localPosition = input.Position;
    const float3x3 worldNormalRotation = (float3x3) OB_Transform;
	
    DefaultMaterial_VStoPS result;
    
    result.Normal = mul(worldNormalRotation, input.Normal);
    result.Tangent = mul(worldNormalRotation, input.Tangent);
    float4x4 skinMatrix = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    if (OB_HasBones)
    {
        skinMatrix = GetSkinMatrix(input, OB_BoneTransforms);
        localPosition = mul(localPosition, skinMatrix);
        const float3x3 skinNormalRotation = (float3x3) transpose(skinMatrix);
        
        result.Normal = mul(skinNormalRotation, input.Normal);
        result.Tangent = mul(skinNormalRotation, input.Tangent);
    }
    
    result.BiNormal = cross(result.Normal, result.Tangent);
    
    float4 vertexWorldPosition = mul(OB_Transform, mul(input.Position, skinMatrix));
    
    if (OB_IsInstanced)
    {
        vertexWorldPosition = mul(input.World, input.Position/*mul(input.Position, skinMatrix)*/);
        vertexWorldPosition = mul(OB_Transform, vertexWorldPosition);
    }
    
    //result.Position = mul(OB_Transform, localPosition);
    result.LocalPosition = localPosition;
    result.WorldPosition = vertexWorldPosition;
    result.Position = mul(FB_InvView, vertexWorldPosition);
    result.Position = mul(FB_Projection, result.Position);
    result.VertexColor = input.VertexColor;
    result.UV0 = input.UV0;
    result.UV1 = input.UV1;
    return result;
}