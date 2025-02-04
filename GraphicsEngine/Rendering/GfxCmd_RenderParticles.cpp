#include "GraphicsEngine.pch.h"
#include "GfxCmd_RenderParticles.h"

GfxCmd_RenderParticles::GfxCmd_RenderParticles(const std::shared_ptr<ParticleEmitter>& aParticleEmitter)
{
	myParticleEmitter = aParticleEmitter;
}

void GfxCmd_RenderParticles::Execute()
{
	ObjectBuffer& obj = GetObjectBuffer();
	obj.Data.HasBones = false;
	obj.Data.Transform = myParticleEmitter->GetTransform().GetMatrix();
	RHI::UpdateConstantBufferData(obj);
	RHI::SetConstantBuffer(PIPELINE_STAGE_GEOMETERY_SHADER | PIPELINE_STAGE_VERTEX_SHADER| PIPELINE_STAGE_PIXEL_SHADER,1, obj);
	myParticleEmitter->SetAsResource();
	myParticleEmitter->Draw();

	
}
