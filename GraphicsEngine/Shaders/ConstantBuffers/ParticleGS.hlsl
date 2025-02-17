#include "ParticleShaderInclude.hlsli"
#include "FrameBuffer.hlsli"
[maxvertexcount(4)]
void main(
	point ParticleVertexData input[1],
	inout TriangleStream<ParticleGeometryToPixel> output
)
{
    const float2 offsets[4] =
    {
        { -1.0f, 1.0f },
        { 1.0f, 1.0f },
        { -1.0f, -1.0f },
        { 1.0f, -1.0f }
    };
	
    const float2 uvs[4] =
    {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f }
    };
	
    const ParticleVertexData inputParticle = input[0];
	
    for (uint index = 0; index < 4; index++)
    {
        ParticleGeometryToPixel result;
        result.Position = mul(FB_InvView, inputParticle.Position);
        result.Position.xy += offsets[index] * inputParticle.Scale.xy;
        result.Position = mul(FB_Projection, result.Position);
        result.Color = inputParticle.Color;
        result.LifeTime = inputParticle.LifeTime;
        result.UV = uvs[index];
        result.Velocity = inputParticle.Velocity;
        output.Append(result);
    }
}