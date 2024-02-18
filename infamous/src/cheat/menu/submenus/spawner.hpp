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

class SpawnerMenu {
public:
	void Run();
	void Update();
};

static SpawnerMenu* GetSpawnerMenu() {
	static SpawnerMenu instance;
	return &instance;
}

namespace SpawnerMenuVars {
	struct Vars_ {
		int m_SelectedVehicleClass;
		std::string m_SpawnByName = "";

		bool m_Notification;
		bool m_Blip;
		bool m_ParticleFX;
		bool m_MaxUpgrades;
		bool m_TeleportInside;
		bool m_Godmode;
		bool m_Fade;
		bool m_AirVehiclesInAir;
		bool m_DeleteCurrent;

		float m_SpawnHeight = 80.f;
		
	};

	extern Vars_ m_Vars;
}
