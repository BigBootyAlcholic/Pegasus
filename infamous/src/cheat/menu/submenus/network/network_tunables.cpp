#include "stdafx.hpp"
#include "network_tunables.hpp"
#include "cheat/util/player_manager.hpp"
#include "cheat/util/timer.hpp"
#include "cheat/util/global.hpp"
#include "cheat/util/script/script_patcher_service.hpp"
using namespace NetworkTunablesMenuVars;
using namespace Framework::Options;
namespace NetworkTunablesMenuVars {
	Vars_ m_Vars;
}

void NetworkTunablesMenu::Run() {
	Framework::addSubmenu("Tunables", "network_tunables", [](Framework::Options::Core* core) {
		core->addOption(ButtonOption("Remove Transaction Pending")
			.addClick([] {
				Menu::Global(4537357).As<int>() = 0;
				Menu::Global(4537358).As<int>() = 0;
				Menu::Global(4537356).As<int>() = 0;
			}));

		core->addOption(ButtonOption("Refresh Interior")
			.addClick([] { Native::RefreshInterior(Native::GetInteriorFromEntity(Menu::GetLocalPlayer().m_Entity)); }));

		core->addOption(ButtonOption("Force Cloud Save")
			.addClick([] { Native::StatSave(0, 0, 3); }));

		core->addOption(ToggleOption("No Idle Kick")
			.addToggle(&m_Vars.m_DisableIdleKick));

		core->addOption(ToggleOption("Disable Transaction Error")
			.addToggle(&m_Vars.m_DisableTransactionError));

		core->addOption(ToggleOption("Disable Despawn Bypass")
			.addToggle(&m_Vars.m_DespawnBypass));

		core->addOption(ToggleOption("All Players In Playerlist")
			.addToggle(&m_Vars.m_AllPlayersInPlayerList));

		core->addOption(ToggleOption("Disable Death Barriers")
			.addToggle(&m_Vars.m_DisableDeathBarriers));

		core->addOption(ToggleOption("Disable Stat Caps")
			.addToggle(&m_Vars.m_DisableStatCaps));

		core->addOption(ToggleOption("Disable Authorities Cooldown")
			.addToggle(&m_Vars.m_DisableBribeCopsCooldown));

		core->addOption(ToggleOption("Disable Ghost Organization")
			.addToggle(&m_Vars.m_DisableGhostOrgCooldown));

		core->addOption(ToggleOption("Disable BST Cooldown")
			.addToggle(&m_Vars.m_DisableBstCooldown));

		core->addOption(ToggleOption("Disable Ceo Call Vehicle Cooldown")
			.addToggle(&m_Vars.m_DisableCeoCallCooldown));

		core->addOption(ToggleOption("Disable Load Balancing")
			.addToggle(&m_Vars.m_DisableLoadBalancing));

		core->addOption(ToggleOption("Island Load Speedup")
			.addToggle(&m_Vars.m_IslandLoadSpeedup));

		core->addOption(ToggleOption("Allow Missions in Private Sessions")
			.addToggle(&m_Vars.m_SellMissionsBypass));

		core->addOption(ToggleOption("Bypass Casino Region Lock")
			.addToggle(&m_Vars.m_BypassCasinoRegionLock));

		core->addOption(ToggleOption("Decloack Players")
			.addToggle(&m_Vars.m_DecloackPlayers));

		core->addOption(ToggleOption("Block Muggers")
			.addToggle(&m_Vars.m_BlockMuggers));

		core->addOption(ToggleOption("Block Ceo Raids")
			.addToggle(&m_Vars.m_BlockCeoRaids));

		core->addOption(ToggleOption("Block Background Script")
			.addToggle(&m_Vars.m_BlockBackgroundScript));
	});
}


void RegisterScriptPatches() {

	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),"2D 01 04 00 ? 2C ? ? ? 5D ? ? ? 71 57 ? ? 2C", 5, {0x2E, 0x01, 0x00}, nullptr }); // script host kick
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "5D ? ? ? 76 57 ? ? 5D ? ? ? 76", 0, {0x2E, 0x00, 0x00}, nullptr }); // end session kick protection
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "2D 01 09 00 00 5D ? ? ? 56 ? ? 3A", 5, {0x2E, 0x01, 0x00}, &m_Vars.m_DisableDeathBarriers });// spectate death
	Menu::g_ScriptPatcher->AddPatch({ joaat("shop_controller"),  "2D 01 04 00 00 2C ? ? ? 56 ? ? 71", 5, {0x71, 0x2E, 0x01, 0x01}, &m_Vars.m_DespawnBypass }); // despawn bypass
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "71 2E ? ? 55 ? ? 61 ? ? ? 47 ? ? 63", 0, {0x72}, &m_Vars.m_IslandLoadSpeedup }); // load island even if stranded animal IPL choice is not set
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "2D 00 07 00 00 7B", 5, {0x2E, 0x00, 0x00}, &m_Vars.m_DisableLoadBalancing }); // disable population load balancing
	Menu::g_ScriptPatcher->AddPatch({ joaat("shop_controller"),  "2D 01 03 00 00 5D ? ? ? 06 56 ? ? 2E ? ? 2C", 5, {0x2E, 0x01, 0x00}, nullptr }); // godmode/invisibility detection bypass

	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "2D 01 03 00 00 38 00 71 72 5D ? ? ? 06 56 ? ? 71 2E ? ? 2C ? ? ? 71", 5, {0x72, 0x2E, 0x01, 0x01}, &m_Vars.m_AllPlayersInPlayerList });

	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "2D 01 08 00 ? 38 00 5D ? ? ? 2A 06", 5, {0x71, 0x2E, 0x01, 0x01}, &m_Vars.m_DecloackPlayers });
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "2D 02 08 00 00 38 01 56", 5, {0x2E, 0x02, 0x00}, &m_Vars.m_BlockMuggers });
	Menu::g_ScriptPatcher->AddPatch({ joaat("freemode"),  "2D 00 CF 00 00", 5, {0x2E, 0x00, 0x00}, &m_Vars.m_BlockCeoRaids });


	for (auto& entry : *Patterns::Vars::g_ProgramTable) {
		if (entry.m_program)
			Menu::g_ScriptPatcher->OnScriptLoad(entry.m_program);
	}
}

static bool once = false;
void NetworkTunablesMenu::Update() {
	Run();

	if (!once) {
		RegisterScriptPatches();
		once = true;
	}

	if (m_Vars.m_DisableIdleKick) {
		Menu::Global(1668317).At(1155).As<int>() = 0;
		Menu::Global(262145).At(87).As<int>() = 120000;
		Menu::Global(262145).At(88).As<int>() = 300000;
		Menu::Global(262145).At(89).As<int>() = 600000;
		Menu::Global(262145).At(90).As<int>() = 900000;

		Menu::Global(1668317).At(1171).As<int>() = 0;
		Menu::Global(262145).At(8429).As<int>() = 30000;
		Menu::Global(262145).At(8426).As<int>() = 60000;
		Menu::Global(262145).At(8427).As<int>() = 90000;
		Menu::Global(262145).At(8428).As<int>() = 120000;
	}

	//transaction error
	if (m_Vars.m_DisableTransactionError) {
		Menu::Global(4537357).As<int>() = 0;
		Menu::Global(4537358).As<int>() = 0;
		Menu::Global(4537356).As<int>() = 0;
	}

	if (m_Vars.m_DisableBribeCopsCooldown) {
		Menu::Global(262145).At(13011).As<int>() = 1;
	}

	if (m_Vars.m_DisableGhostOrgCooldown) {
		Menu::Global(262145).At(13010).As<int>() = 1;
	}

	if (m_Vars.m_DisableBstCooldown) {
		Menu::Global(262145).At(13009).As<int>() = 1;
	}

	if (m_Vars.m_DisableCeoCallCooldown) {
		Menu::Global(262145).At(13005).As<int>() = 1;
	}

	//4537357
	//4537358
	//4537356
}