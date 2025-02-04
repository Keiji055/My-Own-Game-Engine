#pragma once
#include "GameObjectData.h"
using namespace CommonUtilities;


class GameObject
{
public:
	virtual ~GameObject() = default;

	virtual Transform& GetTransform() { return myData.Transform; };

	virtual void SetTransform(const Vector3f aPosition) { myData.Transform.SetPosition(aPosition); };

	virtual void SetTransform(const Transform& aTransform) { myData.Transform = aTransform; };

	virtual void SetName(const std::string aName) { myData.Name = aName; };

	virtual const std::string GetName() const { return myData.Name; };

	virtual const GameObjectType& GetType() const { return myData.Type; };

	virtual void SetType(const GameObjectType& aType) { myData.Type = aType; };

	virtual const Vector4f& GetColor() const { return myData.Color; };

	virtual const Vector3f& GetDirection() const { return myData.Direction; };

	virtual const float GetRange() const { return myData.Range; };

	virtual const float& GetIntensity() const { return myData.Intensity; };

	virtual const float& GetPenumbra() const { return myData.Penumbra; };

	virtual const float& GetConeRadius() const { return myData.Cone; };

	virtual const float& GetSpeed() const { return myData.Speed; };

	virtual const std::vector<std::string>& GetTextures() const { return myData.Textures; };

	virtual void SetIntensity(const float& aValue) { myData.Intensity = aValue; };

	virtual void SetRange(const float& aValue) { myData.Range = aValue; };

	virtual void SetPenumbra(const float& aValue) { myData.Penumbra = aValue; };

	virtual void SetConeRadius(const float& aValue) { myData.Cone = aValue; };
	
	virtual void SetColor(const Vector4f& aColor) { myData.Color = aColor; };

	virtual void SetTextures(const std::vector<std::string>& someTextures) { myData.Textures = someTextures; };
	
	virtual void SetSpeed(const float& aSpeed) { myData.Speed = aSpeed; };

	/*virtual std::weak_ptr<Material>& GetMaterial() { return myData.Material; };

	virtual std::shared_ptr<Mesh>& GetMesh() { return myData.Mesh; };*/

	//static unsigned int GetID() { return myID++; }

protected:
	Data myData;
};