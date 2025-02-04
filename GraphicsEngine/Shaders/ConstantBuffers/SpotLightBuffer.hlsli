struct SpotLightData
{
    float3 Color;
    float Intensity;
    float3 Position;
    float Range;
    float3 Direction;
    float ConeRadius;
    float Penumbra;
    float3 padding;
};

cbuffer SpotLightBuffer : register(b6)
{
    SpotLightData SpotLight;
}