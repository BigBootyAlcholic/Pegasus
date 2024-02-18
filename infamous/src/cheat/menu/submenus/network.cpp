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
using namespace NetworkMenuVars;
namespace NetworkMenuVars {
	Vars_ m_Vars;
	void RemovePlayer(uint64_t RockstarID) {
		if (::Patterns::Vars::g_NetworkBaseConfig) {
			Rage::Network::NetworkBaseConfig* RealConfig = (*(Rage::Network::NetworkBaseConfig**)((uint64_t)::Patterns::Vars::g_NetworkBaseConfig));

			/*		if (Global::Vars::g_Steam) {
						RealConfig = (Rage::Network::NetworkBaseConfig*)(*(uint64_t*)((uint64_t)::Global::Vars::g_NetworkBaseConfig) - 0x50);
					}*/

			if (RealConfig) {
				if (RealConfig->m_LastBlacklisted.m_RockstarID == RockstarID) {
					RealConfig->m_LastBlacklisted.m_RockstarID = 0;
					RealConfig->m_LastBlacklistedTimestamp = 0;
				}

				// If they're the only one in the list
				if (RealConfig->m_FirstBlacklistedPlayer) {
					if (RealConfig->m_FirstBlacklistedPlayer->m_Player.m_RockstarID == RockstarID) {
						if (RealConfig->m_FirstBlacklistedPlayer->m_Next == nullptr) {
							RealConfig->m_FirstBlacklistedPlayer = nullptr;
							RealConfig->m_LastBlacklistedPlayer = nullptr;
						}
						else {
							// Replace it with the next
							RealConfig->m_FirstBlacklistedPlayer = RealConfig->m_FirstBlacklistedPlayer->m_Next;
							RealConfig->m_FirstBlacklistedPlayer->m_Last = nullptr;
						}

						return;
					}
				}

				// If they're not the first
				Rage::Network::BlacklistedPlayer* CurrentPlayer = RealConfig->m_FirstBlacklistedPlayer;
				Rage::Network::BlacklistedPlayer* LastPlayer = nullptr;

				while (CurrentPlayer) {
					if (CurrentPlayer->m_Player.m_RockstarID == RockstarID) {
						if (!IsValidPtr(CurrentPlayer->m_Next)) {
							LastPlayer->m_Next = nullptr;
							return;
						}

						LastPlayer->m_Next = CurrentPlayer->m_Next;
						CurrentPlayer->m_Last = LastPlayer;
						return;
					}

					LastPlayer = CurrentPlayer;
					CurrentPlayer = CurrentPlayer->m_Next;
				}
			}
		}
	}

	Network* GetNetworkBaseConfig() {
		return *Patterns::Vars::g_Network;
	}
}


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

	/*	core->addOption(Framework::Options::ButtonOption("Rejoin Last Session")
			.addClick([] { 
				Engine::JoinSessionByInfo(GetNetworkBaseConfig(), &GetNetworkBaseConfig()->m_last_joined_session.m_session_info, 1, 1 | 2 | 4, nullptr, 0);
			}));*/

		/*core->addOption(Framework::Options::ButtonOption("test")
			.addClick([] {
				Utils::GetFiberPool()->Push([] {
					if (Native::_GetCurrentFrontendMenu() != 0xFFFFFFFF) {
						Native::ActivateFrontendMenu(Joaat("FE_MENU_VERSION_SP_PAUSE"), false, 2);
						Utils::WaitFiber(200);
					}//
					Native::ActivateFrontendMenu(Joaat("FE_MENU_VERSION_SP_PAUSE"), false, 2);
					Utils::WaitFiber(200);
					CPlayerListMenu* Menu = new CPlayerListMenu();
					uint32_t Hash{ 0xDA4858C1 };
					uint64_t Info{ Caller::Call<u64>(Patterns::Vars::g_GetFriendsMenu, 0) };
					uint8_t* Data{ reinterpret_cast<uint8_t*>(Info + 0x8) };
					if (Data) {
						uint8_t Idx{};
						while (*Data <= 3u) {
							if (*Data == 3) {
								break;
							}
							++Idx;
							Data += 0x10;
						}
						if (Idx < 20ui8) {
							uint64_t OriginalRID{ *(uint64_t*)(Info + 16ui64 * Idx) };
							*(uint64_t*)(Info + 16ui64 * Idx) = 128211328;
							Caller::Call<bool>(Patterns::Vars::g_ConstructPlayerMenu, Menu, &Hash);
							Utils::WaitFiber(400);
							*(uint64_t*)(Info + 16ui64 * Idx) = OriginalRID;
						}
					}
				});

			}));*/
	});

	Framework::addSubmenu("Toolkit", "network-toolkit", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Script Migration")
			.setTarget("toolkit-script-migration"));

		core->addOption(Framework::Options::SubmenuOption("Kicked Players")
			.setTarget("toolkit-kicked-players"));
	});

	Framework::addSubmenu("Kicked Players", "toolkit-kicked-players", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ButtonOption("Remove All")
			.addClick([] { 
				if (::Patterns::Vars::g_NetworkBaseConfig) {
					Rage::Network::NetworkBaseConfig* RealConfig = (*(Rage::Network::NetworkBaseConfig**)((uint64_t)::Patterns::Vars::g_NetworkBaseConfig));
					/*if (Global::Vars::g_Steam) {
						RealConfig = (Rage::Network::NetworkBaseConfig*)(*(uint64_t*)((uint64_t)::Global::Vars::g_NetworkBaseConfig) - 0x50);
					}*/

					if (RealConfig) {
						RealConfig->m_LastBlacklisted.m_RockstarID = 0;
						RealConfig->m_LastBlacklistedTimestamp = 0;
						RealConfig->m_FirstBlacklistedPlayer = nullptr;
						RealConfig->m_LastBlacklistedPlayer = nullptr;
					}
				}

				m_Vars.m_Players.clear();
			}));

		core->addOption(Framework::Options::BreakOption("Players"));

		//static int Timer = 0;
		//Menu::Timers::RunTimedFunction(&Timer, 1000, [=] {
			std::vector<Rage::Network::BlacklistedPlayer> Players;	

			if (::Patterns::Vars::g_NetworkBaseConfig) {
				Rage::Network::NetworkBaseConfig* RealConfig = (*(Rage::Network::NetworkBaseConfig**)((uint64_t)::Patterns::Vars::g_NetworkBaseConfig));
				/*if (Global::Vars::g_Steam) {
					RealConfig = (Rage::Network::NetworkBaseConfig*)(*(uint64_t*)((uint64_t)::Global::Vars::g_NetworkBaseConfig) - 0x50);
				}*/

				Rage::Network::BlacklistedPlayer* BlacklistedPlayers = RealConfig->m_FirstBlacklistedPlayer;
				if (IsValidPtr(BlacklistedPlayers)) {
					for (; BlacklistedPlayers; BlacklistedPlayers = BlacklistedPlayers->m_Next) {
						Players.push_back(*BlacklistedPlayers);
					}
				}
			}

			for (auto& Player : Players) {
				auto Vit = std::find_if(m_Vars.m_Players.begin(), m_Vars.m_Players.end(), [=](Blacklist& Element) { return Element.m_Player.m_Player.m_RockstarID == Player.m_Player.m_RockstarID; });
				if (Vit == m_Vars.m_Players.end()) {
					m_Vars.m_Players.push_back({ Player, false, "" });
				}
			}

			std::vector<uint64_t> Delete;
			for (auto& Player : m_Vars.m_Players) {
				auto Vit = std::find_if(Players.begin(), Players.end(), [=](Rage::Network::BlacklistedPlayer& Element) { return Element.m_Player.m_RockstarID == Player.m_Player.m_Player.m_RockstarID; });
				if (Vit == Players.end()) {
					Delete.push_back(Player.m_Player.m_Player.m_RockstarID); // Deleted
					continue;
				}

				if (!Player.m_API) {
					Player.m_API = true;
				}

				std::string Name = std::to_string(Player.m_Player.m_Player.m_RockstarID);
				if (!Player.m_ResolvedName.empty()) Name = Player.m_ResolvedName;

				core->addOption(Framework::Options::ButtonOption(Name.c_str())
					.addClick([=] { RemovePlayer(Player.m_Player.m_Player.m_RockstarID); }));
			}
			for (uint64_t D : Delete) {
				m_Vars.m_Players.erase(std::remove_if(m_Vars.m_Players.begin(), m_Vars.m_Players.end(), [=](Blacklist& Element) { return Element.m_Player.m_Player.m_RockstarID == D; }), m_Vars.m_Players.end());
			}
			if (m_Vars.m_Players.size() == 0) {
				core->addOption(Framework::Options::ButtonOption("~m~None"));
			}
		//});
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