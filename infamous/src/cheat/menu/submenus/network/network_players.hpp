#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "core/core.hpp"

class NetworkPlayersMenu {
public:
	void Run();
	void Update();
};

static NetworkPlayersMenu* GetNetworkPlayersMenu() {
	static NetworkPlayersMenu instance;
	return &instance;
}

namespace NetworkPlayersMenuVars {
	struct Vars_ {
		uint32_t m_SelectedPlayer = 0;
	};

	extern Vars_ m_Vars;
}