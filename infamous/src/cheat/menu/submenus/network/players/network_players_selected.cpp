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
#include "cheat/util/control_manager.hpp"
#include "network_players_selected_griefing.hpp"
using namespace NetworkPlayersSelectedMenuVars;

namespace NetworkPlayersSelectedMenuVars {
	Vars_ m_Vars;



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

	inline bool network_has_control_of_entity(Rage::netObject* net_object) {
		return !net_object || !net_object->m_next_owner_id && (net_object->m_control_id == -1);
	}

	inline bool RequestControlOfEntity(Entity ent, int timeout = 300) {
		if (!Native::NetworkIsSessionStarted())
			return true;

		auto hnd = Caller::Call<Rage::CDynamicEntity*>(Patterns::Vars::g_HandleToPtr, ent);

		if (!hnd || !hnd->m_net_object || !Native::NetworkIsSessionStarted())
			return false;

		if (network_has_control_of_entity(hnd->m_net_object))
			return true;

		for (int i = 0; i < timeout; i++)
		{
			Caller::Call<void >(Patterns::Vars::g_RequestControl, hnd->m_net_object);

			if (network_has_control_of_entity(hnd->m_net_object))
				return true;

			if (timeout != 0)
				Utils::GetFiberManager()->GoToMainFiber();
		}

		return false;
	}

	void tp_to_me() {
		Entity ent = Menu::GetSelectedPlayer().m_Ped;
		if (Native::IsEntityDead(ent, true)) {
			return;
		}

		if (!Native::IsPedInAnyVehicle(ent, true)) {
			return;
		}
		ent = Native::GetVehiclePedIsIn(ent, false);
		Math::Vector3<float> location = Native::GetEntityCoords(Native::PlayerPedId(), 0);

		if (RequestControlOfEntity(ent))
			Native::SetEntityCoords(ent, location.m_X, location.m_Y, location.m_Z, 0, 0, 0, 0);
	}

	bool LoadGround3D(Math::Vector3<float>& location)
	{
		float GroundZ;
		const std::uint8_t Attempts = 10;

		for (std::uint8_t i = 0; i < Attempts; i++)
		{
			// Only request a collision after the first try failed because the location might already be loaded on first attempt
			for (std::uint16_t z = 0; i && z < 1000; z += 100)
			{
				Native::RequestCollisionAtCoord(location.m_X, location.m_Y, (float)z);

				Utils::GetFiberManager()->GoToMainFiber();
			}

			if (Native::GetGroundZFor3DCoord(location.m_X, location.m_Y, 1000.f, &GroundZ, FALSE))
			{
				location.m_Z = GroundZ + 1.f;

				return true;
			}

			Utils::GetFiberManager()->GoToMainFiber();
		}

		location.m_Z = 1000.f;
	}



	void tp_into_veh() {
		Entity ent = Menu::GetSelectedPlayer().m_Ped;
		Vehicle veh = Native::GetVehiclePedIsIn(ent, true);
		if (!Native::IsEntityAVehicle(veh)) {
			return;
		}
		int seat_index = 255;
		if (Native::IsVehicleSeatFree(veh, -1, true))
			seat_index = -1;
		else if (Native::IsVehicleSeatFree(veh, -2, true))
			seat_index = -2;
		if (seat_index == 255) {
			return;
		}
		Math::Vector3<float> location = Native::GetEntityCoords(veh, true);
		LoadGround3D(location);
		Ped ped = Native::PlayerPedId();
		Native::SetEntityCoords(ped, location.m_X, location.m_Y, location.m_Z, 0, 0, 0, 0);
		Utils::GetFiberManager()->GoToMainFiber();
		Native::SetPedIntoVehicle(ped, veh, seat_index);
	}



	void tp_to_apt(Player target, int index) {
		const size_t arg_count = 9;
		int64_t args[arg_count] = {
			(int64_t)eRemoteEvent::Teleport,
			Native::PlayerId(),
			(int64_t)target,
			(int64_t)(int)-1, 1, (int64_t)index, 1, 1, 1
		};

		Engine::TriggerScriptEvent(1, args, arg_count, 1 << target);
	}

	constexpr const static auto apartment_names = std::to_array({
	"INVALID",
	"Eclipse Towers, Apt 31",
	"Eclipse Towers, Apt 9",
	"Eclipse Towers, Apt 40",
	"Eclipse Towers, Apt 5",
	"3 Alta St, Apt 10",
	"3 Alta St, Apt 57",
	"Del Perro Heights, Apt 20",
	"1162 Power St, Apt 3",
	"0605 Spanish Ave, Apt 1",
	"0604 Las Lagunas Blvd, Apt 4",
	"0184 Milton Rd, Apt 13",
	"The Royale, Apt 19",
	"0504 S Mo Milton Dr",
	"0115 Bay City Ave, Apt 45",
	"0325 South Rockford Dr",
	"Dream Tower, Apt 15",
	"2143 Las Lagunas Blvd, Apt 9",
	"1561 San Vitas St, Apt 2",
	"0112 S Rockford Dr, Apt 13",
	"2057 Vespucci Blvd, Apt 1",
	"0069 Cougar Ave, Apt 19",
	"1237 Prosperity St, Apt 21",
	"1115 Blvd Del Perro, Apt 18",
	"0120 Murrieta Heights",
	"Unit 14 Popular St",
	"Unit 2 Popular St",
	"331 Supply St",
	"Unit 1 Olympic Fwy",
	"0754 Roy Lowenstein Blvd",
	"12 Little Bighorn Ave",
	"Unit 124 Popular St",
	"0552 Roy Lowenstein Blvd",
	"0432 Davis Ave",
	"Del Perro Heights, Apt 7",
	"Weazel Plaza, Apt 101",
	"Weazel Plaza, Apt 70",
	"Weazel Plaza, Apt 26",
	"4 Integrity Way, Apt 30",
	"4 Integrity Way, Apt 35",
	"Richards Majestic, Apt 4",
	"Richards Majestic, Apt 51",
	"Tinsel Towers, Apt 45",
	"Tinsel Towers, Apt 29",
	"142 Paleto Blvd",
	"1 Strawberry Ave",
	"1932 Grapeseed Ave",
	"1920 Senora Way",
	"2000 Great Ocean Highway",
	"197 Route 68",
	"870 Route 68 Approach",
	"1200 Route 68",
	"8754 Route 68",
	"1905 Davis Ave",
	"1623 South Shambles St",
	"4531 Dry Dock St",
	"1337 Exceptionalists Way",
	"Unit 76 Greenwich Parkway",
	"Garage Innocence Blvd",
	"634 Blvd Del Perro",
	"0897 Mirror Park Blvd",
	"Eclipse Towers, Apt 3",
	"Del Perro Heights, Apt 4",
	"Richards Majestic, Apt 2",
	"Tinsel Towers, Apt 42",
	"4 Integrity Way, Apt 28",
	"4 Hangman Ave",
	"12 Sustancia Rd",
	"4584 Procopio Dr",
	"4401 Procopio Dr",
	"0232 Paleto Blvd",
	"140 Zancudo Ave",
	"1893 Grapeseed Ave",
	"3655 Wild Oats Drive",
	"2044 North Conker Avenue",
	"2868 Hillcrest Avenue",
	"2862 Hillcrest Avenue",
	"3677 Whispymound Drive",
	"2117 Milton Road",
	"2866 Hillcrest Avenue",
	"2874 Hillcrest Avenue",
	"2113 Mad Wayne Thunder Drive",
	"2045 North Conker Avenue",
	"Eclipse Towers, Penthouse Suite 1",
	"Eclipse Towers, Penthouse Suite 2",
	"Eclipse Towers, Penthouse Suite 3",
	"Private Yacht",
	"Lombank West",
	"Maze Bank West",
	"Arcadius Business Center",
	"Maze Bank Tower",
	"Rancho Clubhouse",
	"Del Perro Beach Clubhouse",
	"Pillbox Hill Clubhouse",
	"Great Chaparral Clubhouse",
	"Paleto Bay Clubhouse",
	"Sandy Shores Clubhouse",
	"La Mesa Clubhouse",
	"Downtown Vinewood Clubhouse",
	"Hawick Clubhouse",
	"Grapeseed Clubhouse",
	"Paleto Bay Clubhouse",
	"Vespucci Beach Clubhouse",
	"Office Garage 1",
	"Office Garage 2",
	"Office Garage 3",
	"Office Garage 1",
	"Office Garage 2",
	"Office Garage 3",
	"Office Garage 1",
	"Office Garage 2",
	"Office Garage 3",
	"Office Garage 1",
	"Office Garage 2",
	"Office Garage 3",
	"Vehicle Warehouse"
		});


	void Crash() {
		int64_t args1[] = { (int64_t)eRemoteEvent::Crash, (int64_t)Menu::GetLocalPlayer().m_ID };
		Engine::TriggerScriptEvent(1, args1, sizeof(args1) / sizeof(args1[0]), 1 << Menu::GetSelectedPlayer().m_ID);

		int64_t args2[] = { (int64_t)eRemoteEvent::Crash2, (int64_t)Menu::GetLocalPlayer().m_ID, rand() % INT64_MAX };
		Engine::TriggerScriptEvent(1, args2, sizeof(args2) / sizeof(args2[0]), 1 << Menu::GetSelectedPlayer().m_ID);

		int64_t args3[] = { (int64_t)eRemoteEvent::Crash3, (int64_t)Menu::GetLocalPlayer().m_ID, rand() % INT64_MAX, (int64_t)NAN, (int64_t)NAN }; // No idea if it works or not.
		Engine::TriggerScriptEvent(1, args3, sizeof(args3) / sizeof(args3[0]), 1 << Menu::GetSelectedPlayer().m_ID);

		int64_t args4[] = { (int64_t)eRemoteEvent::MCTeleport, (int64_t)Menu::GetLocalPlayer().m_ID, rand() % INT64_MAX, rand() % INT64_MAX };
		Engine::TriggerScriptEvent(1, args4, sizeof(args4) / sizeof(args3[0]), 1 << Menu::GetSelectedPlayer().m_ID);
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

			core->addOption(Framework::Options::SubmenuOption("Teleport")
				.setTarget("network_selected_player_teleport"));

			core->addOption(Framework::Options::SubmenuOption("Griefing")
				.setTarget("network_selected_player_griefing"));

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

		Framework::addPlayerSubmenu(&NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer, "network_selected_player_teleport", [=](Framework::Options::PCore* core) {
			core->addOption(Framework::Options::SubmenuOption("Locations")
				.setTarget("network_selected_player_teleport_locations"));

			core->addOption(Framework::Options::ButtonOption("To Player")
				.addClick([] { TeleportToPlayer(); }));

			core->addOption(Framework::Options::ButtonOption("Into Vehicle")
				.addClick([] { tp_into_veh(); }));

			core->addOption(Framework::Options::ButtonOption("To Me")
				.addClick([] { tp_to_me(); }));
		});

		Framework::addPlayerSubmenu(&NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer, "network_selected_player_teleport_locations", [=](Framework::Options::PCore* core) {
			static int idx;
			for (int i = 1; i < apartment_names.size(); i++) {
				core->addOption(Framework::Options::ButtonOption(apartment_names[i])
					.addClick([=] { idx = i;  tp_to_apt(Menu::GetSelectedPlayer().m_ID, idx); }));
			}
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

	GetNetworkPlayersSelectedGriefingMenu()->Update();
}