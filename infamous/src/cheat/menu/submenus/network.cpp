#include "stdafx.hpp"
#include "network.hpp"
#include "network/network_players.hpp"
#include "network/network_tunables.hpp"
#include "cheat/util/network_manager.hpp"
#include "cheat/util/timer.hpp"
#include "rage/engine.hpp"
#include "util/fiber_pool.hpp"
#include "rage/classes/network/Network.hpp"
#include "network/network_spoofing.hpp"

void NetworkMenu::Run() {
	Framework::addSubmenu("Network", "network-menu", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Players")
			.setTarget("network_players"));

		core->addOption(Framework::Options::SubmenuOption("Tunables")
			.setTarget("network_tunables"));

		core->addOption(Framework::Options::SubmenuOption("Toolkit")
			.setTarget("network-toolkit"));

		core->addOption(Framework::Options::SubmenuOption("Spoofing")
			.setTarget("network-spoofing"));
		});

	Framework::addSubmenu("Toolkit", "network-toolkit", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Script Migration")
			.setTarget("toolkit-script-migration"));
	});


	Framework::addSubmenu("Toolkit", "toolkit-script-migration", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Prevent Stealing")
			.addToggle(&Menu::Network::Vars::m_PreventStealing));

		core->addOption(Framework::Options::ButtonOption("Force Script Host")
			.addClick([] { Menu::Network::ForceScriptHost(Joaat("freemode")); }));
	});
}


void NetworkMenu::Update() {
	Run();
	GetNetworkPlayersMenu()->Update();
	GetNetworkTunablesMenu()->Update();
	GetNetworkSpoofingMenu()->Update();
	Menu::Network::PreventStealing();
}