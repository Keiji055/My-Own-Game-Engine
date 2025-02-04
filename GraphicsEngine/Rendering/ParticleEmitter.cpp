#include "GraphicsEngine.pch.h"
#include "ParticleEmitter.h"

bool ParticleEmitter::Init(const ParticleEmitterTemplate& aTemplate, Vector3f& aPosition)
{
	//myPosition = aPosition;
	myTexture = std::make_shared<Texture>();
	myTransform.SetPosition(aPosition);
	myEmitterSettings = aTemplate.EmitterSettings;

	myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	std::srand((unsigned)time(NULL));
	const size_t maxNumberOfParticles = static_cast<size_t>(ceilf(myEmitterSettings.SpawnRate * myEmitterSettings.LifeTime));

	myParticles.resize(maxNumberOfParticles);
	RHI::CreateDynamicVertexBuffer(myVertexBuffer, maxNumberOfParticles, sizeof(ParticleVertex));
	RHI::CreateVertexShader(myVertexShader, BuiltIn_ParticleVS_ByteCode, sizeof(BuiltIn_ParticleVS_ByteCode));
	RHI::CreatePixelShader(myPixelShader, BuiltIn_ParticlePS_ByteCode, sizeof(BuiltIn_ParticlePS_ByteCode));
	RHI::CreateGeometryShader(myGeometryShader, BuiltIn_ParticleGS_ByteCode, sizeof(BuiltIn_ParticleGS_ByteCode));

	RHI::CreateInputLayout(myInputLayout, ParticleVertex::InputLayoutDefinition, BuiltIn_ParticleVS_ByteCode, sizeof(BuiltIn_ParticleVS_ByteCode));

	for (size_t i = 0; i < myParticles.size(); i++)
	{
		InitParticle(i);
	}

	if (!TextureAssetHandler::GetInstance().LoadTextureFromFile(aTemplate.Path, myTexture.get()))
	{
		return false;
	}

	
	return true;
}

void ParticleEmitter::Update(float aDeltaTime)
{
	for (size_t p = 0; p < myParticles.size(); p++)
	{
		ParticleVertex& particle = myParticles[p];

		if (particle.LifeTime <= 0)
		{
			InitParticle(p);
		}

		particle.Velocity.y -= myEmitterSettings.GravityModifier * aDeltaTime;
		particle.Position += CU::Vector4f(particle.Velocity * aDeltaTime, 0);
		particle.LifeTime -= aDeltaTime;
		particle.Color.x = Mathf::Lerp(myEmitterSettings.StartColor.x, myEmitterSettings.EndColor.x, 0.5f);
		particle.Color.y = Mathf::Lerp(myEmitterSettings.StartColor.y, myEmitterSettings.EndColor.y, 0.5f);
		particle.Color.z = Mathf::Lerp(myEmitterSettings.StartColor.z, myEmitterSettings.EndColor.z, 0.5f);
		particle.Color.w = Mathf::Lerp(myEmitterSettings.StartColor.w, myEmitterSettings.EndColor.w, 0.5f);
		particle.Scale = Vector3f(myEmitterSettings.StartSize, myEmitterSettings.StartSize, myEmitterSettings.StartSize);

		if (particle.LifeTime <= 0)
		{
			particle.Color.w = 0;
		}
	}
}

void ParticleEmitter::SetAsResource() const
{
	HRESULT result = S_FALSE;

	D3D11_MAPPED_SUBRESOURCE bufferData;

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = RHI::Context->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);

	memcpy_s(bufferData.pData, sizeof(ParticleVertex) * myParticles.size(), &myParticles[0], sizeof(ParticleVertex) * myParticles.size());

	RHI::Context->Unmap(myVertexBuffer.Get(), 0);

	//Set Topology, Input layout and vertex buffer

	RHI::ConfigureInputAssembler(myPrimitiveTopology, myVertexBuffer, nullptr, sizeof(ParticleVertex), myInputLayout);

	RHI::Context->VSSetShader(myVertexShader.Get(), nullptr, 0);
	RHI::Context->GSSetShader(myGeometryShader.Get(), nullptr, 0);
	RHI::Context->PSSetShader(myPixelShader.Get(), nullptr, 0);

	if (myTexture)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0, myTexture.get());
	}
}

void ParticleEmitter::Draw() const
{
	RHI::Draw(myParticles.size());
	RHI::SetGeometryShader(nullptr);
	/*for (int p = 0; p < myParticles.size(); p++)
	{
		RHI::Draw(p);
	}*/
}

void ParticleEmitter::InitParticle(size_t aParticleIndex)
{
	float randomX = -10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-10))));
	float randomZ = -10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-10))));
	float randomY = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - 1)));
	myParticles[aParticleIndex].Position = { 0,0,0,1 };
	myParticles[aParticleIndex].Color	 =	myEmitterSettings.StartColor;
	myParticles[aParticleIndex].LifeTime =	myEmitterSettings.LifeTime;
	myParticles[aParticleIndex].Velocity = { randomX, myEmitterSettings.StartVelocity.y + randomY, randomZ };
	myParticles[aParticleIndex].Scale	 =
	{
		myEmitterSettings.StartSize,
		myEmitterSettings.StartSize,
		myEmitterSettings.StartSize
	};
}
