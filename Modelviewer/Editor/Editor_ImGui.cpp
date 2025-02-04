#include "Editor_ImGui.h"
#include "../Core/Modelviewer.h"
#define PI 3.14159265359f

namespace fs = std::filesystem;


void Editor_ImGui::Initialize(HWND aWindowHandle, ID3D11Device* aDevice, ID3D11DeviceContext* aContext, std::shared_ptr<CommonUtilities::InputHandler>& anInputHandler)
{
	myShouldSaveBeforeExit = false;
	myHasTransformChanged = false;
	myInputHandler = anInputHandler;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	myShouldShowSaveAsPopup = false;
	myShouldShowSaveColorAsPopup = false;
	myShouldShowLoadColorAsPopup = false;
	ImGui_ImplWin32_Init(aWindowHandle);
	ImGui_ImplDX11_Init(aDevice, aContext);
	LoadEditorSettings();
	LoadScene(myLastSavedScene);
}

void Editor_ImGui::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

	MainEditor();
	if (myInputHandler->IsKeyPressed(VK_CONTROL) && myInputHandler->IsKeyDown('Z'))
	{
		CommandManager::Undo();
	}
	if (myInputHandler->IsKeyPressed(VK_CONTROL) && myInputHandler->IsKeyPressed(VK_SHIFT) && myInputHandler->IsKeyDown('Z'))
	{
		CommandManager::Redo();
	}
	if (myInputHandler->IsKeyPressed(VK_CONTROL) && myInputHandler->IsKeyDown('S'))
	{
		SaveHandler::Get().SaveScene(myLastSavedScene);
	}
	ShowOptions();
	ShowObjectList();
	ShowInspector();
	ShowAssetList();
}

void Editor_ImGui::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Editor_ImGui::ShutDown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Editor_ImGui::MainEditor()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Create"))
			{

			}
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
			}
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				SaveHandler::Get().SaveScene(myLastSavedScene);
			}
			if (ImGui::MenuItem("Save as.."))
			{
				myShouldShowSaveAsPopup = true;
			}
			if (ImGui::MenuItem("Exit"))
			{
				PostQuitMessage(0);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	//ImGui::End();
	if (myShouldShowSaveAsPopup)
	{
		ImGui::OpenPopup("SaveAs");

	}
	SaveAsModalPopup();
}

void Editor_ImGui::UpdateInspector()
{

}

void Editor_ImGui::ShowInspector()
{
	if (ImGui::Begin("Inspector"))
	{
		if (mySelectedGO != nullptr)
		{
			//myCurrentTransform = mySelectedGO->GetTransform();
			ImGui::Text("Selected Object:");
			ImGui::SameLine();
			ImGui::Text(mySelectedGO->GetName().c_str());
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
				{
					if (!myHasTransformChanged)
					{
						myPreviousTransform = mySelectedGO->GetTransform();
						myHasTransformChanged = true;
					}
				}
				if (ImGui::TreeNodeEx("Position", ImGuiTreeNodeFlags_DefaultOpen))
				{
					static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
					dragFloatX = mySelectedGO->GetTransform().GetPosition().x;
					dragFloatY = mySelectedGO->GetTransform().GetPosition().y;
					dragFloatZ = mySelectedGO->GetTransform().GetPosition().z;

					ImGui::DragFloat("X", &dragFloatX, 0.05f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
					ImGui::DragFloat("Y", &dragFloatY, 0.05f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
					ImGui::DragFloat("Z", &dragFloatZ, 0.05f, -FLT_MAX, +FLT_MAX, "%.3f", flags);

					if (dragFloatX != previousDragFloatX)
					{
						mySelectedGO->GetTransform().SetPosition({ dragFloatX, previousDragFloatY, previousDragFloatZ });
						previousDragFloatX = dragFloatX;
					}
					if (dragFloatY != previousDragFloatY)
					{
						mySelectedGO->GetTransform().SetPosition({ previousDragFloatX, dragFloatY, previousDragFloatZ });
						previousDragFloatY = dragFloatY;
					}
					if (dragFloatZ != previousDragFloatZ)
					{
						mySelectedGO->GetTransform().SetPosition({ previousDragFloatX, previousDragFloatY, dragFloatZ });
						previousDragFloatZ = dragFloatZ;
					}

					ImGui::TreePop();
				}
				if (ImGui::TreeNodeEx("Rotation", ImGuiTreeNodeFlags_DefaultOpen))
				{
					static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
					sliderRotX = mySelectedGO->GetTransform().GetRotation().x;
					sliderRotY = mySelectedGO->GetTransform().GetRotation().y;
					sliderRotZ = mySelectedGO->GetTransform().GetRotation().z;

					ImGui::DragFloat("X", &sliderRotX, 0.05f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
					ImGui::DragFloat("Y", &sliderRotY, 0.05f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
					ImGui::DragFloat("Z", &sliderRotZ, 0.05f, -FLT_MAX, +FLT_MAX, "%.3f", flags);

					if (sliderRotX != previousSliderRotX)
					{
						int mod = static_cast<int>(sliderRotX);
						mySelectedGO->GetTransform().SetRotation({ static_cast<float>(mod % 360), previousSliderRotY, previousSliderRotZ });
						previousSliderRotX = sliderRotX;
					}
					if (sliderRotY != previousSliderRotY)
					{
						int mod = static_cast<int>(sliderRotY);
						mySelectedGO->GetTransform().SetRotation({ previousSliderRotX, static_cast<float>(mod % 360), previousSliderRotZ });
						previousSliderRotY = sliderRotY;
					}
					if (sliderRotZ != previousSliderRotZ)
					{
						int mod = static_cast<int>(sliderRotZ);
						mySelectedGO->GetTransform().SetRotation({ previousSliderRotX, previousSliderRotY, static_cast<float>(mod % 360) });
						previousSliderRotZ = sliderRotZ;
					}

					ImGui::TreePop();
				}
				if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
				{
					myCurrentTransform = mySelectedGO->GetTransform();
					if (myHasTransformChanged)
					{
						std::shared_ptr<TransformCommand> cmd = std::make_shared<TransformCommand>
							(
								mySelectedGO,
								myPreviousTransform,
								myCurrentTransform
							);
						CommandManager::DoCommand(cmd);
					}
					myHasTransformChanged = false;
				}
			}
			if (mySelectedGO->GetType() == GameObjectType::DirectionalLightObject)
			{
				if (ImGui::CollapsingHeader("Light Options", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::SetNextItemOpen(true, ImGuiCond_Once);
					static ImGuiSliderFlags flags = ImGuiSliderFlags_None | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview;
					static float lerpvalue = myLerpedBlend;
					static float intensity = mySelectedGO->GetIntensity();
					if (ImGui::TreeNodeEx("Color", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::ColorEdit4("MyColor##1", (float*)&savingColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | flags);
						ImGui::SameLine();
						if (ImGui::Button("save", ImVec2(100, 0)))
						{
							myShouldShowSaveColorAsPopup = true;
						}
						ImGui::SameLine();
						ImGui::Text("Color saver");
						//ImGui::SameLine();
						ImGui::ColorButton("MyColor##2c", *(ImVec4*)&interpolationColor1, flags, ImVec2(20, 20));
						ImGui::SameLine();

						static char buf1[32] = ""; ImGui::InputText("Color1", buf1, 32);

						if (ImGui::Button("LoadColor1", ImVec2(100, 0)))
						{
							std::string color = std::string(buf1) + ".json";
							LoadColor(color, interpolationColor1);
						}


						ImGui::ColorButton("MyColor##3c", *(ImVec4*)&interpolationColor2, flags, ImVec2(20, 20));
						ImGui::SameLine();

						static char buf2[32] = ""; ImGui::InputText("Color2", buf2, 32);

						if (ImGui::Button("LoadColor2", ImVec2(100, 0)))
						{
							std::string color = std::string(buf2) + ".json";
							LoadColor(color, interpolationColor2);
						}

						ImGui::AlignTextToFramePadding();

						ImGui::SliderFloat("Blend", &lerpvalue, 0.0f, 1.0f, "%.3f", flags);
						myLerpedR = CommonUtilities::Mathf::Lerp(interpolationColor1.x, interpolationColor2.x, lerpvalue);
						myLerpedG = CommonUtilities::Mathf::Lerp(interpolationColor1.y, interpolationColor2.y, lerpvalue);
						myLerpedB = CommonUtilities::Mathf::Lerp(interpolationColor1.z, interpolationColor2.z, lerpvalue);
						myLerpedA = CommonUtilities::Mathf::Lerp(interpolationColor1.w, interpolationColor2.w, lerpvalue);
						myLerpedBlend = lerpvalue;
						mySelectedGO->SetColor({ myLerpedR, myLerpedG, myLerpedB, myLerpedA });
						ImGui::ColorButton("MyColor##4c", *(ImVec4*)&mySelectedGO->GetColor(), flags, ImVec2(50, 50));

						static char buf3[32] = ""; ImGui::InputText("Color To Remove", buf3, 32);

						if (ImGui::Button("Remove Preset", ImVec2(100, 0)))
						{
							std::string color = std::string(buf3) + ".json";
							RemovePreset(color);
						}

						ImGui::TreePop();
					}


				}
			}
			if (mySelectedGO->GetType() == GameObjectType::PointLightObject || mySelectedGO->GetType() == GameObjectType::SpotLightObject)
			{
				if (ImGui::CollapsingHeader("Light Options", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::SetNextItemOpen(true, ImGuiCond_Once);
					static ImGuiSliderFlags flags = ImGuiSliderFlags_None | ImGuiColorEditFlags_NoSidePreview;
					static float lerpvalue = 0.5f;
					static float intensity = mySelectedGO->GetIntensity();
					static float range = mySelectedGO->GetRange();
					ImGui::DragFloat("Intensity", &intensity, 0.005f, 0, 10, "%.3f", flags);
					ImGui::DragFloat("Range", &range, 0.05f, 0, 1000, "%.3f", flags);
					mySelectedGO->SetIntensity(intensity);
					mySelectedGO->SetRange(range);
					if (mySelectedGO->GetType() == GameObjectType::SpotLightObject)
					{
						static float cone = PI / mySelectedGO->GetConeRadius();
						static float penumbra = PI / mySelectedGO->GetPenumbra();
						ImGui::DragFloat("Cone Radius", &cone, 0.05f, 0, 100, "%.3f", flags);
						ImGui::DragFloat("Penumbra", &penumbra, 0.005f, 0, 1, "%.3f", flags);
						mySelectedGO->SetConeRadius(cone);
						mySelectedGO->SetPenumbra(penumbra);
					}

					if (ImGui::TreeNodeEx("Color", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::ColorPicker3("##MyColor##5", (float*)&mySelectedGO->GetColor(), ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | flags);
						ImGui::TreePop();
					}
				}
			}
			if (mySelectedGO->GetType() == GameObjectType::ModelObject || mySelectedGO->GetType() == GameObjectType::AnimatedObject)
			{
				if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (ImGui::TreeNodeEx("Applied Textures", ImGuiTreeNodeFlags_DefaultOpen))
					{
						int num = mySelectedGO->GetTextures().size();
						static int selected = -1;
						for (int n = 0; n < num; n++)
						{
							char buf[32];
							sprintf_s(buf, mySelectedGO->GetTextures()[n].c_str(), n);
							if (ImGui::Selectable(buf, selected == n))
							{
								//mySelectedGO = nullptr;
								//mySelectedGO = ModelViewer::Get().GetGOList()[n];
								break;
							}
						}
						ImGui::TreePop();
					}
				}
			}
		}
	}
	ImGui::End();
	if (myShouldShowSaveColorAsPopup)
	{
		ImGui::OpenPopup("SaveColor");
	}
	SaveColorModalPopup({ savingColor.x, savingColor.y, savingColor.z, savingColor.w });
}

void Editor_ImGui::ShowOptions()
{
	if (ImGui::Begin("Options"))
	{

	}
	ImGui::End();
}

void Editor_ImGui::ShowObjectList()
{
	std::string selected_GO;
	const char* names[] = { "Camera", "ModelObject", "AnimatedObject", "DirectionalLight", "PointLightObject", "SpotLight" };

	if (ImGui::Begin("Hierarchy"), names)
	{
		static std::vector<bool> selection;
		if (ImGui::Button("+"))
			ImGui::OpenPopup("my_select_popup");

		if (ImGui::BeginPopup("my_select_popup"))
		{
			ImGui::SeparatorText("New");
			for (int i = 0; i < IM_ARRAYSIZE(names); i++)
			{
				if (ImGui::Selectable(names[i]))
				{
					selected_GO = names[i];
					ModelViewer::Get().CreateGameObject(selected_GO);
				}
			}
			ImGui::EndPopup();
		}
		//ImGui::NewLine();
		if (ImGui::CollapsingHeader(myLastSavedScene.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			int num = ModelViewer::Get().GetGOList().size();

			for (size_t i = 0; i < num; i++)
			{
				selection.push_back(false);
			}
			static int selected = -1;
			for (int n = 0; n < num; n++)
			{
				char buf[32];
				sprintf_s(buf, ModelViewer::Get().GetGOList()[n]->GetName().c_str(), n);
				if (ImGui::Selectable(buf, selected == n))
				{
					mySelectedGO = nullptr;
					mySelectedGO = ModelViewer::Get().GetGOList()[n];
					break;
				}
			}

		}
		ImGui::End();
	}
}

void Editor_ImGui::ShowAssetList()
{
	if (ImGui::Begin("Assets"))
	{
		if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen))
		{
			fs::path p = fs::current_path();
			fs::path rp = p.parent_path();
			fs::path texPath = rp.parent_path();
			texPath += "/Assets/Textures/";
			static int selected = -1;
			static int i = 0;
			for (auto const& tex : fs::directory_iterator{ texPath })
			{
				i++;
				char buf[32];
				sprintf_s(buf, tex.path().stem().string().c_str(), i);
				if (ImGui::Selectable(buf, selected == i))
				{
					//std::shared_ptr<ModelObject> obj = mySelectedGO;
				}
			}
		}
		ImGui::End();
	}
}

void Editor_ImGui::LoadScene(std::string aFileName)
{
	ModelViewer::Get().LoadGameObjects("../../Assets/SceneData/" + aFileName + ".json");
}

void Editor_ImGui::LoadEditorSettings()
{
	FILE* fp;
	std::string path = "../../Assets/SceneData/Settings.json";
	errno_t err = fopen_s(&fp, (path).c_str(), "rb");
	if (err == 0)
	{
		char readBuffer[10000] = {};
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		rapidjson::Document document;
		document.ParseStream(is);

		myLastSavedScene = document["LastSavedScene"]["Name"].GetString();

		interpolationColor1.x = document["Color1"]["R"].GetFloat();
		interpolationColor1.y = document["Color1"]["G"].GetFloat();
		interpolationColor1.z = document["Color1"]["B"].GetFloat();
		interpolationColor1.w = document["Color1"]["A"].GetFloat();

		interpolationColor2.x = document["Color2"]["R"].GetFloat();
		interpolationColor2.y = document["Color2"]["G"].GetFloat();
		interpolationColor2.z = document["Color2"]["B"].GetFloat();
		interpolationColor2.w = document["Color2"]["A"].GetFloat();

		myLerpedR = document["ColorBlend"]["R"].GetFloat();
		myLerpedG = document["ColorBlend"]["G"].GetFloat();
		myLerpedB = document["ColorBlend"]["B"].GetFloat();
		myLerpedA = document["ColorBlend"]["A"].GetFloat();

		myLerpedBlend = document["Blend"]["Value"].GetFloat();
	}
	else
	{
		std::cout << "error json file could not be read!" << std::endl;
	}

	fclose(fp);
}

void Editor_ImGui::SaveAsModalPopup()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("SaveAs", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ModelViewer::Get().DisableInputHandler(true);
		ImGui::Text("Choose a name for the current scene");
		ImGui::Separator();
		static char buf1[32] = ""; ImGui::InputText(".json", buf1, 32);
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			SaveHandler::Get().SaveScene(buf1);
			myShouldShowSaveAsPopup = false;
			ModelViewer::Get().DisableInputHandler(false);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			myShouldShowSaveAsPopup = false;
			ModelViewer::Get().DisableInputHandler(false);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void Editor_ImGui::SaveColorModalPopup(const Vector4f& aColor)
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("SaveColor", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ModelViewer::Get().DisableInputHandler(true);
		ImGui::Text("Choose a name for the current scene");
		ImGui::Separator();
		static char buf1[32] = ""; ImGui::InputText(".json", buf1, 32);
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			SaveHandler::Get().SaveColorPreset(aColor, buf1);
			myShouldShowSaveColorAsPopup = false;
			ModelViewer::Get().DisableInputHandler(false);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			myShouldShowSaveColorAsPopup = false;
			ModelViewer::Get().DisableInputHandler(false);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void Editor_ImGui::RemovePreset(std::string aFileName)
{
	FILE* fp;
	std::string path = "../../Assets/SceneData/" + aFileName;
	errno_t err = fopen_s(&fp, (path).c_str(), "rb");
	if (err == 0)
	{
		fclose(fp);
		std::remove(path.c_str());
	}
}

bool Editor_ImGui::LoadColor(std::string aFileName, ImVec4& aColor)
{
	FILE* fp;
	std::string path = "../../Assets/SceneData/" + aFileName;
	errno_t err = fopen_s(&fp, (path).c_str(), "rb");
	ImVec4 color = { 0,0,0,0 };
	if (err == 0)
	{
		char readBuffer[10000] = {};
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		rapidjson::Document document;
		document.ParseStream(is);

		color.x = document["Color"]["R"].GetFloat();
		color.y = document["Color"]["G"].GetFloat();
		color.z = document["Color"]["B"].GetFloat();
		color.w = document["Color"]["A"].GetFloat();
		aColor = color;
		return true;
	}
	else
	{
		MessageBox(NULL, L"That color is not a saved preset!", L"Color Preset could not be loaded!", MB_ICONERROR | MB_OK);
		std::cout << "error json file could not be read!" << std::endl;
		return false;
	}

}
