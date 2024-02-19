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

	void WritePlayerGameStateDataNodeHook(Rage::netObject* Player, CPlayerGameStateDataNode* Node) {
		OgWritePlayerGameStateDataNode(Player, Node);

		if (ProtectionMenuVars::m_Vars.m_SuperJump) {
			Node->m_super_jump = false;
		}

		if (ProtectionMenuVars::m_Vars.m_Godmode) {
			Node->m_is_invincible = false;
			Node->m_bullet_proof = false;
			Node->m_collision_proof = false;
			Node->m_explosion_proof = false;
			Node->m_fire_proof = false;
			Node->m_melee_proof = false;
			Node->m_steam_proof = false;
			Node->m_water_proof = false;
		}

		if (ProtectionMenuVars::m_Vars.m_Spectate) {
			Node->m_is_spectating = false;
			Node->m_spectating_net_id = 0;
		}

	}
}