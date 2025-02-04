#pragma once
#include <GraphicsCommand.h>
#include <GraphicsEngine/Rendering/ConstantBuffer.h>
#include <GraphicsEngine/Rendering/Vertex.h>
#include <memory>
class GfxCmd_SetMaterialBuffer : public GraphicsCommand
{
	CommonUtilities::Vector4f myAlbedoColor;
	CommonUtilities::Vector2f myUVTiling;
	float myNormalStrength;
	float myShininess;
public:

	GfxCmd_SetMaterialBuffer(const CommonUtilities::Vector4f& anAlbedoColor,
		const CommonUtilities::Vector2f& aUVTiling,
		float aNormalStrength,
		float aShininess);

	void Execute() override;
};