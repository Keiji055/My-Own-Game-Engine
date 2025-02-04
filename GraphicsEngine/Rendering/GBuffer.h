#pragma once
#include "../Rendering/Texture.h"
struct GBufferData 
{
	std::shared_ptr<Texture> Albedo;
	std::shared_ptr<Texture> PixelNormal;
	std::shared_ptr<Texture> Material;
	std::shared_ptr<Texture> VertexNormal;
	std::shared_ptr<Texture> WorldPosition;
	std::shared_ptr<Texture> FXTexture;
};