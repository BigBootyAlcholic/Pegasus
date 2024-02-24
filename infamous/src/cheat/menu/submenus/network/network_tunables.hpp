#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "core/core.hpp"

class NetworkTunablesMenu {
public:
	void Run();
	void Update();
};

static NetworkTunablesMenu* GetNetworkTunablesMenu() {
	static NetworkTunablesMenu instance;
	return &instance;
}

namespace NetworkTunablesMenuVars {
	struct Vars_ {
		bool m_DisableTransactionError = false;
		bool m_DisableIdleKick = false;
		bool m_DespawnBypass = true;
		bool m_BlockMuggers;
		bool m_BlockCeoRaids;
		bool m_AllPlayersInPlayerList;
		bool m_DisableDeathBarriers;
		bool m_BypassCasinoRegionLock;
		bool m_BypassInteriorRestrictions;
		bool m_DecloackPlayers;

		bool m_SellMissionsBypass;
		bool m_DisableStatCaps;
		bool m_DisableGhostOrgCooldown;
		bool m_DisableBstCooldown;
		bool m_DisableBribeCopsCooldown;
		bool m_DisableCeoCallCooldown;

		bool m_IslandLoadSpeedup;
		bool m_DisableLoadBalancing;
		bool m_BlockBackgroundScript;

	};

	extern Vars_ m_Vars;
}