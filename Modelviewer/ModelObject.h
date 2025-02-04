#pragma once
#include "GameObject.h"
#include "../GraphicsEngine/GraphicsEngine.h"
#include "../GraphicsEngine/Objects/Mesh.h"
#include "../GraphicsEngine/Rendering/Texture.h"
#include "../AssetManager/TextureAssetHandler.h"

class ModelObject : public GameObject
{
public:
	ModelObject() = delete;
	ModelObject(CommonUtilities::Vector3f aPosition);
	~ModelObject();
	void Init();
	void SetMesh(std::shared_ptr<Mesh>& aMesh);
	const std::shared_ptr<Mesh>& GetMesh() const;
	/*Transform& GetTransform() override;
	void SetTransform(Vector3f aPosition) override;
	void SetTransform(const Transform& aTransform) override;
	void SetName(const std::string aName);
	const std::string GetName() const;*/
private:
	std::shared_ptr<Mesh> myMesh;
};
