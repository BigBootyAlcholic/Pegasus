#include "stdafx.hpp"
#include "player_manager.hpp"
#include "rage/invoker/natives.hpp"
#include "cheat/util/notify.hpp"
#include "util/caller.hpp"
#include "cheat/menu/submenus/network/network_players.hpp"
#include "rage/engine.hpp"
#include "cheat/menu/submenus/network/network_player_history.hpp"
namespace Menu {
	std::string T_IsModdingInvalidModel("is modding: Invalid Model");
	std::string T_IsModdingSpoofedName("is modding: Spoofed Name");
	std::string T_IsModdingSpoofedPeer("is modding: Spoofed Peer");
	std::string T_IsModdingSpoofedRID("is modding: Spoofed RID");
	std::string T_IsSpoofingTheirIPTo("is spoofing their ip to");
	std::string T_TheirRealIPIs("Their real IP is");
	std::string T_ExplosiveAmmo("Explosive Ammo");
	std::string T_ExplosiveMelee("Explosive Melee");
	std::string T_FireAmmo("Fire Ammo");
	std::string T_SuperJump("Super Jump");
	std::string T_IsModding("is modding");
	std::string T_CRCRequestFlood("CRC Request Flood");
	std::string T_CRCNotReplied("CRC Not Replied");
	std::string T_CRCCompromised("CRC Compromised");
	std::string T_InfoChange1("Info Change 1");
	std::string T_InfoChange4("Info Change 4");
	std::string T_InfoChange5("Info Change 5");
	std::string T_InfoChange6("Info Change 6");
	std::string T_InfoChange7("Info Change 7");
	std::string T_GotReported("got reported");
	std::string T_GotReportedInfoChange2("got reported: Info Change 2");
	std::string T_GotReportedArxan("got reported: Arxan");

	void PlayerManager::Update() {
		for (int i = 0; i < 34; i++) {
			auto& Player = m_Players[i];

			if (Native::GetPlayerName(i) == nullptr) {
				Player.m_Connected = false;
				continue;
			}

			Player.m_Connected = strcmp(Native::GetPlayerName(i), "**Invalid**");

			Player.m_ID = i;
			Player.m_Ped = Native::GetPlayerPedScriptIndex(i);
			Player.m_Exists = Native::DoesEntityExist(Player.m_Ped);

			Player.m_IsScriptHost = Native::NetworkGetHostOfScript("freemode", -1, 0) == Player.m_ID;

			Player.m_Name = Native::GetPlayerName(i);
			//Native::NetworkHandleFromPlayer(i, &Player.m_NetworkHandle.m_Handle, (sizeof(Player.m_NetworkHandle) / 4));

			//if (Native::NetworkIsHandleValid(&Player.m_NetworkHandle.m_Handle, (sizeof(Player.m_NetworkHandle) / 4))) {
			//	Player.m_IsFriend = Native::NetworkIsFriend(&Player.m_NetworkHandle.m_Handle);
			//}

			if (!Player.m_Exists) {
				continue;
			}

			Player.m_NetGamePlayer = Engine::GetNetworkGamePlayer(Player.m_ID);
			Player.m_PedPointer = (CPed*)Engine::GetEntityAddress(Player.m_Ped);

			Player.m_Health = Native::GetEntityHealth(Player.m_Ped);
			Player.m_Alive = !Native::IsPedDeadOrDying(Player.m_Ped, true);
			Player.m_InVehicle = Native::IsPedInAnyVehicle(Player.m_Ped, true);
			Player.m_Entity = Player.m_InVehicle ? Native::GetVehiclePedIsIn(Player.m_Ped, false) : Player.m_Ped;
			if (!Player.m_Entity) Player.m_Entity = Player.m_Ped;

			Player.m_Vehicle = Native::GetVehiclePedIsIn(Player.m_Ped, false);
			Player.m_WeaponEntity = Native::GetCurrentPedWeaponEntityIndex(Player.m_Ped);
			Player.m_Coords = Native::GetEntityCoords(Player.m_Entity, false);
			Player.m_IsShooting = Native::IsPedShooting(Player.m_Ped);
			Player.m_Heading = Native::GetEntityHeading(Player.m_Ped);
			Player.m_HasWeaponInHand = (Player.m_WeaponHash && Player.m_WeaponHash != 2725352035);
			Player.m_Rotation = Native::GetEntityRotation(Player.m_Entity, 2);
			Player.m_Model = Native::GetEntityModel(Player.m_Ped);
			Player.m_IsInside = Native::GetInteriorAtCoords(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z) > 0;

			Native::GetCurrentPedWeapon(Player.m_Ped, &Player.m_WeaponHash, false);
			if (Player.m_InVehicle) Native::GetCurrentPedVehicleWeapon(Player.m_Ped, &Player.m_VehicleWeaponHash);

			//NetworkRecentPlayersMenuVars::Save(Player.m_Name, Player.m_NetGamePlayer->m_player_info->m_net_player_data.m_gamer_handle.m_rockstar_id);

			if (Player.m_Connected) {
				if (i != Native::PlayerId()) {
					if (Player.m_Model != 0x0 && Player.m_Model != 0x9c9effd8 && Player.m_Model != 0x705e61f2) {
						if (!Player.m_ModderFlags[Menu::REASON_INVALID_MODEL]) {
							Menu::Notify::stacked(std::format("{} Is Modding: Model Change", Player.m_Name).c_str());
						}

						Player.m_ModderFlags[Menu::REASON_INVALID_MODEL] = true;
						Player.m_IsModder = true;
					}

					if (IsValidPtr(Player.m_NetGamePlayer)) {
						if (Player.m_NetGamePlayer->m_is_cheater) {
							Player.m_ModderFlags[Menu::REASON_NET_GAME_PLAYER_CHEATER] = true;
							Player.m_IsModder = true;
						}

						if (IsValidPtr(Player.m_NetGamePlayer->m_player_info)) {
							if (((Player.m_NetGamePlayer->m_player_info->m_net_player_data.m_host_token >> 32) & 0xFFFFFFFF) < 0x1000 || Player.m_NetGamePlayer->m_player_info->m_net_player_data.m_host_token < 0x1000) {
								if (!Player.m_ModderFlags[Menu::REASON_SPOOFED_PEER]) {
									Menu::Notify::stacked(std::format("{} Is Modding: Spoofed Peer", Player.m_Name).c_str());
								}

								Player.m_ModderFlags[Menu::REASON_SPOOFED_PEER] = true;
								Player.m_IsModder = true;
							}



							// Frame flag checks
							std::pair<int, std::pair<Menu::ePlayerReportReasons, std::string>> FrameFlags[] = {
								{ 0x800, { Menu::REASON_EXPLOSIVE_AMMO, "Explosive Ammo"}},
								{ 0x2000, { Menu::REASON_EXPLOSIVE_MELEE, "Explosive Melee"}},
								{ 0x1000, { Menu::REASON_FIRE_AMMO, "Fire Ammo"}},
								{ 0x4000, { Menu::REASON_SUPER_JUMP, "Super Jump"}}
							};

							for (auto& Flag : FrameFlags) {
								if ((Player.m_NetGamePlayer->m_player_info->m_frame_flags & Flag.first) != 0) {
									if (!Player.m_ModderFlags[Flag.second.first]) {
										Menu::Notify::stacked(std::format("{} Is Modding: {}", Player.m_Name, Flag.second.second.c_str()).c_str());
									}

									Player.m_ModderFlags[Flag.second.first] = true;
									Player.m_IsModder = true;
								}
							}

							if (Player.m_NetGamePlayer->m_player_info->m_net_player_data.m_gamer_handle.m_rockstar_id < 10000) {
								if (!Player.m_ModderFlags[Menu::REASON_SPOOFED_RID]) {
									Menu::Notify::stacked(std::format("{} Is Modding: Spoofed Rid", Player.m_Name).c_str());
								}

								Player.m_ModderFlags[Menu::REASON_SPOOFED_RID] = true;
								Player.m_IsModder = true;
							}
						}
					}

				}
			}
		}
	}

	void PlayerManager::Foreach(std::function<void(PlayerVars&)> Callback, bool BypassExcludes, bool BypassGhost) {
		for (int i = 0; i < 34; i++) {
			if (!BypassGhost) {
				if (!m_Players[i].m_Exists) continue;
			}

			if (m_Players[i].m_Connected) {
				if (!BypassExcludes) {
					// handle excludes
					if (i == GetLocalPlayer().m_ID) continue;
				}

				Callback(m_Players[i]);
			}
		}
	}

	PlayerVars& PlayerManager::GetPlayer(int Index) {
		return m_Players[Index];
	}

	PlayerVars& PlayerManager::GetLocalPlayer() {
		return m_Players[Native::PlayerId()];
	}

	PlayerManager* GetPlayerManager() {
		static PlayerManager Instance;
		return &Instance;
	}

	PlayerVars& GetPlayer(int Index) {
		return GetPlayerManager()->GetPlayer(Index);
	}

	PlayerVars& GetLocalPlayer() {
		return GetPlayerManager()->GetLocalPlayer();
	}

	PlayerVars& GetSelectedPlayer() {
		return GetPlayerManager()->GetPlayer(NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer);
	}
}