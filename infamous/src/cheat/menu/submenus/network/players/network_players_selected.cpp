#include "stdafx.hpp"
#include "network_players_selected.hpp"
#include "cheat/util/player_manager.hpp"
#include "cheat/util/timer.hpp"
#include "../network_players.hpp"
#include "util/fiber.hpp"
#include "util/fiber_pool.hpp"
#include "util/caller.hpp"
#include "core/patterns.hpp"
#include "rage/engine.hpp"
#include "rage/classes/network/CNetGamePlayer.hpp"
#include "cheat/util/packet.hpp"
#include "rage/classes/network/Network.hpp"
using namespace NetworkPlayersSelectedMenuVars;

namespace NetworkPlayersSelectedMenuVars {
	Vars_ m_Vars;

	void TriggerScriptEvent(int event_group, int64_t* args, int arg_count, int player_bits, int event_id) {
		Caller::Call<void>(Patterns::Vars::g_TriggerScriptEvent, event_group, args, arg_count, player_bits, event_id);
	}

	void TeleportToPlayer() {
		Entity handle;
		Math::Vector3<float> coords = Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, false);
		Native::IsPedInAnyVehicle(Menu::GetLocalPlayer().m_Ped, false) ? handle = Native::GetVehiclePedIsUsing(Menu::GetLocalPlayer().m_Ped) : handle = Menu::GetLocalPlayer().m_Ped;
		Native::SetEntityCoords(handle, coords.m_X, coords.m_Y, coords.m_Z, false, false, false, false);
		if (Native::IsPedInAnyVehicle(Menu::GetSelectedPlayer().m_Ped, FALSE)) {
			Vehicle veh = Native::GetVehiclePedIsIn(Menu::GetSelectedPlayer().m_Ped, false);
			for (int i = -1; i < 16; i++) {
				if (Native::IsVehicleSeatFree(veh, i, 0)) {
					Native::SetPedIntoVehicle(Menu::GetLocalPlayer().m_Ped, veh, i);
					return;
				}
			}
		}
	}

	void Crash() {
		int64_t args1[] = { (int64_t)eRemoteEvent::Crash, (int64_t)Menu::GetLocalPlayer().m_ID };
		TriggerScriptEvent(1, args1, sizeof(args1) / sizeof(args1[0]), 1 << Menu::GetSelectedPlayer().m_ID, (int)eRemoteEvent::Crash);

		int64_t args2[] = { (int64_t)eRemoteEvent::Crash2, (int64_t)Menu::GetLocalPlayer().m_ID, rand() % INT64_MAX };
		TriggerScriptEvent(1, args2, sizeof(args2) / sizeof(args2[0]), 1 << Menu::GetSelectedPlayer().m_ID, (int)eRemoteEvent::Crash2);

		int64_t args3[] = { (int64_t)eRemoteEvent::Crash3, (int64_t)Menu::GetLocalPlayer().m_ID, rand() % INT64_MAX, (int64_t)NAN, (int64_t)NAN }; // No idea if it works or not.
		TriggerScriptEvent(1, args3, sizeof(args3) / sizeof(args3[0]), 1 << Menu::GetSelectedPlayer().m_ID, (int)eRemoteEvent::Crash3);

		int64_t args4[] = { (int64_t)eRemoteEvent::MCTeleport, (int64_t)Menu::GetLocalPlayer().m_ID, rand() % INT64_MAX, rand() % INT64_MAX };
		TriggerScriptEvent(1, args4, sizeof(args4) / sizeof(args3[0]), 1 << Menu::GetSelectedPlayer().m_ID, (int)eRemoteEvent::MCTeleport);
	}
}

void NetworkPlayersSelectedMenu::Run() {

	if (!Menu::GetSelectedPlayer().IsValidName()) return;
	const char* name = Menu::GetSelectedPlayer().m_Name;

	bool flag = false;
	if (!flag) {

		flag = true;
	}
	
	if (Menu::GetSelectedPlayer().IsValidName()) {
		Framework::addPlayerSubmenu(&NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer, "network_selected_player", [=](Framework::Options::PCore* core) {
			
			g_EnablePlayerInfo = true;

			core->addOption(Framework::Options::SubmenuOption("Removals")
				.setTarget("network_selected_player_removals"));

			core->addOption(Framework::Options::ButtonOption("Teleport To Player")
				.addClick([] { TeleportToPlayer(); }));

			core->addOption(Framework::Options::ButtonOption("View Profile")
				.addClick([] {
					CNetGamePlayer* NetPlayer = Engine::GetNetworkGamePlayer(NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer);
					auto rid = NetPlayer->GetGamerInfo()->m_gamer_handle_2.m_rockstar_id;
					if (rid > 0) {
						int netHandle[13];
						Native::NetworkHandleFromMemberId(std::to_string(rid).c_str(), netHandle, 13);
						Native::NetworkShowProfileUi(netHandle);
					}
				}));

			core->addOption(Framework::Options::ToggleOption("Spectate")
				.addToggle(&Menu::GetSelectedPlayer().m_Spectate)
				.addClick([] {
					if (Menu::GetSelectedPlayer().m_Spectate) {
						if (Native::NetworkIsInSpectatorMode()) {
							Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
								Native::NetworkSetInSpectatorMode(false, Player.m_Ped);
								});
						}

						Native::NetworkSetInSpectatorMode(true, Menu::GetSelectedPlayer().m_Ped);
					}
					else {
						Native::NetworkSetInSpectatorMode(false, Menu::GetSelectedPlayer().m_Ped);
					}
					}));
			});

		Framework::addPlayerSubmenu(&NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer, "network_selected_player_removals", [=](Framework::Options::PCore* core) {
			core->addOption(Framework::Options::ButtonOption("Desync Kick")
				.addClick([] {

					(*Patterns::Vars::g_NetworkPlayerMgr)->RemovePlayer(Menu::GetSelectedPlayer().m_NetGamePlayer);
					Native::RemoveAllPedWeapons(Menu::GetSelectedPlayer().m_Ped, true);
				/*	Menu::Packet::Packet msg;
					msg.WriteMessage(Rage::eNetMessage::MsgRemoveGamersFromSessionCmd);
					msg.Write<uint64_t>((*Patterns::Vars::g_Network)->m_game_session_ptr->m_rline_session.m_session_id, 0x40);
					auto netConnectionMgr = ((*Patterns::Vars::g_Network)->m_game_session_ptr->m_net_connection_mgr;
					auto netConnectionPeer = Caller::Call<bool>(Patterns::Vars::g_GetConnectionPeer, netConnectionMgr, m_request.m_connection_identifier);
					rage::snMsgRemoveGamersFromSessionCmd cmd{ util::player::g_handle->m_gamer_info.m_gamer_handle.m_rockstar_id, { netConnectionPeer->m_peer_id }, 1 };
					msg.send(uint32_t(m_request.m_identifier));*/
			}));

			core->addOption(Framework::Options::ButtonOption("Crash")
				.addClick([] {
					Crash();
				}));

			/*core->addOption(Framework::Options::ButtonOption("Ghetto Desync Kick")
				.addClick([] {
					auto rlGamerInfo = Menu::GetSelectedPlayer().m_NetGamePlayer->GetGamerInfo();
					auto complaintMgr = (*Patterns::Vars::g_Network)->m_game_complaint_mgr;
					complaintMgr.raise_complaint(rlGamerInfo->m_host_token, true, true);
				}));*/
		});
	}

}


void NetworkPlayersSelectedMenu::Update() {
	Run();
	Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
		if (Player.m_Spectate) {
			if (!Menu::GetLocalPlayer().m_Alive) {
				Native::NetworkSetInSpectatorMode(false, Player.m_Ped);
			}
		}
	});
}