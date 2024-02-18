#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "core/core.hpp"

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

	};

	extern Vars_ m_Vars;
}