#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "core/core.hpp"
#include "rage/types/network_types.hpp"
#include "rage/classes/rage/rlSessionInfo.hpp"

class NetworkMenu {
public:
	void Run();
	void Update();
};

static NetworkMenu* GetNetworkMenu() {
	static NetworkMenu instance;
	return &instance;
}

namespace NetworkMenuVars {
	struct Vars_ {
		bool m_SpeedUp;
		bool m_LogState;
		bool m_DisableProfanityFilter;

		bool m_JoinQueue = false;
		Rage::rlSessionInfo m_SessionInfo;

		bool m_RejoinDesyncedSessions;
		bool m_NotifyDesyncKicks;

		bool m_ToggleSessionType;
		int m_SessionType = 0;
		std::string m_GsType;

		bool m_OpenExtraSlot;

		bool m_InviteOnly = true;
		bool m_FriendOnly = true;
		bool m_CrewOnly = true;
	};

	extern Vars_ m_Vars;
}
