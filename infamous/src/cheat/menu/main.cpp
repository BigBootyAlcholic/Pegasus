#include "stdafx.hpp"
#include "main.hpp"
#include "cheat/util/notify.hpp"
#include "cheat/util/input.hpp"
#include "core/patterns.hpp"
#include "submenus/player.hpp"
#include "submenus/protection.hpp"
#include "cheat/util/shop_manager.hpp"
#include "cheat/util/texture.hpp"
#include "submenus/network.hpp"
#include "submenus/spawner.hpp"
#include "submenus/weapon.hpp"
#include "cheat/util/global.hpp"
#include "submenus/world.hpp"
#include "submenus/vehicle.hpp"
#include "submenus/settings.hpp"
#include "submenus/teleport.hpp"
#include "util/fiber.hpp"
#include "util/fiber_pool.hpp"
#include "submenus/recovery.hpp"
#include "../shv/shv.hpp"
using namespace MainMenuVars;
using namespace Patterns::Vars;
namespace MainMenuVars {
	Vars_ m_Vars;

	void TriggerTrx(int Hash, int Amount) {
		Menu::Global(4537212).At(1).As<int>() = 2147483646;
		Menu::Global(4537212).At(7).As<int>() = 2147483647;
		Menu::Global(4537212).At(6).As<int>() = 0;
		Menu::Global(4537212).At(5).As<int>() = 0;
		Menu::Global(4537212).At(3).As<int>() = Hash;
		Menu::Global(4537212).At(2).As<int>() = Amount;
		Menu::Global(4537212).As<int>() = 2;

	}
}
static bool Init = false;
bool m_TestLoop;
void MainMenu::Run() {
	Framework::addSubmenu("Main Menu", "Main Menu", [](Framework::Options::Core* core) {

		core->addOption(Framework::Options::SubmenuOption("Player")
			.setTarget("player-menu"));

		core->addOption(Framework::Options::SubmenuOption("Network")
			.setTarget("network-menu"));

		core->addOption(Framework::Options::SubmenuOption("Protections")
			.setTarget("protections"));

		core->addOption(Framework::Options::SubmenuOption("Weapons")
			.setTarget("weapon-menu"));

		core->addOption(Framework::Options::SubmenuOption("Vehicle")
			.setTarget("vehicle-menu"));

		core->addOption(Framework::Options::SubmenuOption("Spawner")
			.setTarget("spawner-menu"));

		core->addOption(Framework::Options::SubmenuOption("Teleport")
			.setTarget("teleport-menu"));

		core->addOption(Framework::Options::SubmenuOption("World")
			.setTarget("world-menu"));

		core->addOption(Framework::Options::SubmenuOption("Recovery")
			.setTarget("recovery-menu"));

		core->addOption(Framework::Options::SubmenuOption("Settings")
			.setTarget("settings-menu"));

		if (!Init) {
			if (!GetModuleHandleA("ScriptHookV.dll")) {
				FILE* Fp = fopen((Utils::GetConfig()->GetTempPath() + std::string("ScriptHookV.dll")).c_str(), "wb");
				if (Fp) {
					fwrite(ScriptHook::Proxy::ScriptHookDummy, 1, sizeof(ScriptHook::Proxy::ScriptHookDummy), Fp);
					fclose(Fp);
				}

				if (LoadLibraryA((Utils::GetConfig()->GetTempPath() + std::string("ScriptHookV.dll")).c_str())) {
					Menu::Notify::stacked("ScriptHookV loaded!");
				}
				else {
					Menu::Notify::stacked("~r~ScriptHookV failed to load!");
				}
			}

			for (auto Script : Utils::GetConfig()->GetFilesInDirectory(Utils::GetConfig()->GetScriptHookPath(), ".asi")) {
				core->addOption(Framework::Options::ToggleOption(Script.c_str())
					.addToggle(&ScriptHook::Vars::m_ScriptState[Script + ".asi"].m_Loaded)
					.addClick([=] { ScriptHook::HandleToggle(Script + ".asi"); }));

			}

			for (auto Script : Utils::GetConfig()->GetFilesInDirectory(Utils::GetConfig()->GetScriptHookPath(), ".dll")) {
				core->addOption(Framework::Options::ToggleOption(Script.c_str())
					.addToggle(&ScriptHook::Vars::m_ScriptState[Script + ".dll"].m_Loaded)
					.addClick([=] { ScriptHook::HandleToggle(Script + ".dll"); }));
			}

			Init = true;
		}

		

	});
}



/*bool enabled{ false };
		bool rainbow{ false };
		float intensity{ 100 };
		void init() {
			if (!enabled) return;
			g_pointers->m_timecycle->set_var(TimeCycleVar_e::TCVAR_PED_LIGHT_COL_R, rainbow ? (g_rainbow.r / 255.f) : 1);
			g_pointers->m_timecycle->set_var(TimeCycleVar_e::TCVAR_PED_LIGHT_COL_G, rainbow ? (g_rainbow.g / 255.f) : 1);
			g_pointers->m_timecycle->set_var(TimeCycleVar_e::TCVAR_PED_LIGHT_COL_B, rainbow ? (g_rainbow.b / 255.f) : 1);
			g_pointers->m_timecycle->set_var(TimeCycleVar_e::TCVAR_PED_LIGHT_MULT, intensity);
		}
		void on_disable() {
			g_pointers->m_timecycle->set_var(TimeCycleVar_e::TCVAR_PED_LIGHT_MULT, 0);
		}*/




void MainMenu::Update() {
	Menu::textures::tick();
	Native::DisableControlAction(0, 27, true);
	Framework::GetFrameWork()->Run();
	Run();



	if (g_EnablePlayerInfo && Native::DoesEntityExist(Menu::GetSelectedPlayer().m_Ped) && Menu::GetSelectedPlayer().m_NetGamePlayer->is_valid()) {
		Framework::GetFrameWork()->DrawPlayerInfo(Menu::GetSelectedPlayer().m_ID);
	}

	if (m_Vars.m_SeamlessJoin) {
		const auto TransitionState = Menu::Global(1575008).As<eTransitionState>();

		if (TransitionState <= eTransitionState::TRANSITION_STATE_FM_FINAL_SETUP_PLAYER) {
			Native::SetPlayerControl(Native::PlayerId(), true, 0);
			if (Native::IsPlayerSwitchInProgress()) {
				Native::StopPlayerSwitch();
				Native::_StopAllScreenEffects();
			}
		}
	}


	if (g_WaypointData) {
		if (g_WaypointData->m_points && g_WaypointData->m_count) {
			for (uint32_t i = 0; i < g_WaypointData->m_count; i++) {
				if (i >= 40) break;

				Math::Vector4<float> point = g_WaypointData->m_points[i];
				Native::DrawMarker(28,  point.m_X, point.m_Y, point.m_Z + 0.5f ,  0.f, 0.f, 0.f ,  0.f, 0.f, 0.f , 0.5f, 0.5f, 0.5f , 255, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0);
			}
		}
	}

	if (m_Vars.m_TestLoopV2) {
		Utils::GetFiberPool()->Push([] {
			TriggerTrx(0x176D9D54, 15000000);//bend job
			TriggerTrx(0xED97AFC1, 7000000);//gang money
			TriggerTrx(0xA174F633, 15000000);//bend bonus
			TriggerTrx(0x314FB8B0, 1000000);//daily object event money
			TriggerTrx(0x4B6A869C, 2000000);//buisness hub money
			Utils::GetFiberManager()->Sleep(4000);
		});
	}

	//4537212
	/* SetGlobalInt(4537212+ 1, 2147483646)
    SetGlobalInt(4537212+ 7, 2147483647)
    SetGlobalInt(4537212+ 6, 0)
    SetGlobalInt(4537212+ 5, 0)
    SetGlobalInt(4537212+ 3, hash)
    SetGlobalInt(4537212+ 2, amount)
    SetGlobalInt(4537212,2)*/


	GetProtectionMenu()->Update();
	GetPlayerMenu()->Update();
	GetNetworkMenu()->Update();
	GetSpawnerMenu()->Update();
	GetWeaponMenu()->Update();
	GetWorldMenu()->Update();
	GetVehicleMenu()->Update();
	GetSettingsMenu()->Update();
	GetTeleportMenu()->Update();
	GetRecoveryMenu()->Update();
}