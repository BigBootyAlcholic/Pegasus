#include "stdafx.hpp"
#include "network_tunables.hpp"
#include "cheat/util/player_manager.hpp"
#include "cheat/util/timer.hpp"
#include "cheat/util/global.hpp"
using namespace NetworkTunablesMenuVars;
using namespace Framework::Options;
namespace NetworkTunablesMenuVars {
	Vars_ m_Vars;

}

void NetworkTunablesMenu::Run() {
	Framework::addSubmenu("Tunables", "network_tunables", [](Framework::Options::Core* core) {
		core->addOption(ToggleOption("No Idle Kick")
			.addToggle(&m_Vars.m_DisableIdleKick));

		core->addOption(ToggleOption("Disable Transaction Error")
			.addToggle(&m_Vars.m_DisableTransactionError));
	});
}


void NetworkTunablesMenu::Update() {
	Run();

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
	//4537357
	//4537358
	//4537356
}