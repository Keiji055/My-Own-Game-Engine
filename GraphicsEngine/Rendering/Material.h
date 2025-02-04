#pragma once

#include "ConstantBuffer.h"
#include "Texture.h"
#include "../Shaders/Shader.h"
#include <CommonUtilities/Vector.hpp>
#include "../InterOp/RHI.h"
#include <memory>

struct MaterialBufferData
{
	CommonUtilities::Vector4f	AlbedoColor;
	CommonUtilities::Vector2f	UVTiling;
	float						NormalStrength;
	float						Shininess;
};

typedef ConstantBuffer<MaterialBufferData> MaterialBuffer;

class Material
{
	struct MaterialTextureBinding
	{
		unsigned Slot{};
		unsigned Stage{};
		std::wstring Name;
		std::weak_ptr<Texture> Texture;
	};

	std::weak_ptr<Shader> myVertexShader;
	std::weak_ptr<Shader> myPixelShader;

	std::vector<MaterialTextureBinding> myTextures;
	std::unordered_map<std::wstring, size_t> myTextureNameIndex;

	std::weak_ptr<MaterialBuffer> myMaterialBufferData;

	unsigned myPipelineStages = 0;
	unsigned mySlotCounter = 0;

public:

	Material() = default;

	Material(const std::shared_ptr<Shader>& aVertexShader, const std::shared_ptr<Shader>& aPixelShader);

	void SetAsResource();

	void SetTexture(const std::wstring& aName, const std::weak_ptr<Texture>& aTexture);

	void ChangeTexture(const std::wstring& aName, const std::weak_ptr<Texture>& aTexture, int aSlot);
};
