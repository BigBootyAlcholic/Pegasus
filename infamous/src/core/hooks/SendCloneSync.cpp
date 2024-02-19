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

	bool SendCloneSyncHook(uint64_t This, Rage::netObject* NetObj, CNetGamePlayer* NetGamePlayer) {
		if (ProtectionMenuVars::m_Vars.m_BlockOutGoingSyncs) {
			LOG_WARN("BLOCKING SEND SYNC PROCESS");
			return false;
		}
		return OgSendCloneSync(This, NetObj, NetGamePlayer);
	}
}