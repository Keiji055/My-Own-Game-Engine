#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include "Material.h"

Material::Material(const std::shared_ptr<Shader>& aVertexShader, const std::shared_ptr<Shader>& aPixelShader)
{
	myVertexShader = aVertexShader;
	myPixelShader = aPixelShader;
}

void Material::SetAsResource()
{
	for (int i = 0; i < myTextures.size(); i++)
	{
		if (!myTextures[i].Texture.expired())
		{
			std::shared_ptr<Texture> texPtr = myTextures[i].Texture.lock();
			RHI::SetTextureResource(myTextures[i].Stage, myTextures[i].Slot, texPtr.get());
		}
	}

	MaterialBuffer material;
	
	material.Data.AlbedoColor = CommonUtilities::Vector4f(1, 1, 1, 1);
	material.Data.UVTiling = CommonUtilities::Vector2f(1, 1);
	material.Data.NormalStrength = 1.0f;
	material.Data.Shininess = 1.0f;
	material.Initialize();
	RHI::UpdateConstantBufferData(material);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, material);

	if (!myVertexShader.expired())
	{
		std::shared_ptr<Shader> VSPtr = myVertexShader.lock();
		RHI::SetVertexShader(VSPtr.get());
	}
	if (!myPixelShader.expired())
	{
		std::shared_ptr<Shader> PSPtr = myPixelShader.lock();
		RHI::SetPixelShader(PSPtr.get());
	}
}

void Material::SetTexture(const std::wstring& aName, const std::weak_ptr<Texture>& aTexture)
{
	MaterialTextureBinding newTexture;
	newTexture.Name = aName;
	newTexture.Texture = aTexture;
	newTexture.Slot = mySlotCounter % 4;
	newTexture.Stage = PIPELINE_STAGE_PIXEL_SHADER;
	myTextures.push_back(newTexture);
	myTextureNameIndex.emplace(aName, mySlotCounter);
	mySlotCounter++;
}

void Material::ChangeTexture(const std::wstring& aName, const std::weak_ptr<Texture>& aTexture, int aSlot)
{
	MaterialTextureBinding newTexture;
	newTexture.Name = aName;
	newTexture.Texture = aTexture;
	newTexture.Slot = aSlot;
	newTexture.Stage = PIPELINE_STAGE_PIXEL_SHADER;
	myTextures[aSlot] = newTexture;
	/*myTextureNameIndex.at(aSlot) = aTexture;
	myTextureNameIndex.at()*/
}
