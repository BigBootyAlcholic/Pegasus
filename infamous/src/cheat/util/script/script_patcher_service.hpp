#pragma once 
#include "script_patch.hpp"
#include "script_data.hpp"
#include "rage/classes/script/scrProgram.hpp"
namespace Menu
{
	class ScriptPatcher
	{
		std::vector<ScriptPatch> m_ScriptPatches;
		std::unordered_map<Rage::joaat_t, std::unique_ptr<ScriptData>> m_ScriptData;
		ScriptData* GetDataForScript(Rage::joaat_t script);
		bool DoesScriptHavePatches(Rage::joaat_t script);
		void CreateDataForScript(Rage::scrProgram* program);
		void UpdateAllPatchesForScript(Rage::joaat_t script);

	public:
		ScriptPatcher();
		~ScriptPatcher();

		void AddPatch(ScriptPatch&& patch);
		void OnScriptLoad(Rage::scrProgram* program);
		std::uint8_t** GetScriptBytecode(Rage::joaat_t script);
		void Update();
	};

	inline ScriptPatcher* g_ScriptPatcher;
}