#include "stdafx.hpp"
#include "network_players.hpp"
#include "cheat/util/player_manager.hpp"
#include "cheat/util/timer.hpp"
#include "players/network_players_selected.hpp"
using namespace NetworkPlayersMenuVars;

namespace NetworkPlayersMenuVars {
	Vars_ m_Vars;

	std::string CalculateFlags(Menu::PlayerVars& Player) {
		std::string Return = Player.m_Name;

		bool NetGameIsValid = true;// = ((uint64_t)Player.m_NetGamePlayer & 0xFFFFFFFF) != 0;

		if (Player.m_UsingGhost) {
			Return += " [GHOST]";
		}

		if (Player.m_Spectate) {
			Return += " ~HUD_COLOUR_GREYLIGHT~[SPEC]";
		}

		if (Player.m_IsFriend) {
			Return += " ~HUD_COLOUR_BLUE~[F]";
		}

		if (NetGameIsValid && Player.m_NetGamePlayer->is_host()) {
			Return += " ~HUD_COLOUR_BLUELIGHT~[H]";
		}

		if (Player.m_IsModder) {
			Return += " ~g~[M]";
		}

		if (Player.m_IsGodmode) {
			Return += " ~y~[G]";
		}

		if (Player.m_IsScriptHost) {
			Return += " ~o~[SC-H]";
		}

		if (Player.m_ID > 31) {
			Return += " ~r~[SCTV]";
		}

		if (Player.m_ID == Menu::GetLocalPlayer().m_ID) {
			Return += " ~HUD_COLOUR_PINK~[ME]";
		}

		if (NetGameIsValid && Player.m_NetGamePlayer->m_is_rockstar_dev) {
			Return += " ~c~[R*]";
		}

		if (NetGameIsValid && Player.m_NetGamePlayer->m_is_rockstar_qa) {
			Return += " ~c~[R*QA]";
		}

		return Return;
	}

	std::string CalculateName(Menu::PlayerVars& Player) {
		std::string Return = Player.m_Name;
		

		return Return;
	}
}

void NetworkPlayersMenu::Run() {

	Framework::addSubmenu("Players", "network_players", [=](Framework::Options::Core* core) {
		g_EnablePlayerInfo = false;

		static int Timer = 0;
		Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
			if (!Player.IsValidName()) return;
			core->addOption(Framework::Options::SubmenuOption(CalculateFlags(Player).c_str())
				.addClick([&] {
					Menu::GetSelectedPlayer().Reset();
					m_Vars.m_SelectedPlayer = Player.m_ID; 
					}).setTarget("network_selected_player"));
		}, true);

	
	});

}


void NetworkPlayersMenu::Update() {
	Run();
	if (Native::NetworkIsSessionStarted()) {
		GetNetworkPlayersSelectedMenu()->Update();
	}

}