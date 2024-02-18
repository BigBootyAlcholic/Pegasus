#include "stdafx.hpp"
#include "../hooks.hpp"
#include "SyncData.hpp"
namespace Hooks {

	bool CanApplyDataHook(Rage::netSyncTree* tree, Rage::netObject* object) {
		static bool init = ([] { SyncData::SyncNode::Init(); }(), true);

		SyncData::VehicleCreationModel = std::nullopt;

		if (tree->m_child_node_count && tree->m_next_sync_node && SyncData::CheckNode(tree->m_next_sync_node, g_SyncingPlayer, object)) {
			return false;
		}

		return OgCanApplyDataHook(tree, object);
	}

}