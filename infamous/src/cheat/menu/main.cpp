#include "stdafx.hpp"
#include "main.hpp"
#include "cheat/util/notify.hpp"
#include "cheat/util/input.hpp"
#include "core/patterns.hpp"
#include "submenus/player.hpp"
#include "submenus/protection.hpp"
#include "cheat/util/script/script_patcher_service.hpp"
#include "cheat/util/shop_manager.hpp"
#include "cheat/util/texture.hpp"
#include "submenus/network.hpp"
#include "submenus/spawner.hpp"
#include "submenus/weapon.hpp"
#include "cheat/util/global.hpp"
#include "submenus/world.hpp"
#include "submenus/vehicle.hpp"
using namespace MainMenuVars;
using namespace Patterns::Vars;
namespace MainMenuVars {
	Vars_ m_Vars;
}
bool m_TestLoop;
void MainMenu::Run() {
	Framework::addSubmenu("Main Menu", "Main Menu", [](Framework::Options::Core* core) {

		core->addOption(Framework::Options::SubmenuOption("Player")
			.addTooltip("Tooltip Test TMP on my dick frfr ong Txl is a pussy Shawarma beef + turkey on top with thina and pickles on the side frfr").setTarget("player-menu"));

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

		core->addOption(Framework::Options::SubmenuOption("World")
			.setTarget("world-menu"));

		core->addOption(Framework::Options::ToggleOption("$1m Loop")
			.addToggle(&m_TestLoop));


		core->addOption(Framework::Options::ButtonOption("Unload")
			.addClick([] { Core::g_Running = false; }));
	});
}

void RegisterScriptPatches() {

	//ScriptPatch(Rage::joaat_t script, const Memory::Pattern pattern, std::int32_t offset, std::vector<std::uint8_t> patch, bool* enable_bool);

	/*Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),"2D 01 04 00 ? 2C ? ? ? 5D ? ? ? 71 57 ? ? 2C", 5, {0x2E, 0x01, 0x00}, nullptr }); // script host kick
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "5D ? ? ? 76 57 ? ? 5D ? ? ? 76", 0, {0x2E, 0x00, 0x00}, nullptr }); // end session kick protection
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "2D 01 09 00 00 5D ? ? ? 56 ? ? 3A", 5, {0x2E, 0x01, 0x00}, nullptr }); // disable death when undermap/spectating
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "71 2E ? ? 55 ? ? 61 ? ? ? 47 ? ? 63", 0, {0x72}, nullptr }); // load island even if stranded animal IPL choice is not set
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "2D 00 07 00 00 7B", 5, {0x2E, 0x00, 0x00}, nullptr }); // disable population load balancing
	Menu::g_ScriptPatcher->AddPatch({ joaat("shop_controller"),  "2D 01 04 00 00 2C ? ? ? 56 ? ? 71", 5, {0x71, 0x2E, 0x01, 0x01}, nullptr }); // despawn bypass
	Menu::g_ScriptPatcher->AddPatch({ joaat("shop_controller"),  "2D 01 03 00 00 5D ? ? ? 06 56 ? ? 2E ? ? 2C", 5, {0x2E, 0x01, 0x00}, nullptr }); // godmode/invisibility detection bypass*/

	bool True = true;

	/*Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"), "2D 00 03 00 00 5D ? ? ? 71 08", 5, { 0x2E, 0x00, 0x00 }, &True }); // Anti-Afk
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),
		"2D 00 03 00 00 5D ? ? ? 56 ? ? 72 2E ? ? 62",
		5,
		{ 0x72, 0x2E, 0x00, 0x01 },
		&True }); // Anti-Afk 2
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"), "2D 00 07 00 00 7B", 5, { 0x2E, 0x00, 0x00 }, &True }); // Disable population load balancing
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"), "2D 01 09 00 00 5D ? ? ? 56 ? ? 2E", 5, {0x2E, 0x01, 0x00}, &True }); // Disable death when undermap/spectating
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"), "2D 01 04 00 ? 2C ? ? ? 5D ? ? ? 71 57 ? ? 2C", 5, { 0x2E, 0x01, 0x00 }, &True }); // Script host kick
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"), "5D ? ? ? 76 57 ? ? 5D ? ? ? 76", 0, { 0x2E, 0x00, 0x00 }, &True }); // End session kick protection*/

	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),"2D 01 04 00 ? 2C ? ? ? 5D ? ? ? 71 57 ? ? 2C", 5, {0x2E, 0x01, 0x00}, nullptr }); // script host kick
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "5D ? ? ? 76 57 ? ? 5D ? ? ? 76", 0, {0x2E, 0x00, 0x00}, nullptr }); // end session kick protection
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "2D 01 09 00 00 5D ? ? ? 56 ? ? 3A", 5, {0x2E, 0x01, 0x00}, nullptr });

	/* // disable death when undermap/spectating*/
	//Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "71 2E ? ? 55 ? ? 61 ? ? ? 47 ? ? 63", 0, {0x72}, nullptr }); // load island even if stranded animal IPL choice is not set
//	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "2D 00 07 00 00 7B", 5, {0x2E, 0x00, 0x00}, nullptr }); // disable population load balancing
	//Menu::g_ScriptPatcher->AddPatch({ joaat("shop_controller"),  "2D 01 04 00 00 2C ? ? ? 56 ? ? 71", 5, {0x71, 0x2E, 0x01, 0x01}, nullptr }); // despawn bypass
//	Menu::g_ScriptPatcher->AddPatch({ joaat("shop_controller"),  "2D 01 03 00 00 5D ? ? ? 06 56 ? ? 2E ? ? 2C", 5, {0x2E, 0x01, 0x00}, nullptr }); // godmode/invisibility detection bypass


/*	Menu::g_ScriptPatcher->AddPatch({ joaat("maintransition"), "2D 00 02 00 00 2C ? ? ? 56 ? ? 2C ? ? ? 74 58 ? ? 2C ? ? ? 73", 5, {0x72, 0x2E, 0x00, 0x01}, &MainMenuVars::m_Vars.m_SeamlessJoin });
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"), "2D 01 03 00 00 71 61 ? ? ? 42 ? 25", 5, {0x2E, 0x01, 0x00}, &MainMenuVars::m_Vars.m_SeamlessJoin });*/

	for (auto& entry : *Patterns::Vars::g_ProgramTable) {
		if (entry.m_program)
			Menu::g_ScriptPatcher->OnScriptLoad(entry.m_program);
	}
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
	//register_script_patches();
	Menu::textures::tick();
	Native::DisableControlAction(0, 27, true);
	Framework::GetFrameWork()->Run();
	Run();

	//RegisterScriptPatches();

	/*SetTimeCycleVar(eTimeCycleVar::TCVAR_FOG_FOGRAY_INTENSITY, 0);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_FOGVOLUME_FOG_SCALER, 0);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_FOG_DENSITY, 0);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_FOG_SHADOW_AMOUNT, 0);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_FOG_VOLUME_INTENSITY_SCALE, 0);*/


	if (m_Vars.m_SeamlessJoin) {
		const auto TransitionState = *Menu::Global(1575008).As<eTransitionState*>();

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

	if (m_TestLoop) {
		Menu::NetShoppingManager::ProcessServiceTransaction(joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1633116913, joaat("NET_SHOP_ACTION_EARN"), 1000000, 4);
	}


	GetProtectionMenu()->Update();
	GetPlayerMenu()->Update();
	GetNetworkMenu()->Update();
	GetSpawnerMenu()->Update();
	GetWeaponMenu()->Update();
	GetWorldMenu()->Update();
	GetVehicleMenu()->Update();
}