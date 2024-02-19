#pragma once

#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "framework/options/number.hpp"
#include "core/core.hpp"

class TeleportMenu {
public:
	void Run();
	void Update();
};

static TeleportMenu* GetTeleportMenu() {
	static TeleportMenu instance;
	return &instance;
}

namespace TeleportMenuVars {
	struct Vars_ {

		bool m_Animation = true;
		bool m_KeepVehicle = true;
		float m_Directions[6] = { 5.f, 5.f, 5.f, 5.f, 5.f, 5.f };
	};

	extern Vars_ m_Vars;
}
