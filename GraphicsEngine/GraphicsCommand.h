#pragma once
#include <GraphicsEngine/Rendering/ConstantBuffer.h>
#include <GraphicsEngine.h>
#include <GraphicsEngine/Rendering/Vertex.h>

using namespace CommonUtilities;

class GraphicsCommand
{
protected:
	FrameBuffer&		GetFrameBuffer();
	ObjectBuffer&		GetObjectBuffer();
	LightBuffer&		GetLightBuffer();
	MaterialBuffer&		GetMaterialBuffer();
	PointLightBuffer&	GetPointLightBuffer();
	SpotLightBuffer&	GetSpotLightBuffer();

public:
	virtual ~GraphicsCommand() = default;
	virtual void Execute() = 0;
};
