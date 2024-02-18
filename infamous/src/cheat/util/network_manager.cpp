#include "stdafx.hpp"
#include "network_manager.hpp"
#include "rage/classes/network/CNetworkPlayerMgr.hpp"
namespace Menu::Network {

	inline GtaThread* FindScriptThread(Rage::joaat_t hash) {
		for (auto thread : *Patterns::Vars::g_ScriptThreads) {
			if (thread && thread->m_context.m_thread_id && thread->m_handler && thread->m_script_hash == hash) {
				return thread;
			}
		}

		return nullptr;
	}

	inline GtaThread* FindScriptThreadById(std::uint32_t id) {
		for (auto thread : *Patterns::Vars::g_ScriptThreads) {
			if (thread && thread->m_handler && thread->m_context.m_thread_id == id) {
				return thread;
			}
		}

		return nullptr;
	}

	inline Rage::scrProgram* FindScriptProgram(Rage::joaat_t hash) {
		for (auto& script : *Patterns::Vars::g_ProgramTable) {
			if (script.m_program && script.m_program->m_name_hash == hash)
				return script.m_program;
		}

		return nullptr;
	}

	bool ForceScriptHost(Rage::joaat_t Script) {
		if (auto ScriptThread = FindScriptThread(Script); ScriptThread && ScriptThread->m_net_component) {
			for (int i = 0; !((CGameScriptHandlerNetComponent*)ScriptThread->m_net_component)->is_local_player_host(); i++) {
				if (i > 200) {
					Menu::Notify::stacked(std::format("Failed to Force Host Of Script {}", Script).c_str());
					return false;
				}
				((CGameScriptHandlerNetComponent*)ScriptThread->m_net_component)->send_host_migration_event((*Patterns::Vars::g_NetworkPlayerMgr)->m_local_net_player);
				Utils::GetFiberManager()->GoToMainFiber();

				if (!ScriptThread->m_stack || !ScriptThread->m_net_component)
					return false;
			}
		}
		Vars::m_ForcedScriptHost = true;
		Menu::Notify::stacked("Forced Script Host Sucessfully");
		return true;
	}

	void PreventStealing() {
		Rage::joaat_t Script = Joaat("freemode");
		if (Vars::m_PreventStealing && Vars::m_ForcedScriptHost) {
			if (auto ScriptThread = FindScriptThread(Script); ScriptThread && ScriptThread->m_net_component) {
				if (!((CGameScriptHandlerNetComponent*)ScriptThread->m_net_component)->is_local_player_host()) {
					if (ForceScriptHost(Script)) {
						Menu::Notify::stacked("Prevented Someone From Stealing Script Host");
					}
				}
			}
		}
	}
}