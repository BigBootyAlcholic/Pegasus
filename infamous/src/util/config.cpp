#include "stdafx.hpp"
#include "config.hpp"
#include "va.hpp"
#include <shlobj.h>
#include <shlwapi.h>
#include <direct.h>
#include "json.hpp"
#include <filesystem>

namespace Utils {
	void Config::Initialize() {
		char LocalTmp[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, m_DocumentsPath);
		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, m_AppdataPath);
		SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, LocalTmp);

		_mkdir(VA::VA("%s\\Pegasus", m_DocumentsPath));
		_mkdir(VA::VA("%s\\Pegasus\\Grand Theft Auto V", m_DocumentsPath));
		_mkdir(VA::VA("%s\\Pegasus\\Grand Theft Auto V\\Outfits", m_DocumentsPath));
		_mkdir(VA::VA("%s\\Pegasus\\Grand Theft Auto V\\Themes", m_DocumentsPath));
		_mkdir(VA::VA("%s\\Pegasus\\Grand Theft Auto V\\Translations", m_DocumentsPath));
		_mkdir(VA::VA("%s\\Pegasus\\Grand Theft Auto V\\Logs", m_DocumentsPath));
		_mkdir(VA::VA("%s\\Pegasus\\Grand Theft Auto V\\Profiles", m_DocumentsPath));
		_mkdir(VA::VA("%s\\Ozark\\Grand Theft Auto V\\ScriptHook", m_DocumentsPath));

		snprintf(m_GTA, MAX_PATH, "%s\\Pegasus\\Grand Theft Auto V", m_DocumentsPath);

		snprintf(m_TmpPath, MAX_PATH, "%s\\", LocalTmp);
		snprintf(m_EntityLimitsPath, MAX_PATH, "%s\\Entity Limits.json", m_GTA);
		snprintf(m_ConfigPath, MAX_PATH, "%s\\Config.json", m_GTA);
		snprintf(m_LogPath, MAX_PATH, "%s\\Log.txt", m_GTA);
		snprintf(m_ThemePath, MAX_PATH, "%s\\Themes\\", m_GTA);
		snprintf(m_PlayerLogPath, MAX_PATH, "%s\\Logs\\", m_GTA);
		snprintf(m_TranslationPath, MAX_PATH, "%s\\Translations\\", m_GTA);
		snprintf(m_ScriptHookPath, MAX_PATH, "%s\\ScriptHook\\", m_GTA);
		snprintf(m_OutfitsPath, MAX_PATH, "%s\\Outfits\\", m_GTA);
		snprintf(m_OverseerPath, MAX_PATH, "%s\\Overseer.json", m_GTA);
		snprintf(m_RecentPlayersSavePath, MAX_PATH, "%s\\Recent Players.json", m_GTA);
		snprintf(m_SpoofingProfilesPath, MAX_PATH, "%s\\Profiles\\", m_GTA);

		if (std::filesystem::exists(m_ConfigPath)) {
			if (std::filesystem::file_size(m_ConfigPath) <= 0) {
				std::filesystem::remove(m_ConfigPath);
			}
		}
	}

	std::vector<std::string> Config::GetFilesInDirectory(std::string Folder, std::string Extension) {
		std::vector<std::string> Buffer;

		std::string loc = Folder + "*" + Extension;
		WIN32_FIND_DATAA fd;
		auto vit = FindFirstFileA(loc.c_str(), &fd);
		if (vit != INVALID_HANDLE_VALUE) {
			do {
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					std::string orig = fd.cFileName;
					Buffer.push_back(orig.substr(0, orig.length() - Extension.length()));
				}
			} while (FindNextFileA(vit, &fd));
			FindClose(vit);
		}

		return Buffer;
	}

	nlohmann::json ReadConfigContents() {
		nlohmann::json Json;

		std::ifstream Input(GetConfig()->GetConfigPath());
		if (Input.good()) {
			Input >> Json;
			Input.close();
		}

		return Json;
	}

	void WriteConfigContents(nlohmann::json Json) {
		std::ofstream Output(GetConfig()->GetConfigPath());
		if (Output.good()) {
			Output << Json.dump(4);
			Output.close();
		}
	}

	template<typename T>
	void BuildStack(nlohmann::json& Contents, std::stack<std::string> MenuStack, std::string Key, T Value) {
		std::vector<std::string> Names;
		while (!MenuStack.empty()) {
			Names.push_back(MenuStack.top());
			MenuStack.pop();
		}

		switch (Names.size()) {
		case 1:
			Contents[Names.at(0)][Key] = Value;
			break;
		case 2:
			Contents[Names.at(0)][Names.at(1)][Key] = Value;
			break;
		case 3:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Key] = Value;
			break;
		case 4:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Key] = Value;
			break;
		case 5:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Key] = Value;
			break;
		case 6:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Key] = Value;
			break;
		case 7:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Key] = Value;
			break;
		case 8:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Names.at(7)][Key] = Value;
			break;
		case 9:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Names.at(7)][Names.at(8)][Key] = Value;
			break;
		case 10:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Names.at(7)][Names.at(8)][Names.at(9)][Key] = Value;
			break;
		case 11:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Names.at(7)][Names.at(8)][Names.at(9)][Names.at(10)][Key] = Value;
			break;
		case 12:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Names.at(7)][Names.at(8)][Names.at(9)][Names.at(10)][Names.at(11)][Key] = Value;
			break;
		case 13:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Names.at(7)][Names.at(8)][Names.at(9)][Names.at(10)][Names.at(11)][Names.at(12)][Key] = Value;
			break;
		case 14:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Names.at(7)][Names.at(8)][Names.at(9)][Names.at(10)][Names.at(11)][Names.at(12)][Names.at(13)][Key] = Value;
			break;
		case 15:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Names.at(7)][Names.at(8)][Names.at(9)][Names.at(10)][Names.at(11)][Names.at(12)][Names.at(13)][Names.at(14)][Key] = Value;
			break;
		case 16:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Names.at(7)][Names.at(8)][Names.at(9)][Names.at(10)][Names.at(11)][Names.at(12)][Names.at(13)][Names.at(14)][Names.at(15)][Key] = Value;
			break;
		case 17:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Names.at(7)][Names.at(8)][Names.at(9)][Names.at(10)][Names.at(11)][Names.at(12)][Names.at(13)][Names.at(14)][Names.at(15)][Names.at(16)][Key] = Value;
			break;
		case 18:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Names.at(7)][Names.at(8)][Names.at(9)][Names.at(10)][Names.at(11)][Names.at(12)][Names.at(13)][Names.at(14)][Names.at(15)][Names.at(16)][Names.at(17)][Key] = Value;
			break;
		case 19:
			Contents[Names.at(0)][Names.at(1)][Names.at(2)][Names.at(3)][Names.at(4)][Names.at(5)][Names.at(6)][Names.at(7)][Names.at(8)][Names.at(9)][Names.at(10)][Names.at(11)][Names.at(12)][Names.at(13)][Names.at(14)][Names.at(15)][Names.at(16)][Names.at(17)][Names.at(18)][Key] = Value;
			break;
		}
	}

	nlohmann::json GetStackValue(std::stack<std::string> MenuStack, std::string Key) {
		nlohmann::json Contents = ReadConfigContents();
		if (!Contents.empty()) {
			bool Error = false;
			while (!MenuStack.empty()) {
				Contents = Contents[MenuStack.top()];
				if (Contents.is_null()) {
					Error = true;
					break;
				}

				MenuStack.pop();
			}

			if (Error) return nlohmann::json();

			if (!Contents[Key].is_null()) {
				return Contents[Key];
			}
		}

		return nlohmann::json();
	}

	std::stack<std::string> IncreaseStack(std::stack<std::string> MenuStack, std::vector<std::string> Stacks) {
		std::vector<std::string> Tmp;
		while (!MenuStack.empty()) {
			Tmp.push_back(MenuStack.top());
			MenuStack.pop();
		}

		for (auto& S : Stacks) Tmp.push_back(S);

		std::reverse(Tmp.begin(), Tmp.end());

		std::stack<std::string> Return;
		for (auto& S : Tmp) Return.push(S);
		return Return;
	}

	std::string Config::ReadString(std::stack<std::string> MenuStack, std::string Key, std::string DefaultString, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Json = GetStackValue(MenuStack, Key);
		if (Json.is_null() || !Json.is_string()) return DefaultString;
		return Json.get<std::string>();
	}

	void Config::WriteString(std::stack<std::string> MenuStack, std::string Key, std::string Value, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Contents = ReadConfigContents();
		BuildStack(Contents, MenuStack, Key, Value);
		WriteConfigContents(Contents);
	}

	int Config::ReadInt(std::stack<std::string> MenuStack, std::string Key, int DefaultInt, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Json = GetStackValue(MenuStack, Key);
		if (Json.is_null() || !Json.is_number_integer()) return DefaultInt;
		return Json.get<int>();
	}

	uint64_t Config::ReadUInt64(std::stack<std::string> MenuStack, std::string Key, uint64_t DefaultInt, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Json = GetStackValue(MenuStack, Key);
		if (Json.is_null() || !Json.is_number_integer()) return DefaultInt;
		return Json.get<uint64_t>();
	}

	void Config::WriteInt(std::stack<std::string> MenuStack, std::string Key, int Value, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Contents = ReadConfigContents();
		BuildStack(Contents, MenuStack, Key, Value);
		WriteConfigContents(Contents);
	}

	void Config::WriteUInt64(std::stack<std::string> MenuStack, std::string Key, uint64_t Value, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Contents = ReadConfigContents();
		BuildStack(Contents, MenuStack, Key, Value);
		WriteConfigContents(Contents);
	}

	float Config::ReadFloat(std::stack<std::string> MenuStack, std::string Key, float DefaultFloat, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Json = GetStackValue(MenuStack, Key);
		if (Json.is_null() || !Json.is_number_float()) return DefaultFloat;
		return Json.get<float>();
	}

	void Config::WriteFloat(std::stack<std::string> MenuStack, std::string Key, float Value, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Contents = ReadConfigContents();
		BuildStack(Contents, MenuStack, Key, Value);
		WriteConfigContents(Contents);
	}

	bool Config::ReadBool(std::stack<std::string> MenuStack, std::string Key, bool DefaultBool, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Json = GetStackValue(MenuStack, Key);
		if (Json.is_null() || !Json.is_boolean()) return DefaultBool;
		return Json.get<bool>();
	}

	void Config::WriteBool(std::stack<std::string> MenuStack, std::string Key, bool Value, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Contents = ReadConfigContents();
		BuildStack(Contents, MenuStack, Key, Value);
		WriteConfigContents(Contents);
	}

	bool Config::ReadColor(std::stack<std::string> MenuStack, std::string Key, Color* Col, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Json = GetStackValue(MenuStack, Key);
		if (Json.is_null() || !Json.is_array() || Json.size() != 4) return false;

		Col->r = Json[0].get<int>();
		Col->g = Json[1].get<int>();
		Col->b = Json[2].get<int>();
		Col->a = Json[3].get<int>();
		return true;
	}

	void Config::WriteColor(std::stack<std::string> MenuStack, std::string Key, Color Value, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Contents = ReadConfigContents();
		BuildStack(Contents, MenuStack, Key, nlohmann::json::array({ Value.r, Value.g, Value.b, Value.a }));
		WriteConfigContents(Contents);
	}

	bool Config::ReadVector(std::stack<std::string> MenuStack, std::string Key, Math::Vector3_<float>* Vec, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Json = GetStackValue(MenuStack, Key);
		if (Json.is_null() || !Json.is_array() || Json.size() != 3) return false;

		Vec->m_X = Json[0].get<int>();
		Vec->m_Y = Json[1].get<int>();
		Vec->m_Z = Json[2].get<int>();
		return true;
	}

	void Config::WriteVector(std::stack<std::string> MenuStack, std::string Key, Math::Vector3_<float> Value, std::vector<std::string> AdditionalStacks) {
		if (AdditionalStacks.size()) {
			MenuStack = IncreaseStack(MenuStack, AdditionalStacks);
		}

		nlohmann::json Contents = ReadConfigContents();
		BuildStack(Contents, MenuStack, Key, nlohmann::json::array({ Value.m_X, Value.m_Y, Value.m_Z }));
		WriteConfigContents(Contents);
	}

	Config* GetConfig() {
		static Config Instance;
		return &Instance;
	}
}