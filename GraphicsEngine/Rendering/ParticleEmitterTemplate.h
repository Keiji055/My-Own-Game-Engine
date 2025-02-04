#pragma once
#include <string>
#include <CommonUtilities/Transform.h>

using namespace CommonUtilities;
struct EmitterSettingsData
{
	float GravityModifier = 9.81f;

	float SpawnRate = 1.0f;

	float SpawnAngle = 0.0f;

	float LifeTime;

	Vector3f StartVelocity{ 0,10,0 };

	Vector3f EndVelocity;

	float StartSize;

	float EndSize;

	Vector4f StartColor;

	Vector4f EndColor;
};

struct ParticleEmitterTemplate
{
	std::wstring Path;
	EmitterSettingsData EmitterSettings;
};