#pragma once
#include "stdafx.hpp"
#include "framework/types.hpp"

namespace Utils {
	class Config {
	public:
		void Initialize();
		std::vector<std::string> GetFilesInDirectory(std::string Folder, std::string Extension = ".txt");

		std::string ReadString(std::stack<std::string> MenuStack, std::string Key, std::string DefaultString = "", std::vector<std::string> AdditionalStacks = {});
		int ReadInt(std::stack<std::string> MenuStack, std::string Key, int DefaultInt = 0, std::vector<std::string> AdditionalStacks = {});
		uint64_t ReadUInt64(std::stack<std::string> MenuStack, std::string Key, uint64_t DefaultInt = 0, std::vector<std::string> AdditionalStacks = {});
		float ReadFloat(std::stack<std::string> MenuStack, std::string Key, float DefaultFloat = 0.f, std::vector<std::string> AdditionalStacks = {});
		bool ReadBool(std::stack<std::string> MenuStack, std::string Key, bool DefaultBool = false, std::vector<std::string> AdditionalStacks = {});
		bool ReadColor(std::stack<std::string> MenuStack, std::string Key, Color* Col, std::vector<std::string> AdditionalStacks = {});
		bool ReadVector(std::stack<std::string> MenuStack, std::string Key, Math::Vector3_<float>* Vec, std::vector<std::string> AdditionalStacks = {});

		void WriteString(std::stack<std::string> MenuStack, std::string Key, std::string Value, std::vector<std::string> AdditionalStacks = {});
		void WriteInt(std::stack<std::string> MenuStack, std::string Key, int Value, std::vector<std::string> AdditionalStacks = {});
		void WriteUInt64(std::stack<std::string> MenuStack, std::string Key, uint64_t Value, std::vector<std::string> AdditionalStacks = {});
		void WriteFloat(std::stack<std::string> MenuStack, std::string Key, float Value, std::vector<std::string> AdditionalStacks = {});
		void WriteBool(std::stack<std::string> MenuStack, std::string Key, bool Value, std::vector<std::string> AdditionalStacks = {});
		void WriteColor(std::stack<std::string> MenuStack, std::string Key, Color Value, std::vector<std::string> AdditionalStacks = {});
		void WriteVector(std::stack<std::string> MenuStack, std::string Key, Math::Vector3_<float> Value, std::vector<std::string> AdditionalStacks = {});

		std::string GetDocumentsPath() { return m_DocumentsPath; }
		std::string GetEntityLimitsPath() { return m_EntityLimitsPath; }
		std::string GetGTAPath() { return m_GTA; }
		std::string GetAppDataPath() { return m_AppdataPath; }
		std::string GetConfigPath() { return m_ConfigPath; }
		std::string GetLogPath() { return m_LogPath; }
		std::string GetThemePath() { return m_ThemePath; }
		std::string GetTranslationPath() { return m_TranslationPath; }
		std::string GetScriptHookPath() { return m_ScriptHookPath; }
		std::string GetOverseerPath() { return m_OverseerPath; }
		std::string GetTeleportSavePath() { return m_TeleportSavePath; }
		std::string GetRecentPlayersPath() { return m_RecentPlayersSavePath; }
		std::string GetOutfitPath() { return m_OutfitsPath; }
		std::string GetAuthPath() { return m_AuthPath; }
		std::string GetRainbowPath() { return m_RainbowPath; }
		std::string GetHandlingPath() { return m_HandlingPath; }
		std::string GetMenyooPath() { return m_MenyooPath; }
		std::string GetTextMessagePath() { return m_TextMessagePath; }
		std::string GetTempPath() { return m_TmpPath; }
		std::string GetAmmoModifiersPath() { return m_AmmoModifiersPath; }
		std::string GetPlayerLogPath() { return m_PlayerLogPath; }
		std::string GetLSCPath() { return m_LSCPath; }
		std::string GetTextureSwapPath() { return m_TextureSwapPath; }
		std::string GetTextureSwapFolderPath() { return m_TextureSwapFolderPath; }
		std::string GetSpoofingProfilesPath() { return m_SpoofingProfilesPath; }
	private:
		char m_DocumentsPath[MAX_PATH];
		char m_GTA[MAX_PATH];
		char m_AmmoModifiersPath[MAX_PATH];
		char m_EntityLimitsPath[MAX_PATH];
		char m_AuthPath[MAX_PATH];
		char m_AppdataPath[MAX_PATH];
		char m_TmpPath[MAX_PATH];
		char m_ConfigPath[MAX_PATH];
		char m_LogPath[MAX_PATH];
		char m_ThemePath[MAX_PATH];
		char m_TranslationPath[MAX_PATH];
		char m_ScriptHookPath[MAX_PATH];
		char m_OverseerPath[MAX_PATH];
		char m_TeleportSavePath[MAX_PATH];
		char m_RecentPlayersSavePath[MAX_PATH];
		char m_OutfitsPath[MAX_PATH];
		char m_RainbowPath[MAX_PATH];
		char m_HandlingPath[MAX_PATH];
		char m_MenyooPath[MAX_PATH];
		char m_TextMessagePath[MAX_PATH];
		char m_PlayerLogPath[MAX_PATH];
		char m_LSCPath[MAX_PATH];
		char m_TextureSwapPath[MAX_PATH];
		char m_TextureSwapFolderPath[MAX_PATH];
		char m_SpoofingProfilesPath[MAX_PATH];
	};

	Config* GetConfig();
}