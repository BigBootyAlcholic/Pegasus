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
namespace Hooks {

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
				}
			}
		}

		OgPackCloneCreate(This, Object, Player, SyncData);
	}
}