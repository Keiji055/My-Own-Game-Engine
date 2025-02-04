#pragma once
#include "Windows.h"
#include <memory>
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_dx11.h>
#include <Imgui/imgui_impl_win32.h>
#include <CommonUtilities/MathFunctions.hpp>
#include <rapidjson.h>
#include "../../ThirdParty/rapidjson/filewritestream.h"
#include "../../ThirdParty/rapidjson/filereadstream.h"
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/filewritestream.h"
#include "../rapidjson/ostreamwrapper.h"
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/prettywriter.h"
#include "../rapidjson/stringbuffer.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stdio.h>
#include <vector>
#include "../GameObject.h"
#include "../ModelObject.h"
#include "../LightObject.h"
#include "CommandManager.h"
#include "TransformCommand.h"
#include "SaveHandler.h"
#include <CommonUtilities/InputHandler.h>
#include <filesystem>

class Editor_ImGui
{
public:
	friend class SaveHandler;
	static Editor_ImGui& Get() { static Editor_ImGui myInstance; return myInstance; }
	~Editor_ImGui() = default;
	void Initialize(HWND aWindowHandle, ID3D11Device* aDevice, ID3D11DeviceContext* aContext, std::shared_ptr<CommonUtilities::InputHandler>& anInputHandler);
	void Update();
	void Render();
	void ShutDown();

	void MainEditor();
	void UpdateInspector();
	void ShowInspector();
	void ShowOptions();
	void ShowObjectList();
	void ShowAssetList();
	
	void LoadScene(std::string aFileName);
	void LoadEditorSettings();
	void SaveAsModalPopup();
	void SaveColorModalPopup(const Vector4f& aColor);
	void RemovePreset(std::string aFileName);
	bool LoadColor(std::string aFileName, ImVec4& aColor);
private:
	std::shared_ptr<InputHandler> myInputHandler;
	Editor_ImGui() = default;
	ImVec4 interpolationColor1;
	ImVec4 interpolationColor2;
	ImVec4 savingColor;
	float dragFloatX;
	float dragFloatY;
	float dragFloatZ;
	float sliderRotX;
	float sliderRotY;
	float sliderRotZ;
	float previousSliderRotX;
	float previousSliderRotY;
	float previousSliderRotZ;
	float myLerpedR;
	float myLerpedG;
	float myLerpedB;
	float myLerpedA;
	float myLerpedBlend;
	float previousDragFloatX;
	float previousDragFloatY;
	float previousDragFloatZ;
	bool myShouldSaveBeforeExit;
	bool myShouldShowSaveAsPopup;
	bool myShouldShowSaveColorAsPopup;
	bool myShouldShowLoadColorAsPopup;
	bool myHasTransformChanged;
	Transform myCurrentTransform;
	Transform myPreviousTransform;
	std::string myLastSavedScene;
	std::shared_ptr<GameObject> mySelectedObject;
	std::shared_ptr<LightObject> mySelectedLight;
	std::shared_ptr<GameObject> mySelectedGO;
	std::shared_ptr<TransformCommand> myTransformCommand;
	std::unordered_map<std::shared_ptr<GameObject>, unsigned int> myGameObjectList;

};
