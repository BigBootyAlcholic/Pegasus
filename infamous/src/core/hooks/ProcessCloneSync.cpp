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

	eAckCode ProcessCloneSyncHook(CNetworkObjectMgr* _this,
								CNetGamePlayer* fromPlayer,
								CNetGamePlayer* toPlayer,
								eNetObjType objectType,
								u16 objectID,
								Rage::datBitBuffer* msgBuffer,
								u16 seqNum,
								u32 timeStamp)
	{

		Rage::netSyncTree* tree = Caller::Call<Rage::netSyncTree*>(Patterns::Vars::g_GetSyncTree, _this, objectType);
		if (!tree) {
			LOG_ERROR("No Sync Tree Found");
			return eAckCode::ACKCODE_FAIL;
		}

		Rage::netObject* netObject = Caller::Call<Rage::netObject*>(Patterns::Vars::g_GetNetObject, _this, objectID, true);

		if (netObject && netObject->m_object_type != (u16)objectType) {
			LOG_ERROR("Mismatch Type Crash");
			Menu::Notify::stacked(std::format("Blocked Sync Mismatch From {}", fromPlayer->get_name()).c_str());
			return eAckCode::ACKCODE_FAIL;
		}

		if (objectType < eNetObjType::NET_OBJ_TYPE_AUTOMOBILE || objectType > eNetObjType::NET_OBJ_TYPE_TRAIN) {
			LOG_ERROR("Out of Bounds Crash");
			Menu::Notify::stacked(std::format("Blocked Invalid Object Type From {}", fromPlayer->get_name()).c_str());
			return eAckCode::ACKCODE_FAIL;
		}

		//for can apply data
		g_SyncingPlayer = fromPlayer;
		g_SyncingObjectType = objectType;
		
		return OgProcessCloneSyncHook(_this, fromPlayer, toPlayer, objectType, objectID, msgBuffer, seqNum, timeStamp);

	}
}