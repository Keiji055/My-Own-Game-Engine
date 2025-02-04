#pragma once
#include <memory>

#include "ApplicationState.h"
#include "GlobalEnums.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "Logging/Logging.h"
#include <Modelviewer/CameraObject.h>
#include <Modelviewer/ModelObject.h>
#include <Modelviewer/LightObject.h>
#include <CommonUtilities/InputHandler.h>
#include <CommonUtilities/Matrix4x4.hpp>
#include <CommonUtilities/Timer.h>
#include <AssetManager/ModelAssetHandler.h>
#include <AssetManager/AnimationAssetHandler.h>
#include <AssetManager/TextureAssetHandler.h>
#include <AssetManager/ParticleAssetHandler.h>
#include <Modelviewer/AnimatedObject.h>
#include "../../GraphicsEngine/Rendering/ParticleEmitter.h"
#include "../Editor/Editor_ImGui.h"
#include <codecvt>
#include <string>
#include <StringConverter.h>
#include <rapidjson.h>
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"

#include <cstdio>
#include <iostream>

class SplashWindow;
class Texture;

class ModelViewer
{
	HINSTANCE myModuleHandle = nullptr;
	HWND myMainWindowHandle = nullptr;
	SplashWindow* mySplashWindow = nullptr;

	ApplicationState myApplicationState;

	Logger myLogger;

	ModelViewer() = default;

	void ShowSplashScreen();
	void HideSplashScreen() const;

	void InitializeGameObjectTypes();

	void LoadScene();
	void UpdateScene();
	const std::shared_ptr<GameObject>& GetSelectedObject(int anIndex);
	void Update();

	void ClearRenderTargets();
	CommonUtilities::Timer							myTimer;
	std::shared_ptr<CommonUtilities::InputHandler>	myInputHandler;
	std::shared_ptr<CameraObject>					myCamera;

	std::vector<std::shared_ptr<ModelObject>>		myModels;
	std::shared_ptr<AnimatedObject>					myAnimatedObject;
	std::shared_ptr<Mesh>							myInstancedMesh;
	std::vector<std::shared_ptr<Mesh>>				myMeshes;
	std::vector < std::shared_ptr<AnimatedObject>>	myAnimatedObjects;
	std::vector<std::weak_ptr<Texture>>				myTextures;
	std::vector<std::shared_ptr<Material>>			myMaterials;
	std::vector<std::shared_ptr<GameObject>>		myGOlist;
	std::weak_ptr<Texture> myBlueNoiseTexture;
	std::weak_ptr<Texture>							myCubeMap;
	std::shared_ptr<Material>						myDefaultMaterial;
	//std::shared_ptr<Animation>						myWalkAnimation;

	std::vector<std::shared_ptr<LightObject>>		myDirectionalLights;
	std::vector<std::shared_ptr<LightObject>>		myPointLights;
	std::vector<std::shared_ptr<LightObject>>		mySpotLights;
	std::shared_ptr<ParticleEmitter>				myParticleEmitter;

	std::unordered_map<std::string, GameObjectType> myGameObjectIds;

	MaterialBufferData								myMaterialBufferData;

public:

	// Singleton Getter.
	static ModelViewer& Get() { static ModelViewer myInstance; return myInstance; }

	// Acceleration Getters for components.
	FORCEINLINE static ApplicationState& GetApplicationState() { return Get().myApplicationState; }
	FORCEINLINE static Logger& GetLogger() { return Get().myLogger; }
	void DisableInputHandler(bool aShouldBeDisabled);
	void LoadGameObjects(std::string aPath);
	void CreateGameObject(std::string aGOName);
	size_t LoadAllAssets();
	const std::vector<std::shared_ptr<GameObject>>& GetGOList() const;
	const std::vector<std::weak_ptr<Texture>>& GetTextureList() const;
	const std::unordered_map<std::string, GameObjectType>& GetGOIDs() const;
	void SaveScene();

	bool Initialize(HINSTANCE aHInstance, SIZE aWindowSize, WNDPROC aWindowProcess, LPCWSTR aWindowTitle, std::shared_ptr<CommonUtilities::InputHandler>& anInputHandler);
	int Run();
};
