#pragma once
#include <wrl.h>
#include "ParticleEmitterTemplate.h"
#include "../../AssetManager/TextureAssetHandler.h"
#include <CommonUtilities/MathFunctions.hpp>
#include "../Shaders/Include/ParticleGS.h"
#include "../Shaders/Include/ParticleVS.h"
#include "../Shaders/Include/ParticlePS.h"
#include "ParticleVertex.h"
#include "Texture.h"
#include "../InterOp/RHI.h"
using namespace CommonUtilities;
using namespace Microsoft::WRL;

class ParticleEmitter
{
	friend class ParticleAssetHandler;

public:
	virtual ~ParticleEmitter() = default;

	virtual bool Init(const ParticleEmitterTemplate& aTemplate, Vector3f& aPosition);
	virtual void Update(float aDeltaTime);

	virtual void SetAsResource() const;
	virtual void Draw() const;

	FORCEINLINE const Transform& GetTransform() const { return myTransform; }
	FORCEINLINE const EmitterSettingsData& GetEmitterSettings() const { return myEmitterSettings; }

private:
	void InitParticle(size_t aParticleIndex);

	EmitterSettingsData myEmitterSettings;

	std::vector<ParticleVertex> myParticles;
	UINT myStride;
	UINT myOffset;
	UINT myPrimitiveTopology;

	ComPtr<ID3D11Buffer> myVertexBuffer;
	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11GeometryShader> myGeometryShader;
	ComPtr<ID3D11PixelShader> myPixelShader;
	ComPtr<ID3D11InputLayout> myInputLayout;
	
	std::shared_ptr<Texture> myTexture;
	Transform myTransform;
	//Vector3f myPosition;
};