
// Exclude things we don't need from the Windows headers
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include "Modelviewer.h"
#include <filesystem>
#include <GraphicsEngine/GfxCmd_RenderMesh.h>
#include <GraphicsEngine/Rendering/GfxCmd_RenderMeshShadow.h>
#include <GraphicsEngine/GfxCmd_SetTextureResource.h>
#include <GraphicsEngine/GfxCmd_SetCubeMapResource.h>
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/InterOp/Helpers.h"
#include "Windows/SplashWindow.h"
#include <GraphicsEngine/Rendering/GfxCmd_SetFrameBuffer.h>
#include <GraphicsEngine/Rendering/GfxCmd_SetLightBuffer.h>
#include <GraphicsEngine/Rendering/GfxCmd_SetPointLightBuffer.h>
#include <GraphicsEngine/Rendering/GfxCmd_SetSpotLightBuffer.h>
#include <GraphicsEngine/Rendering/GfxCmd_SetMaterialBuffer.h>
#include <GraphicsEngine/Rendering/GfxCmd_SetRenderTarget.h>
#include <GraphicsEngine/Rendering/GfxCmd_ChangeDepthStencilState.h>
#include <GraphicsEngine/Rendering/GfxCmd_RenderDeferredDataPass.h>
#include <GraphicsEngine/Rendering/GfxCmd_LuminencePass.h>
#include <GraphicsEngine/Rendering/GfxCmd_SSAODataPass.h>
#include <GraphicsEngine/Rendering/GfxCmd_BloomPass.h>
#include <GraphicsEngine/Rendering/GfxCmd_GammaPass.h>
#include <GraphicsEngine/Rendering/GfxCmd_ToneMapPass.h>
#include <GraphicsEngine/Rendering/GfxCmd_CopyPixelPass.h>
#include <GraphicsEngine/Rendering/GfxCmd_GaussianBlurPass.h>
#include <GraphicsEngine/Rendering/GfxCmd_RenderParticles.h>
#include <GraphicsEngine/Rendering/GfxCmd_UpsamplePass.h>
#include <GraphicsEngine/Rendering/GfxCmd_SetDeferredRenderTarget.h>
#include <GraphicsEngine/Rendering/GfxCmd_SetDeferredTextureResources.h>
#include <GraphicsEngine/Rendering/GfxCmd_ChangeBlendState.h>
#include <GraphicsEngine/Rendering/GfxCmd_EnvironmentLightPass.h>
#include <GraphicsEngine/Rendering/GfxCmd_PointLightPass.h>
#include <GraphicsEngine/Rendering/GfxCmd_SpotLightPass.h>
#include <GraphicsEngine/Rendering/GfxCmd_ClearDepthStencil.h>
#include <stringapiset.h>

#define PI 3.14159265359f

bool ModelViewer::Initialize(HINSTANCE aHInstance, SIZE aWindowSize, WNDPROC aWindowProcess, LPCWSTR aWindowTitle, std::shared_ptr<CommonUtilities::InputHandler>& anInputHandler)
{
	InitializeGameObjectTypes();
	CommonUtilities::Timer::Reset();
	myInputHandler = anInputHandler;
	myLogger = Logger::Create("ModelViewer");
	myModuleHandle = aHInstance;

	constexpr LPCWSTR windowClassName = L"ModelViewerMainWindow";

	// First we create our Window Class
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = aWindowProcess;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = windowClassName;
	RegisterClass(&windowClass);

	// Then we use the class to create our window
	myMainWindowHandle = CreateWindow(
		windowClassName,                                // Classname
		aWindowTitle,                                    // Window Title
		WS_OVERLAPPEDWINDOW | WS_POPUP,    // Flags
		(GetSystemMetrics(SM_CXSCREEN) - aWindowSize.cx) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - aWindowSize.cy) / 2,
		aWindowSize.cx,
		aWindowSize.cy,
		nullptr, nullptr, nullptr,
		nullptr
	);

	ShowSplashScreen();

	// TODO: Here we should init the Graphics Engine.
	GraphicsEngine::Get().Initialize(myMainWindowHandle, true);
	ModelAssetHandler::GetInstance().Initialize();
	AnimationAssetHandler::GetInstance().Initialize();

	myMaterialBufferData.AlbedoColor = CommonUtilities::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	myMaterialBufferData.UVTiling = CommonUtilities::Vector2f(1.0f, 1.0f);
	myMaterialBufferData.NormalStrength = 1.0f;
	myMaterialBufferData.Shininess = 1.0f;
	Editor_ImGui::Get().Initialize(myMainWindowHandle, RHI::Device.Get(), RHI::Context.Get(), myInputHandler);

	LoadScene();

	HideSplashScreen();

	return true;
}

int ModelViewer::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	bool isRunning = true;

	while (isRunning)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{

				isRunning = false;
			}
		}


		// REMEMBER!
		// The frame update for the game does NOT happen inside the PeekMessage loop.
		// This would cause the game to only update if there are messages and also run
		// the update several times per frame (once for each message).

		Update();
	}

	Editor_ImGui::Get().ShutDown();

	return 0;
}

void ModelViewer::ShowSplashScreen()
{
	if (!mySplashWindow)
	{
		mySplashWindow = new SplashWindow();
		mySplashWindow->Init(myModuleHandle);
	}
}

void ModelViewer::HideSplashScreen() const
{
	mySplashWindow->Close();
	delete mySplashWindow;
	ShowWindow(myMainWindowHandle, SW_SHOW);
	SetForegroundWindow(myMainWindowHandle);
}

void ModelViewer::InitializeGameObjectTypes()
{
	myGameObjectIds.emplace("CameraObject", GameObjectType::CameraObject);
	myGameObjectIds.emplace("ModelObject", GameObjectType::ModelObject);
	myGameObjectIds.emplace("AnimatedObject", GameObjectType::AnimatedObject);
	myGameObjectIds.emplace("DirectionalLightObject", GameObjectType::DirectionalLightObject);
	myGameObjectIds.emplace("PointLightObject", GameObjectType::PointLightObject);
	myGameObjectIds.emplace("SpotLightObject", GameObjectType::SpotLightObject);
}

void ModelViewer::Update()
{
	float deltaTime = CommonUtilities::Timer::GetDeltaTime();
	double totalTime = CommonUtilities::Timer::GetTotalTime();
	myInputHandler->UpdateInput();
	myParticleEmitter->Update(deltaTime);
	UpdateScene();
	CommonUtilities::Timer::Update();
	myCamera->Update(deltaTime);
	Editor_ImGui::Get().Update();

	GraphicsEngine::Get().BeginFrame();
	GraphicsEngine::Get().RenderFrame(deltaTime, totalTime);

	Editor_ImGui::Get().Render();

	GraphicsEngine::Get().EndFrame();

	myInputHandler->LateUpdate();
}

void ModelViewer::ClearRenderTargets()
{
	RHI::ClearRenderTarget(GraphicsEngine::Get().GetGBufferTextures()[0].get());
	RHI::ClearRenderTarget(GraphicsEngine::Get().GetGBufferTextures()[1].get());
	RHI::ClearRenderTarget(GraphicsEngine::Get().GetGBufferTextures()[2].get());
	RHI::ClearRenderTarget(GraphicsEngine::Get().GetGBufferTextures()[3].get());
	RHI::ClearRenderTarget(GraphicsEngine::Get().GetGBufferTextures()[4].get());
	RHI::ClearRenderTarget(GraphicsEngine::Get().GetGBufferTextures()[5].get());
	RHI::ClearRenderTarget(GraphicsEngine::Get().GetIntermediateTexture().get());
	RHI::ClearRenderTarget(GraphicsEngine::Get().GetHalfSizeTexture().get());
	RHI::ClearRenderTarget(GraphicsEngine::Get().GetQuaterSizeTexture().get());
	RHI::ClearRenderTarget(GraphicsEngine::Get().GetSecondQuaterSizeTexture().get());
}

void ModelViewer::LoadScene()
{
	myDefaultMaterial = GraphicsEngine::Get().GetDefaultMaterial();
	myCubeMap = TextureAssetHandler::GetInstance().TryGetAsset(L"../../Assets/Textures/skansen_cubemap.dds");
	myBlueNoiseTexture = TextureAssetHandler::GetInstance().TryGetAsset(L"../../Textures/BlueNoise.dds");
	myParticleEmitter = std::make_shared<ParticleEmitter>();
	Vector3f pos = { 0,0,0 };
	ParticleEmitterTemplate temp;
	temp.EmitterSettings.StartColor = Vector4f(1, 0, 0, 1);
	temp.EmitterSettings.LifeTime = 8.0f;
	temp.EmitterSettings.StartSize = 5.0f;
	temp.EmitterSettings.SpawnRate = 10.0f;
	temp.EmitterSettings.EndColor = Vector4f(0, 0, 1, 0);
	temp.EmitterSettings.GravityModifier = 9.81f * 2.f;
	temp.EmitterSettings.StartVelocity = Vector3f(0, 50.0f, 0);
	temp.Path = L"../../Assets/Textures/ParticleStar.dds";
	myParticleEmitter->Init(temp, pos);
	std::vector<Transform> myInstances;
	myInstances.push_back({ 100,0,100,0,0,0,1,1,1 });
	myInstances.push_back({ 300,0,100,0,0,0,1,1,1 });
	myInstances.push_back({ 500,0,100,0,0,0,1,1,1 });
	myInstances.push_back({ 700,0,100,0,0,0,1,1,1 });
	myInstances.push_back({ 900,0,100,0,0,0,1,1,1 });
	myInstances.push_back({ 0,0,400,0,0,0,1,1,1 });
	myInstances.push_back({ 200,0,400,0,0,0,1,1,1 });
	myInstances.push_back({ 400,0,400,0,0,0,1,1,1 });
	myInstances.push_back({ 600,0,400,0,0,0,1,1,1 });
	myInstances.push_back({ 800,0,400,0,0,0,1,1,1 });
	myInstancedMesh = std::make_shared<Mesh>(Cube, myInstances);
	myInstancedMesh->SetMaterialsToElements(myDefaultMaterial);
}

const std::vector<std::shared_ptr<GameObject>>& ModelViewer::GetGOList() const
{
	return myGOlist;
}

const std::vector<std::weak_ptr<Texture>>& ModelViewer::GetTextureList() const
{
	return myTextures;
}

const std::unordered_map<std::string, GameObjectType>& ModelViewer::GetGOIDs() const
{
	return myGameObjectIds;
}


void ModelViewer::UpdateScene()
{
	GraphicsEngine::Get().AddCommand<GfxCmd_ChangeDepthStencilState>(DepthStencilState::DDS_ReadWrite);

	GraphicsEngine::Get().AddCommand<GfxCmd_SetRenderTarget>(nullptr, nullptr);
	ClearRenderTargets();
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0);
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 1);
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 2);
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 3);
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 4);
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 5);
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 8);
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 9);
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 10);
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 11);
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 12);

	GraphicsEngine::Get().AddCommand<GfxCmd_SetTextureResource>(nullptr, 110);

	GraphicsEngine::Get().AddCommand<GfxCmd_SetRenderTarget>(nullptr, GraphicsEngine::Get().GetShadowMap());

	for (int index = 0; index < myDirectionalLights.size(); index++)
	{
		GraphicsEngine::Get().AddCommand<GfxCmd_SetFrameBuffer>(myDirectionalLights[index]->GetViewProjection(),
			myDirectionalLights[index]->GetLightProjection(), myDirectionalLights[index]->GetTransform().GetPosition(),
			CommonUtilities::Vector2f(RHI::GetDeviceSize().Width, RHI::GetDeviceSize().Height));
	}

	for (size_t i = 0; i < myModels.size(); i++)
	{
		GraphicsEngine::Get().AddCommand<GfxCmd_RenderMeshShadow>(myModels[i]->GetMesh(), myModels[i]->GetTransform().GetMatrix());
	}

	for (size_t i = 0; i < myAnimatedObjects.size(); i++)
	{
		GraphicsEngine::Get().AddCommand<GfxCmd_RenderMeshShadow>(myAnimatedObjects[i]->GetMesh(), myAnimatedObjects[i]->GetTransform().GetMatrix(),
			myAnimatedObjects[i]->GetBoneTransforms(), myAnimatedObjects[i]->GetMesh()->GetSkeleton().bones.size());
	}

	GraphicsEngine::Get().AddCommand<GfxCmd_SetRenderTarget>(nullptr, nullptr);


	GraphicsEngine::Get().AddCommand<GfxCmd_SetTextureResource>(GraphicsEngine::Get().GetShadowMap(), 110);

	GraphicsEngine::Get().AddCommand<GfxCmd_SetRenderTarget>(GraphicsEngine::Get().GetSceneBuffer(), GraphicsEngine::Get().GetDepthStencil());

	for (int index = 0; index < myDirectionalLights.size(); index++)
	{
		GraphicsEngine::Get().AddCommand<GfxCmd_SetLightBuffer>
			(
				myDirectionalLights[index]->GetViewProjection(),
				myDirectionalLights[index]->GetLightProjection(),
				myDirectionalLights[index]->GetColor(),
				myDirectionalLights[index]->GetDirection(),
				myDirectionalLights[index]->GetIntensity()
			);
	}

	for (int index = 0; index < myPointLights.size(); index++)
	{
		GraphicsEngine::Get().AddCommand<GfxCmd_SetPointLightBuffer>(
			myPointLights[index]->GetColor(),
			myPointLights[index]->GetTransform().GetPosition(),
			myPointLights[index]->GetIntensity(),
			myPointLights[index]->GetRange()
		);
	}

	for (int index = 0; index < mySpotLights.size(); index++)
	{
		GraphicsEngine::Get().AddCommand<GfxCmd_SetSpotLightBuffer>
			(
				mySpotLights[index]->GetColor(),
				mySpotLights[index]->GetTransform().GetPosition(),
				mySpotLights[index]->GetTransform().GetRotation(),
				mySpotLights[index]->GetIntensity(),
				mySpotLights[index]->GetRange(),
				mySpotLights[index]->GetConeRadius(),
				mySpotLights[index]->GetPenumbra()
			);
	}

	GraphicsEngine::Get().AddCommand<GfxCmd_SetCubeMapResource>(myCubeMap);

	for (int index = 0; index < myAnimatedObjects.size(); index++)
	{
		myAnimatedObjects[index]->Update();
	}

	GraphicsEngine::Get().AddCommand<GfxCmd_SetFrameBuffer>(myCamera->GetTransform().GetMatrix(), myCamera->GetProjection(), myCamera->GetPosition(),
		CommonUtilities::Vector2f(RHI::GetDeviceSize().Width, RHI::GetDeviceSize().Height));


	//Deferred Rendering
	GraphicsEngine::Get().AddCommand<GfxCmd_SetDeferredRenderTarget>(GraphicsEngine::Get().GetGBuffer(), GraphicsEngine::Get().GetDepthStencil());

	for (int index = 0; index < myModels.size(); index++)
	{
		GraphicsEngine::Get().AddCommand<GfxCmd_RenderDeferredDataPass>(myModels[index]->GetMesh(), myModels[index]->GetTransform().GetMatrix());
	}
	
	for (int index = 0; index < myAnimatedObjects.size(); index++)
	{
		GraphicsEngine::Get().AddCommand<GfxCmd_RenderDeferredDataPass>
			(
				myAnimatedObjects[index]->GetMesh(),
				myAnimatedObjects[index]->GetTransform().GetMatrix(),
				myAnimatedObjects[index]->GetBoneTransforms(),
				myAnimatedObjects[index]->GetMesh()->GetSkeleton().bones.size()
			);
	}
		//Forward goes here...
	GraphicsEngine::Get().AddCommand<GfxCmd_SetRenderTarget>(GraphicsEngine::Get().GetRenderTarget(), GraphicsEngine::Get().GetDepthStencil());
	GraphicsEngine::Get().AddCommand<GfxCmd_RenderMesh>(myInstancedMesh, Matrix4f());
		//Release GBuffer
	GraphicsEngine::Get().AddCommand<GfxCmd_SetRenderTarget>(nullptr, nullptr);

	//Set GBuffer as resource
	GraphicsEngine::Get().AddCommand<GfxCmd_SetDeferredTextureResources>(GraphicsEngine::Get().GetGBufferTextures(), 0);

	GraphicsEngine::Get().AddCommand<GfxCmd_SSAODataPass>(GraphicsEngine::Get().GetEmptyTexture(), myBlueNoiseTexture);

	//Set back buffer as target without depth
	GraphicsEngine::Get().AddCommand<GfxCmd_SetRenderTarget>(GraphicsEngine::Get().GetSceneBuffer(), nullptr);

	//Set ScreenSpaceQuad_VS as vertex shader & EnvironmentPS as pixel shader
	GraphicsEngine::Get().AddCommand<GfxCmd_EnvironmentLightPass>();

	GraphicsEngine::Get().AddCommand<GfxCmd_ChangeBlendState>(BlendState::eAddative);

	for (int index = 0; index < myPointLights.size(); index++)
	{
		GraphicsEngine::Get().AddCommand<GfxCmd_PointLightPass>
			(
				myPointLights[index]->GetColor(),
				myPointLights[index]->GetIntensity(),
				myPointLights[index]->GetTransform().GetPosition(),
				myPointLights[index]->GetRange()
			);
	}

	for (int index = 0; index < mySpotLights.size(); index++)
	{
		GraphicsEngine::Get().AddCommand<GfxCmd_SpotLightPass>
			(
				mySpotLights[index]->GetColor(),
				mySpotLights[index]->GetTransform().GetPosition(),
				mySpotLights[index]->GetTransform().GetRotation(),
				mySpotLights[index]->GetIntensity(),
				mySpotLights[index]->GetRange(),
				mySpotLights[index]->GetConeRadius(),
				mySpotLights[index]->GetPenumbra());
	}

	GraphicsEngine::Get().AddCommand<GfxCmd_ChangeBlendState>(BlendState::eNone);

	GraphicsEngine::Get().AddCommand<GfxCmd_LuminencePass>(GraphicsEngine::Get().GetSceneBuffer(), GraphicsEngine::Get().GetIntermediateTexture());

	GraphicsEngine::Get().AddCommand<GfxCmd_CopyPixelPass>(GraphicsEngine::Get().GetIntermediateTexture(), GraphicsEngine::Get().GetHalfSizeTexture());
	GraphicsEngine::Get().AddCommand<GfxCmd_CopyPixelPass>(GraphicsEngine::Get().GetHalfSizeTexture(), GraphicsEngine::Get().GetQuaterSizeTexture());

	GraphicsEngine::Get().AddCommand<GfxCmd_GaussianBlurPass>(GraphicsEngine::Get().GetQuaterSizeTexture(), GraphicsEngine::Get().GetSecondQuaterSizeTexture());
	GraphicsEngine::Get().AddCommand<GfxCmd_GaussianBlurPass>(GraphicsEngine::Get().GetSecondQuaterSizeTexture(), GraphicsEngine::Get().GetQuaterSizeTexture());

	GraphicsEngine::Get().AddCommand<GfxCmd_UpsamplePass>(GraphicsEngine::Get().GetQuaterSizeTexture(), GraphicsEngine::Get().GetHalfSizeTexture());
	GraphicsEngine::Get().AddCommand<GfxCmd_UpsamplePass>(GraphicsEngine::Get().GetHalfSizeTexture(), GraphicsEngine::Get().GetIntermediateTexture());

	GraphicsEngine::Get().AddCommand<GfxCmd_SetRenderTarget>(GraphicsEngine::Get().GetIntermediateBTexture(), nullptr);

	GraphicsEngine::Get().AddCommand<GfxCmd_BloomPass>(GraphicsEngine::Get().GetIntermediateTexture(), GraphicsEngine::Get().GetSceneBuffer());

	GraphicsEngine::Get().AddCommand<GfxCmd_ToneMapPass>(GraphicsEngine::Get().GetIntermediateBTexture(), GraphicsEngine::Get().GetRenderTarget());

	

	GraphicsEngine::Get().AddCommand<GfxCmd_ChangeDepthStencilState>(DepthStencilState::DDS_ReadOnly);

	GraphicsEngine::Get().AddCommand<GfxCmd_RenderParticles>(myParticleEmitter);
}

const std::shared_ptr<GameObject>& ModelViewer::GetSelectedObject(int anIndex)
{
	return myGOlist[anIndex];
}

void ModelViewer::DisableInputHandler(bool aShouldBeDisabled)
{
	myCamera->DisableCameraMovement(aShouldBeDisabled);
}

void ModelViewer::LoadGameObjects(std::string aPath)
{
	FILE* fp;
	errno_t err = fopen_s(&fp, (aPath).c_str(), "rb");
	if (err == 0)
	{
		char readBuffer[10000] = {};
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		rapidjson::Document document;
		document.ParseStream(is);
		for (unsigned gameObjectIndex = 0; gameObjectIndex < document["GameObjects"].Size(); gameObjectIndex++)
		{
			const rapidjson::Value& GO = document["GameObjects"][gameObjectIndex];
			GameObjectType type = myGameObjectIds.at(GO["Type"].GetString());

			switch (type)
			{
			case GameObjectType::CameraObject:
			{
				CommonUtilities::Vector3f camPos = { GO["Position"]["X"].GetFloat(), GO["Position"]["Y"].GetFloat(), GO["Position"]["Z"].GetFloat() };
				CommonUtilities::Vector3f camRot = { GO["Rotation"]["X"].GetFloat(), GO["Rotation"]["Y"].GetFloat(), GO["Rotation"]["Z"].GetFloat() };
				myCamera = std::make_shared<CameraObject>
					(
						myInputHandler,
						camPos,
						camRot,
						GO["Speed"].GetFloat()
					);
				myCamera->SetType(type);
				myCamera->SetName("Camera");
				myGOlist.push_back(myCamera);
				//myCamera->GetID();
			}
			break;
			case GameObjectType::ModelObject:
			{
				std::shared_ptr<ModelObject> model = std::make_shared<ModelObject>(CommonUtilities::Vector3f(GO["Position"]["X"].GetFloat(),
					GO["Position"]["Y"].GetFloat(), GO["Position"]["Z"].GetFloat()));
				std::shared_ptr<Mesh> mesh;
				std::weak_ptr<Texture> albedoMap;
				std::weak_ptr<Texture> normalMap;
				std::weak_ptr<Texture> materialMap;
				std::weak_ptr<Texture> FXMap;
				std::shared_ptr<Material> material;

				std::string meshPath = GO["Mesh"].GetString();
				std::string albedoPath = GO["Textures"][0]["Albedo"].GetString();
				std::string normalPath = GO["Textures"][1]["Normal"].GetString();
				std::string materialTexPath = GO["Textures"][2]["Material"].GetString();
				std::string FXPath = GO["Textures"][3]["FX"].GetString();
				std::vector<std::string> textureNames;
				textureNames.push_back(albedoPath);
				textureNames.push_back(normalPath);
				textureNames.push_back(materialTexPath);
				textureNames.push_back(FXPath);

				model->SetTextures(textureNames);
				model->SetName(meshPath);
				if (meshPath == "Cube")
				{
					mesh = ModelAssetHandler::GetInstance().TryGetAsset("Cube");
				}
				else if (meshPath == "Plane")
				{
					mesh = ModelAssetHandler::GetInstance().TryGetAsset("Plane");
				}
				else
				{
					mesh = ModelAssetHandler::GetInstance().TryGetAsset("../../Assets/Models/" + meshPath + ".fbx");
				}
				GraphicsEngine::Get().SetWorldBounds(mesh->GetAABB());

				if (albedoPath.size() <= 1)
				{
					mesh->SetMaterialsToElements(myDefaultMaterial);
				}
				else
				{
					for (unsigned index = 0; index < GO["Textures"].Size(); index++)
					{
						switch (index)
						{
						case 0:
							albedoMap = TextureAssetHandler::GetInstance().TryGetAsset(L"../../Assets/Textures/" +
								RHI::string_cast<std::wstring>(albedoPath) + L".dds");
							break;
						case 1:
							normalMap = TextureAssetHandler::GetInstance().TryGetAsset(L"../../Assets/Textures/" +
								RHI::string_cast<std::wstring>(normalPath) + L".dds");
							break;
						case 2:
							materialMap = TextureAssetHandler::GetInstance().TryGetAsset(L"../../Assets/Textures/" +
								RHI::string_cast<std::wstring>(materialTexPath) + L".dds");
							break;
						case 3:
							/*if (FXPath.size() >= 1)
							{*/
							FXMap = TextureAssetHandler::GetInstance().TryGetAsset(L"../../Assets/Textures/" +
								RHI::string_cast<std::wstring>(FXPath) + L".dds");
							//}
							break;
						default:
							break;
						}
					}
					material = std::make_shared<Material>(GraphicsEngine::Get().GetVertexShader(), GraphicsEngine::Get().GetPixelShader());
					material->SetTexture(RHI::string_cast<std::wstring>(meshPath) + L"_Albedo", albedoMap);
					material->SetTexture(RHI::string_cast<std::wstring>(meshPath) + L"_Normal", normalMap);
					material->SetTexture(RHI::string_cast<std::wstring>(meshPath) + L"_Material", materialMap);
					/*if (FXPath.size() >= 1)
					{*/
					material->SetTexture(RHI::string_cast<std::wstring>(meshPath) + L"_FX", FXMap);
					//}

					mesh->SetMaterialsToElements(material);
				}
				model->SetMesh(mesh);
				model->SetType(type);
				//model->GetID();
				myMaterials.push_back(material);
				myModels.push_back(model);
				myGOlist.push_back(model);
			}
			break;
			case GameObjectType::AnimatedObject:
			{
				std::shared_ptr<AnimatedObject> object;
				std::shared_ptr<Mesh> mesh;
				std::shared_ptr<Animation> animation;
				std::weak_ptr<Texture> albedoMap;
				std::weak_ptr<Texture> normalMap;
				std::weak_ptr<Texture> materialMap;
				std::weak_ptr<Texture> FXMap;

				std::shared_ptr<Material> material;
				std::string meshPath = GO["Mesh"].GetString();
				std::string albedoPath = GO["Textures"][0]["Albedo"].GetString();
				std::string normalPath = GO["Textures"][1]["Normal"].GetString();
				std::string materialTexPath = GO["Textures"][2]["Material"].GetString();
				std::string FXPath = GO["Textures"][3]["FX"].GetString();
				std::string animationPath = GO["Animation"].GetString();
				
				std::vector<std::string> textureNames;
				textureNames.push_back(albedoPath);
				textureNames.push_back(normalPath);
				textureNames.push_back(materialTexPath);
				textureNames.push_back(FXPath);
				textureNames.push_back(animationPath);

				

				//TODO: Make more generic..
				animation = AnimationAssetHandler::GetInstance().TryGetAsset("../../Assets/Animations/Locomotion/" + animationPath + ".fbx");
				mesh = ModelAssetHandler::GetInstance().TryGetAsset("../../Assets/Models/" + meshPath + ".fbx");
				GraphicsEngine::Get().SetWorldBounds(mesh->GetAABB());

				if (albedoPath.size() <= 1)
				{
					mesh->SetMaterialsToElements(myDefaultMaterial);
				}
				else
				{
					for (unsigned index = 0; index < GO["Textures"].Size(); index++)
					{
						switch (index)
						{
						case 1:
							albedoMap = TextureAssetHandler::GetInstance().TryGetAsset(L"../../Assets/Textures/" +
								RHI::string_cast<std::wstring>(albedoPath) + L".dds");
							break;
						case 2:
							normalMap = TextureAssetHandler::GetInstance().TryGetAsset(L"../../Assets/Textures/" +
								RHI::string_cast<std::wstring>(normalPath) + L".dds");
							break;
						case 3:
							materialMap = TextureAssetHandler::GetInstance().TryGetAsset(L"../../Assets/Textures/" +
								RHI::string_cast<std::wstring>(materialTexPath) + L".dds");
							break;
						case 4:
							if (FXPath.size() >= 1)
							{
								FXMap = TextureAssetHandler::GetInstance().TryGetAsset(L"../../Assets/Textures/" +
									RHI::string_cast<std::wstring>(FXPath) + L".dds");
							}
							break;
						default:
							break;
						}
					}
					material = std::make_shared<Material>(GraphicsEngine::Get().GetVertexShader(), GraphicsEngine::Get().GetPixelShader());
					material->SetTexture(RHI::string_cast<std::wstring>(meshPath) + L"_Albedo", albedoMap);
					material->SetTexture(RHI::string_cast<std::wstring>(meshPath) + L"_Normal", normalMap);
					material->SetTexture(RHI::string_cast<std::wstring>(meshPath) + L"_Material", materialMap);
					if (FXPath.size() >= 1)
					{
						material->SetTexture(RHI::string_cast<std::wstring>(meshPath) + L"_FX", FXMap);
					}

					mesh->SetMaterialsToElements(material);
				}

				object = std::make_shared<AnimatedObject>
					(
						animation,
						mesh,
						CommonUtilities::Vector3f
						(
							GO["Position"]["X"].GetFloat(),
							GO["Position"]["Y"].GetFloat(),
							GO["Position"]["Z"].GetFloat()
						)
					);
				object->SetTextures(textureNames);
				object->SetName(meshPath);
				object->SetType(type);
				//object->GetID();
				myMaterials.push_back(material);
				myAnimatedObjects.push_back(object);
				myGOlist.push_back(object);
			}
			break;
			case GameObjectType::DirectionalLightObject:
			{
				GraphicsEngine::Get().CalculatWorldCenterAndRadius();
				Vector4f color =
				{
					GO["Color"]["R"].GetFloat(),
					GO["Color"]["G"].GetFloat(),
					GO["Color"]["B"].GetFloat(),
					GO["Color"]["A"].GetFloat()
				};
				Vector3f direction =
				{
					GO["Direction"]["X"].GetFloat(),
					GO["Direction"]["Y"].GetFloat(),
					GO["Direction"]["Z"].GetFloat()
				};
				float intesity = GO["Intensity"].GetFloat();

				std::shared_ptr<LightObject> directional = std::make_shared<LightObject>
					(
						GraphicsEngine::Get().GetWorldCenter(),
						GraphicsEngine::Get().GetWorldRadius(),
						color,
						direction,
						intesity
					);
				//directional->GetID();
				directional->SetType(type);
				directional->SetName("DirectionalLight");
				myGOlist.push_back(directional);
				
				myDirectionalLights.push_back(directional);
			}
			break;
			case GameObjectType::PointLightObject:
			{
				Vector4f color =
				{
					GO["Color"]["R"].GetFloat(),
					GO["Color"]["G"].GetFloat(),
					GO["Color"]["B"].GetFloat(),
					GO["Color"]["A"].GetFloat()
				};
				Vector3f position =
				{
					GO["Position"]["X"].GetFloat(),
					GO["Position"]["Y"].GetFloat(),
					GO["Position"]["Z"].GetFloat()
				};
				float intensity = GO["Intensity"].GetFloat();
				float range = GO["Range"].GetFloat();

				std::shared_ptr<LightObject> point = std::make_shared<LightObject>
					(
						color,
						position,
						intensity,
						range
					);
				point->SetName("PointLight");
				point->SetType(type);
				//point->GetID();
				myPointLights.push_back(point);
				myGOlist.push_back(point);
			}
			break;
			case GameObjectType::SpotLightObject:
			{
				Vector4f color =
				{
					GO["Color"]["R"].GetFloat(),
					GO["Color"]["G"].GetFloat(),
					GO["Color"]["B"].GetFloat(),
					GO["Color"]["A"].GetFloat()
				};
				Vector3f position =
				{
					GO["Position"]["X"].GetFloat(),
					GO["Position"]["Y"].GetFloat(),
					GO["Position"]["Z"].GetFloat()
				};
				Vector3f direction =
				{
					GO["Direction"]["X"].GetFloat(),
					GO["Direction"]["Y"].GetFloat(),
					GO["Direction"]["Z"].GetFloat()
				};
				float intensity = GO["Intensity"].GetFloat();
				float range = GO["Range"].GetFloat();
				float coneRadius = PI / GO["ConeRadius"].GetFloat();
				float penumbra = PI / GO["Penumbra"].GetFloat();

				std::shared_ptr<LightObject> spot = std::make_shared<LightObject>
					(
						color,
						position,
						direction,
						intensity,
						range,
						coneRadius,
						penumbra
					);
				//spot->GetID();
				spot->SetName("SpotLight");
				spot->SetType(type);
				mySpotLights.push_back(spot);
				myGOlist.push_back(spot);
			}
			break;
			default:
				break;
			}
		}
	}
	else
	{
		std::cout << "error json file could not be read!" << std::endl;
	}

	fclose(fp);
}

void ModelViewer::CreateGameObject(std::string aGOName)
{
	GameObjectType type = myGameObjectIds.at(aGOName);
	switch (type)
	{
	case GameObjectType::CameraObject:
		break;
	case GameObjectType::ModelObject:
	{
		std::shared_ptr<ModelObject> model = std::make_shared<ModelObject>(CommonUtilities::Vector3f::Zero);
		std::shared_ptr<Mesh> mesh;
		std::weak_ptr<Texture> albedoMap;
		std::weak_ptr<Texture> normalMap;
		std::weak_ptr<Texture> materialMap;
		std::weak_ptr<Texture> FXMap;
		std::shared_ptr<Material> material;

		mesh = ModelAssetHandler::GetInstance().TryGetAsset("Cube");

		GraphicsEngine::Get().SetWorldBounds(mesh->GetAABB());
		mesh->SetMaterialsToElements(myDefaultMaterial);
		model->SetName("Cube");
		model->SetMesh(mesh);
		myMaterials.push_back(material);
		myModels.push_back(model);
		myGOlist.push_back(model);
	}
	break;
	case GameObjectType::AnimatedObject:
		break;
	case GameObjectType::DirectionalLightObject:
		break;
	case GameObjectType::PointLightObject:
	{
		Vector4f color = { 1,1,1,1 };
		Vector3f position = { 0,0,0 };
		float intensity = 1.0f;
		float range = 100.0f;
		std::shared_ptr<LightObject> point = std::make_shared<LightObject>
			(
				color,
				position,
				intensity,
				range
			);
		myPointLights.push_back(point);
	}
	break;
	case GameObjectType::SpotLightObject:
	{
		Vector4f color = { 1,1,1,1 };
		Vector3f position = { 0,0,0 };
		Vector3f direction = { 0,0,1 };
		float intensity = 1.0f;
		float range = 100.0f;
		float coneRadius = PI / 10.0f;
		float penumbra = PI / 10.0f;

		std::shared_ptr<LightObject> spot = std::make_shared<LightObject>
			(
				color,
				position,
				direction,
				intensity,
				range,
				coneRadius,
				penumbra
			);
		mySpotLights.push_back(spot);
	}
	break;
	break;
	default:
		break;
	}
}
