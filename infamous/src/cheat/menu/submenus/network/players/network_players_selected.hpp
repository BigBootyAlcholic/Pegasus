#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "core/core.hpp"
#include "rage/classes/network/netObject.hpp"
class NetworkPlayersSelectedMenu {
public:
	void Run();
	void Update();
};

static NetworkPlayersSelectedMenu* GetNetworkPlayersSelectedMenu() {
	static NetworkPlayersSelectedMenu instance;
	return &instance;
}

namespace NetworkPlayersSelectedMenuVars {
	struct Vars_ {
		std::unordered_map<Rage::netObject*, uint64_t> m_BrokenEntities;

		bool m_BreakNodes = false;
		uint32_t m_BreakNodeThreads[2] = { 0, 0 };
		int m_Crash = 0;

		bool m_SessionCrash = false;
		uint32_t m_SessionCrashTimeout = 0;
		int m_SessionCrashID = 0;

		bool m_TargetedCrash = false;
		uint64_t m_TargetedCrashTarget = 0;
		uint32_t m_TargetedCrashTimeout = 0;
		int m_TargetedCrashID = 0;

		int m_Commend;
		int m_BodyguardCount = 1;
		bool m_DemiGodmode;
		bool m_OffTheRadar;
		bool m_DisablePolice;
		bool m_Revenge;
	};

	extern Vars_ m_Vars;
}