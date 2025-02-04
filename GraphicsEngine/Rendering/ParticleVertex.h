#pragma once
#include <CommonUtilities/Vector.hpp>
#include <d3d11.h>
using namespace Microsoft::WRL;
using namespace CommonUtilities;

typedef std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementList;

struct ParticleVertex
{
	Vector4f	Position = { 0,0,0,1 };
	Vector4f	Color = { 0,0,0,0 };
	Vector3f	Velocity = { 0,0,0 };
	Vector3f	Scale = { 1,1,1 };
	float		LifeTime = 0;


	static const InputElementList InputLayoutDefinition;
	static ComPtr<ID3D11InputLayout> InputLayout;
};