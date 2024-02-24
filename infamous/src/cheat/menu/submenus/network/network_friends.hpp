#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "core/core.hpp"

class NetworkFriendsMenu {
public:
	void Run();
	void Update();
};

static NetworkFriendsMenu* GetNetworkFriendsMenu() {
	static NetworkFriendsMenu instance;
	return &instance;
}

namespace NetworkFriendsMenuVars {
	struct Vars_ {
		int m_Type; // 0 = selected, 1 = all
		Friends* m_Selected;

	};

	extern Vars_ m_Vars;
}