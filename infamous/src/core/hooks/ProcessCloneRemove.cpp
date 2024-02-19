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
namespace Hooks {

	bool ProcessCloneRemoveHook(uint64_t This, CNetGamePlayer* NetGamePlayer, uint64_t R8, uint16_t NetworkID, uint64_t Stack) {
		if ((*Patterns::Vars::g_PedFactory) && (*Patterns::Vars::g_PedFactory)->m_local_ped) {
			Rage::netObject* Object = Caller::Call<Rage::netObject*>(Patterns::Vars::g_GetNetworkObjectFromNetworkID, This, NetworkID, true);
			if (Object == (*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object) {
				LOG_CUSTOM_WARN("ReceiveCloneRemove", "Trying to delete local player");
				return false;
			}
		}

		Rage::netObject* Object = Caller::Call<Rage::netObject*>(Patterns::Vars::g_GetNetworkObjectFromNetworkID, This, NetworkID, true);
		if (Object) {
			if (Object->m_object_type == NetObjectPlayer) {
				LOG(std::format("Preventing {} removing due to Ghost", NetGamePlayer->get_name()).c_str());
				Menu::Notify::stacked(std::format("Preventing {} removing due to Ghost", NetGamePlayer->get_name()).c_str());
				return false;
			}
		}

		return OgProcessCloneRemove(This, NetGamePlayer, R8, NetworkID, Stack);
	}
}