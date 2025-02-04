#include "ModelObject.h"

ModelObject::ModelObject(CommonUtilities::Vector3f aPosition)
{
	myData.Transform.SetPosition(aPosition);
}

ModelObject::~ModelObject()
{
}

void ModelObject::Init()
{
}


void ModelObject::SetMesh(std::shared_ptr<Mesh>& aMesh)
{
	myMesh = aMesh;
}

const std::shared_ptr<Mesh>& ModelObject::GetMesh() const
{
	return myMesh;
}


