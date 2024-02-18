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
