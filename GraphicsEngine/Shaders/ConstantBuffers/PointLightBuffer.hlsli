struct PointLightData
{
    float3 Color;
    float Intensity;
    float3 Position;
    float Range;
};

cbuffer PointLightBuffer : register(b5)
{
    PointLightData PointLight;
}