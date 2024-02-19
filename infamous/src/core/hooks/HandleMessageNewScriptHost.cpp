#include "stdafx.hpp"
#include "../hooks.hpp"
#include "rage/classes/enums/enums.hpp"
#include "rage/classes/rage/joaat.hpp"
#include "rage/classes/netsync/netSyncTree.hpp"
#include "util/caller.hpp"
#include "rage/classes/enums/enums.hpp"
#include "rage/classes/netsync/data_nodes.hpp"
#include "rage/classes/base/CObject.hpp"
#include "cheat/util/notify.hpp"
#include "SyncData.hpp"
#include "rage/engine.hpp"
#include "util/fiber.hpp"
#include "util/fiber_pool.hpp"
#include "cheat/util/control_manager.hpp"
#include "cheat/menu/submenus/protection.hpp"
#include "rage/classes/script/scriptManager.hpp"
namespace Hooks {

	bool HandleMsgNewScriptHostHook(uint64_t Rcx, uint64_t Rdx) {
		uint8_t Data[0x50];
		memset(Data, 0, 0x50);

		uint64_t ScriptTable = *(uint64_t*)(Rcx);
		*(uint64_t*)&Data[0] = Caller::Call<uint64_t>(*(uint64_t*)(ScriptTable + 0x78), Rcx);

		if (Caller::Call<bool>(Patterns::Vars::g_ReadNewScriptHostMessage, Data, *(uint64_t*)(Rdx), *(uint32_t*)(Rdx + 0x8), 0)) {
			CNetGamePlayer* NewHost = *(CNetGamePlayer**)(Rdx + 0x10);

			if (IsValidPtr(NewHost)) {
				uint16_t HostToken = *(uint16_t*)&Data[0x8];
				if (HostToken == 0xFFFF) {
					if (!Menu::GetPlayer(NewHost->m_player_id).m_ModderFlags[Menu::REASON_SCRIPT_MIGRATION]) {
						Menu::Notify::stacked(std::format("{} Forced Script Host", NewHost->get_name()).c_str());
					}

					Menu::GetPlayer(NewHost->m_player_id).m_IsModder = true;
					Menu::GetPlayer(NewHost->m_player_id).m_ModderFlags[Menu::REASON_SCRIPT_MIGRATION] = true;
				}

				CGameScriptHandler* ScriptHandler = Caller::Call<CGameScriptHandler*>(*(uint64_t*)(ScriptTable + 0x48), Rcx, *(uint64_t*)&Data[0]);
				if (IsValidPtr(ScriptHandler)) {
					if (IsValidPtr(ScriptHandler->m_script_thread)) {
						// LOG_INFO("%s is taking host of script %s from %s", NewHost->m_PlayerInfo->m_Name, ScriptHandler->m_Thread->m_Name, Menu::GetPlayer(Native::NetworkGetHostOfScript(ScriptHandler->m_Thread->m_Name, -1, 0)).m_Name);

						if (!strcmp(ScriptHandler->m_script_thread->m_name, "freemode")) {
							//if (NetworkProtectionNotificationsMenuVars::m_Vars.m_ScriptHostMigration) {
								if (NewHost->m_player_id != Native::NetworkGetHostOfScript(ScriptHandler->m_script_thread->m_name, -1, 0)) {
									Menu::Notify::stacked(std::format("{} Script Host Migration", NewHost->get_name()).c_str());
								}
							//}
						}
					}
				}
			}
		}

		return OgHandleMsgNewScriptHost(Rcx, Rdx);
	}
}