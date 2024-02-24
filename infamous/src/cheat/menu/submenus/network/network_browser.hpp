#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "core/core.hpp"
#include "framework/options/number.hpp"
#include "cheat/util/matchmaking_manager.hpp"
class NetworkBrowserMenu {
public:
	void Run();
	void Update();
};

static NetworkBrowserMenu* GetNetworkBrowserMenu() {
	static NetworkBrowserMenu instance;
	return &instance;
}

namespace NetworkBrowserMenuVars {

	struct Vars_ {
		bool m_EnableRegionFilter = true;
		int m_RegionFilter = 0;

		bool m_EnableLanguageFilter = false;
		int m_LangaugeFilter = 0;

		bool m_EnablePoolFilter = false;
		int m_PoolFilter = 0;

		bool m_EnablePlayerCountFilter = false;
		int m_PlayerCountFilterMin = 0;
		int m_PlayerCountFilterMax = 32;

		int m_SortMethod = 0;
		int m_SortDirection = 0;

		bool m_Populate = false;

		Menu::MatchmakingManager::Session m_FoundSession;
	};

	extern Vars_ m_Vars;
}