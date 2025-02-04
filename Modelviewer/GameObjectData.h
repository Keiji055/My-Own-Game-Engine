#pragma once
#include "GlobalEnums.h"
#include "../GraphicsEngine/Objects/Mesh.h"
#include <CommonUtilities/Transform.h>
#include <string>
using namespace CommonUtilities;

struct Data
{
	GameObjectType Type;
	std::string Name;
	
	Transform Transform;
	Vector3f Position;
	Vector3f Rotation;
	Vector3f Scale;
	Vector3f Direction;

	std::vector<std::string> Textures;

	Vector4f Color;
	float Speed;
	float Intensity;
	float Range;
	float Penumbra;
	float Cone;

	//std::shared_ptr<Mesh> Mesh;
	//std::weak_ptr<Material> Material;
};