#include "ParticleShaderInclude.hlsli"
#include "ObjectBuffer.hlsli"

ParticleVertexData main(ParticleVertexData input)
{
    ParticleVertexData result;
    
    const float4 particleLocalPosition = input.Position;
    const float4 particleWorldPosition = mul(OB_Transform, particleLocalPosition);

    result.Position = particleWorldPosition;
    result.Color = input.Color;
    result.LifeTime = input.LifeTime;
    result.Scale = input.Scale;
    result.Velocity = input.Velocity;
    
    return result;
}