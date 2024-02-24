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
#include "cheat/menu/submenus/network/players/network_players_selected.hpp"
namespace Hooks {

	Menu::PlayerVars& GetPlayerFromRID(uint64_t RID) {
		int ID = -1;
		Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
			if (IsValidPtr(Player.m_NetGamePlayer)) {
				if (IsValidPtr(Player.m_NetGamePlayer->m_player_info)) {
					if (Player.m_NetGamePlayer->m_player_info->m_net_player_data.m_gamer_handle.m_rockstar_id == RID) {
						ID = Player.m_ID;
					}
				}
			}
			}, true, true);

		static Menu::PlayerVars Static;
		if (ID == -1) return Static;
		return Menu::GetPlayer(ID);
	}

	void PackCloneCreateHook(uint64_t This, Rage::netObject* Object, CNetGamePlayer* Player, uint64_t SyncData) {
		if (Object) {
			if (Object->m_object_type >= NetObjectAutomobile && Object->m_object_type < INVALID) {
				if (ProtectionMenuVars::m_Vars.m_Ghosts[Object->m_object_type].m_Enabled) {
					if (Object->m_object_type == NetObjectPlayer) return;
					if (ProtectionMenuVars::m_Vars.m_Ghosts[Object->m_object_type].m_DisableForSession) return;
				}

				if (Player->m_player_id >= 0 && Player->m_player_id < 34) {
					if (Menu::GetPlayer(Player->m_player_id).m_GhostDisable[Object->m_object_type]) {
						return;
					}

					if (Object->m_object_type == NetObjectPickup) {
						std::vector<int> DroppingStealth;
						Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
							if (Player.m_DropToggle && Player.m_DropStealth) {
								DroppingStealth.push_back(Player.m_ID);
							}
							}, true);

						bool Bad = false;
						if (DroppingStealth.size()) {
							for (int ID : DroppingStealth) {
								if (ID != Player->m_player_id) {
									Bad = true;
									break;
								}
							}
						}

						if (Bad) return;
					}
				
					if (NetworkPlayersSelectedMenuVars::m_Vars.m_TargetedCrash) {
						if (NetworkPlayersSelectedMenuVars::m_Vars.m_BrokenEntities.find(Object) != NetworkPlayersSelectedMenuVars::m_Vars.m_BrokenEntities.end()) {
							auto P = GetPlayerFromRID(NetworkPlayersSelectedMenuVars::m_Vars.m_BrokenEntities[Object]);
							if (P.m_Connected && P.m_NetGamePlayer) {
								LOG_ERROR("Packing clone create crash");
								return OgPackCloneCreate(This, Object, P.m_NetGamePlayer, SyncData);
							}

							return;
						}
					}
				}
			}
		}

		OgPackCloneCreate(This, Object, Player, SyncData);
	}
}