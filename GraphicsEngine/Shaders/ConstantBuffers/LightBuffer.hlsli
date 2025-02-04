

struct DirectionalLightData
{
    float4x4    ProjectionMatrix;
    float4x4    ViewMatrix;
    float3      Position;
    float       padding0;
    float3      Color;
    float       Intensity;
    float3      Direction;
    float       padding1;
};

struct PointLightData
{
    float3  Color;
    float   Intensity;
    float3  Position;
    float   Range;
};

struct SpotLightData
{
    float3  Color;
    float   Intensity;
    float3  Position;
    float   Range;
    float3  Direction;
    float   ConeRadius;
    float   Penumbra;
    float3  padding;
};

cbuffer LightBuffer : register(b3)
{
    DirectionalLightData    DirectionalLight;
    PointLightData          PointLights[8];
    SpotLightData           SpotLights[8];
}