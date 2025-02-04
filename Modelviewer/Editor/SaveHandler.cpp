#include "SaveHandler.h"

void SaveHandler::SaveScene(std::string aFileName)
{
	
	const std::string fileName = "../../Assets/SceneData/" + aFileName + ".json";
	std::fstream file;
	file.open(fileName);
	if (!file)
	{
		file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
		file << "\n";
		file.close();

		rapidjson::Document settings;
		rapidjson::Document::AllocatorType& allocator = settings.GetAllocator();
		settings.SetObject();

		rapidjson::Value go(rapidjson::Type::kArrayType);
		for (int i = 0; i < ModelViewer::Get().GetGOList().size(); i++)
		{
#pragma region CommonVariables
			rapidjson::Value obj(rapidjson::Type::kObjectType);
			rapidjson::Value var(rapidjson::Type::kObjectType);
			rapidjson::Value color(rapidjson::Type::kObjectType);
			rapidjson::Value direction(rapidjson::Type::kObjectType);
			rapidjson::Value texture(rapidjson::Type::kArrayType);
			rapidjson::Value val(rapidjson::Type::kObjectType);
			GameObjectType type = ModelViewer::Get().GetGOList()[i]->GetType();
			std::string typeStr;

			for (const auto& ot : ModelViewer::Get().GetGOIDs())
			{
				if (ot.second == type)
				{
					typeStr = ot.first;
					break;
				}
			}
			val.SetString(typeStr.c_str(), static_cast<rapidjson::SizeType>(typeStr.length()), allocator);
			obj.AddMember("Type", val, allocator);

			for (size_t pos = 0; pos < 3; pos++)
			{
				if (pos == 0)
				{
					val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetTransform().GetPosition().x);
					var.AddMember("X", val, allocator);

				}
				else if (pos == 1)
				{
					val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetTransform().GetPosition().y);
					var.AddMember("Y", val, allocator);

				}
				else if (pos == 2)
				{
					val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetTransform().GetPosition().z);
					var.AddMember("Z", val, allocator);

				}
			}
			obj.AddMember("Position", var, allocator);
#pragma endregion

			switch (type)
			{
			case GameObjectType::CameraObject:
#pragma region Camera



				for (size_t rot = 0; rot < 3; rot++)
				{
					if (rot == 0)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetTransform().GetRotation().x);
						direction.AddMember("X", val, allocator);

					}
					else if (rot == 1)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetTransform().GetRotation().y);
						direction.AddMember("Y", val, allocator);

					}
					else if (rot == 2)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetTransform().GetRotation().z);
						direction.AddMember("Z", val, allocator);
					}
				}
				obj.AddMember("Rotation", direction, allocator);
				obj.AddMember("Speed", ModelViewer::Get().GetGOList()[i]->GetSpeed(), allocator);
#pragma endregion
				break;
			case GameObjectType::ModelObject: 
			{
				std::string meshName = ModelViewer::Get().GetGOList()[i]->GetName().c_str();
				val.SetString(meshName.c_str(), static_cast<rapidjson::SizeType>(meshName.length()), allocator);
				obj.AddMember("Mesh", val, allocator);

				for (int tex = 0; tex < ModelViewer::Get().GetGOList()[i]->GetTextures().size(); tex++)
				{
					if (tex == 0)
					{
						rapidjson::Value albedo;
						albedo.SetObject();
						std::string name = ModelViewer::Get().GetGOList()[i]->GetTextures()[tex];
						val.SetString(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), allocator);
						albedo.AddMember("Albedo", val, allocator);
						texture.PushBack(albedo, allocator);
					}
					else if (tex == 1)
					{
						rapidjson::Value normal;
						normal.SetObject();
						std::string name = ModelViewer::Get().GetGOList()[i]->GetTextures()[tex];
						val.SetString(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), allocator);
						normal.AddMember("Normal", val, allocator);
						texture.PushBack(normal, allocator);
					}
					else if (tex == 2)
					{
						rapidjson::Value material;
						material.SetObject();
						std::string name = ModelViewer::Get().GetGOList()[i]->GetTextures()[tex];
						val.SetString(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), allocator);
						material.AddMember("Material", val, allocator);
						texture.PushBack(material, allocator);
					}
					else if (tex == 3)
					{
						rapidjson::Value fx;
						fx.SetObject();
						std::string name = ModelViewer::Get().GetGOList()[i]->GetTextures()[tex];
						val.SetString(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), allocator);
						fx.AddMember("FX", val, allocator);
						texture.PushBack(fx, allocator);
					}
				}
				obj.AddMember("Textures", texture, allocator);

			}
				break;
			case GameObjectType::AnimatedObject:
			{
				std::string meshName = ModelViewer::Get().GetGOList()[i]->GetName().c_str();
				val.SetString(meshName.c_str(), static_cast<rapidjson::SizeType>(meshName.length()), allocator);
				obj.AddMember("Mesh", val, allocator);

				for (int tex = 0; tex < ModelViewer::Get().GetGOList()[i]->GetTextures().size(); tex++)
				{
					if (tex == 0)
					{
						rapidjson::Value albedo;
						albedo.SetObject();
						std::string name = ModelViewer::Get().GetGOList()[i]->GetTextures()[tex];
						val.SetString(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), allocator);
						albedo.AddMember("Albedo", val, allocator);
						texture.PushBack(albedo, allocator);
					}
					else if (tex == 1)
					{
						rapidjson::Value normal;
						normal.SetObject();
						std::string name = ModelViewer::Get().GetGOList()[i]->GetTextures()[tex];
						val.SetString(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), allocator);
						normal.AddMember("Normal", val, allocator);
						texture.PushBack(normal, allocator);
					}
					else if (tex == 2)
					{
						rapidjson::Value material;
						material.SetObject();
						std::string name = ModelViewer::Get().GetGOList()[i]->GetTextures()[tex];
						val.SetString(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), allocator);
						material.AddMember("Material", val, allocator);
						texture.PushBack(material, allocator);
					}
					else if (tex == 3)
					{
						rapidjson::Value fx;
						fx.SetObject();
						std::string name = ModelViewer::Get().GetGOList()[i]->GetTextures()[tex];
						val.SetString(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), allocator);
						fx.AddMember("FX", val, allocator);
						texture.PushBack(fx, allocator);
					}
				}
				obj.AddMember("Textures", texture, allocator);
				std::string anim = ModelViewer::Get().GetGOList()[i]->GetTextures()[4].c_str();
				val.SetString(anim.c_str(), static_cast<rapidjson::SizeType>(anim.length()), allocator);
				obj.AddMember("Animation", val, allocator);
			}
				break;
			case GameObjectType::DirectionalLightObject:
#pragma region Directional
				for (size_t col = 0; col < 4; col++)
				{
					if (col == 0)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetColor().x);
						color.AddMember("R", val, allocator);

					}
					else if (col == 1)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetColor().y);
						color.AddMember("G", val, allocator);

					}
					else if (col == 2)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetColor().z);
						color.AddMember("B", val, allocator);
					}
					else if (col == 3)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetColor().w);
						color.AddMember("A", val, allocator);
					}
				}
				obj.AddMember("Color", color, allocator);
				for (size_t dir = 0; dir < 3; dir++)
				{
					if (dir == 0)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetDirection().x);
						direction.AddMember("X", val, allocator);

					}
					else if (dir == 1)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetDirection().y);
						direction.AddMember("Y", val, allocator);

					}
					else if (dir == 2)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetDirection().z);
						direction.AddMember("Z", val, allocator);
					}
				}
				obj.AddMember("Direction", direction, allocator);
				val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetIntensity());
				obj.AddMember("Intensity", val, allocator); 
#pragma endregion
				break;
			case GameObjectType::PointLightObject:
#pragma region Point
				for (size_t col = 0; col < 4; col++)
				{
					if (col == 0)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetColor().x);
						color.AddMember("R", val, allocator);

					}
					else if (col == 1)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetColor().y);
						color.AddMember("G", val, allocator);

					}
					else if (col == 2)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetColor().z);
						color.AddMember("B", val, allocator);
					}
					else if (col == 3)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetColor().w);
						color.AddMember("A", val, allocator);
					}
				}
				obj.AddMember("Color", color, allocator);
				val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetIntensity());
				obj.AddMember("Intensity", val, allocator); 
				val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetRange());
				obj.AddMember("Range", val, allocator);
#pragma endregion
				break;
			case GameObjectType::SpotLightObject:
				for (size_t col = 0; col < 4; col++)
				{
					if (col == 0)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetColor().x);
						color.AddMember("R", val, allocator);

					}
					else if (col == 1)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetColor().y);
						color.AddMember("G", val, allocator);

					}
					else if (col == 2)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetColor().z);
						color.AddMember("B", val, allocator);
					}
					else if (col == 3)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetColor().w);
						color.AddMember("A", val, allocator);
					}
				}
				obj.AddMember("Color", color, allocator);

				for (size_t dir = 0; dir < 3; dir++)
				{
					if (dir == 0)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetTransform().GetRotation().x);
						direction.AddMember("X", val, allocator);

					}
					else if (dir == 1)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetTransform().GetRotation().y);
						direction.AddMember("Y", val, allocator);

					}
					else if (dir == 2)
					{
						val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetTransform().GetRotation().z);
						direction.AddMember("Z", val, allocator);
					}
				}
				obj.AddMember("Direction", direction, allocator);
				val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetIntensity());
				obj.AddMember("Intensity", val, allocator);
				val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetRange());
				obj.AddMember("Range", val, allocator);
				val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetPenumbra());
				obj.AddMember("Penumbra", val, allocator);
				val.SetFloat(ModelViewer::Get().GetGOList()[i]->GetConeRadius());
				obj.AddMember("ConeRadius", val, allocator);
				break;
			default:
				break;
			}

			go.PushBack(obj, allocator);
		}
		settings.AddMember("GameObjects", go, allocator);

		std::ofstream engine_ofs(fileName);
		rapidjson::OStreamWrapper osw(engine_ofs);
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
		settings.Accept(writer);
	}
	else
	{
		file.close();
	}

	SaveEditorSettings(aFileName);
}

void SaveHandler::SaveEditorSettings(std::string aFileName)
{
	std::string currentScene = aFileName;
	const std::string fileName = "../../Assets/SceneData/Settings.json";
	std::fstream file;
	file.open(fileName);
	if (file)
	{
		file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
		file << "\n";
		file.close();

		rapidjson::Document settings;
		rapidjson::Document::AllocatorType& allocator = settings.GetAllocator();
		settings.SetObject();

		rapidjson::Value lastSavedScene;
		lastSavedScene.SetObject();
		lastSavedScene.AddMember("Name", rapidjson::StringRef(currentScene.c_str()), allocator);
		settings.AddMember("LastSavedScene", lastSavedScene, allocator);
		
		rapidjson::Value color1Value;
		color1Value.SetObject();
		color1Value.AddMember("R", Editor_ImGui::Get().interpolationColor1.x, allocator);
		color1Value.AddMember("G", Editor_ImGui::Get().interpolationColor1.y, allocator);
		color1Value.AddMember("B", Editor_ImGui::Get().interpolationColor1.z, allocator);
		color1Value.AddMember("A", Editor_ImGui::Get().interpolationColor1.w, allocator);
		settings.AddMember("Color1", color1Value, allocator);
		
		rapidjson::Value color2Value;
		color2Value.SetObject();
		color2Value.AddMember("R", Editor_ImGui::Get().interpolationColor2.x, allocator);
		color2Value.AddMember("G", Editor_ImGui::Get().interpolationColor2.y, allocator);
		color2Value.AddMember("B", Editor_ImGui::Get().interpolationColor2.z, allocator);
		color2Value.AddMember("A", Editor_ImGui::Get().interpolationColor2.w, allocator);
		settings.AddMember("Color2", color2Value, allocator);

		rapidjson::Value colorBlendValue;
		colorBlendValue.SetObject();
		colorBlendValue.AddMember("R", Editor_ImGui::Get().myLerpedR, allocator);
		colorBlendValue.AddMember("G", Editor_ImGui::Get().myLerpedG, allocator);
		colorBlendValue.AddMember("B", Editor_ImGui::Get().myLerpedB, allocator);
		colorBlendValue.AddMember("A", Editor_ImGui::Get().myLerpedA, allocator);
		settings.AddMember("ColorBlend", colorBlendValue, allocator);

		rapidjson::Value floatBlendValue;
		floatBlendValue.SetObject();
		floatBlendValue.AddMember("Value", Editor_ImGui::Get().myLerpedBlend, allocator);
		settings.AddMember("Blend", floatBlendValue, allocator);

		std::ofstream engine_ofs(fileName);
		rapidjson::OStreamWrapper osw(engine_ofs);
		rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
		settings.Accept(writer);
	}
	else
	{
		file.close();
	}

	std::ifstream engine_ifs(fileName);
	std::string contents((std::istreambuf_iterator<char>(engine_ifs)), std::istreambuf_iterator<char>());
	engine_ifs.close();
	rapidjson::Document settings;
	settings.Parse(contents.c_str());

	settings["LastSavedScene"]["Name"].SetString(rapidjson::StringRef(currentScene.c_str()));

	std::ofstream engine_ofs(fileName);
	rapidjson::OStreamWrapper osw(engine_ofs);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
	settings.Accept(writer);

}

void SaveHandler::SaveColorPreset(const CommonUtilities::Vector4f& aColor, std::string aFileName)
{
	const std::string fileName = "../../Assets/SceneData/" + aFileName + ".json";
	std::fstream file;
	file.open(fileName);
	if (!file)
	{
		file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
		file << "\n";
		file.close();

		rapidjson::Document settings;
		rapidjson::Document::AllocatorType& allocator = settings.GetAllocator();
		settings.SetObject();

		rapidjson::Value color1Value;
		color1Value.SetObject();
		color1Value.AddMember("R", aColor.x, allocator);
		color1Value.AddMember("G", aColor.y, allocator);
		color1Value.AddMember("B", aColor.z, allocator);
		color1Value.AddMember("A", aColor.w, allocator);
		settings.AddMember("Color", color1Value, allocator);

		std::ofstream engine_ofs(fileName);
		rapidjson::OStreamWrapper osw(engine_ofs);
		rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
		settings.Accept(writer);
	}
	else
	{
		file.close();
	}

	std::ifstream engine_ifs(fileName);
	std::string contents((std::istreambuf_iterator<char>(engine_ifs)), std::istreambuf_iterator<char>());
	engine_ifs.close();
	rapidjson::Document settings;
	settings.Parse(contents.c_str());

	settings["Color"]["R"].SetFloat(aColor.x);
	settings["Color"]["G"].SetFloat(aColor.y);
	settings["Color"]["B"].SetFloat(aColor.z);

	std::ofstream engine_ofs(fileName);
	rapidjson::OStreamWrapper osw(engine_ofs);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
	settings.Accept(writer);
}
