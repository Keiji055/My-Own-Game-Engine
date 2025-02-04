#pragma once
//#include "Editor_ImGui.h"
#include "../Core/Modelviewer.h"

class SaveHandler
{
public:
	static SaveHandler& Get() { static SaveHandler myInstance; return myInstance; }
	void SaveScene(std::string aFileName);
	void SaveEditorSettings(std::string aFileName);

	void SaveColorPreset(const CommonUtilities::Vector4f& aColor, std::string aFileName);

private:
	SaveHandler() = default;
};