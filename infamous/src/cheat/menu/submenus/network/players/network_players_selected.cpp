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
#include "cheat/util/network_manager.hpp"
#include "cheat/util/global.hpp"
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


	void GiveWeapon(Ped Ped, uint32_t WeaponHash, bool EquipNow, bool FullyUpgraded) {
		int Ammo = -1;
		Native::GetMaxAmmo(Ped, WeaponHash, &Ammo);
		Native::GiveWeaponToPed(Ped, WeaponHash, Ammo, false, EquipNow);
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

	inline constexpr auto badObjects = { RAGE_JOAAT("prop_thindesertfiller_aa"), RAGE_JOAAT("prop_dummy_01"), RAGE_JOAAT("prop_dummy_car"), RAGE_JOAAT("prop_dummy_light"), RAGE_JOAAT("prop_dummy_plane"), RAGE_JOAAT("prop_distantcar_night"), RAGE_JOAAT("prop_distantcar_day"), RAGE_JOAAT("hei_bh1_08_details4_em_night"), RAGE_JOAAT("dt1_18_sq_night_slod"), RAGE_JOAAT("ss1_12_night_slod"), RAGE_JOAAT("hash_b334b5e2_qyquzxq_collision"), RAGE_JOAAT("h4_prop_bush_bgnvla_med_01"), RAGE_JOAAT("h4_prop_bush_bgnvla_lrg_01"), RAGE_JOAAT("h4_prop_bush_buddleia_low_01"), RAGE_JOAAT("h4_prop_bush_ear_aa"), RAGE_JOAAT("h4_prop_bush_ear_ab"), RAGE_JOAAT("h4_prop_bush_fern_low_01"), RAGE_JOAAT("h4_prop_bush_fern_tall_cc"), RAGE_JOAAT("h4_prop_bush_mang_ad"), RAGE_JOAAT("h4_prop_bush_mang_low_aa"), RAGE_JOAAT("h4_prop_bush_mang_low_ab"), RAGE_JOAAT("h4_prop_bush_seagrape_low_01"), RAGE_JOAAT("prop_h4_ground_cover"), RAGE_JOAAT("h4_prop_weed_groundcover_01"), RAGE_JOAAT("h4_prop_grass_med_01"), RAGE_JOAAT("h4_prop_grass_tropical_lush_01"), RAGE_JOAAT("h4_prop_grass_wiregrass_01"), RAGE_JOAAT("h4_prop_weed_01_plant"), RAGE_JOAAT("h4_prop_weed_01_row"), RAGE_JOAAT("urbanweeds02_l1"), RAGE_JOAAT("proc_forest_grass01"), RAGE_JOAAT("prop_small_bushyba"), RAGE_JOAAT("v_res_d_dildo_a"), RAGE_JOAAT("v_res_d_dildo_b"), RAGE_JOAAT("v_res_d_dildo_c"), RAGE_JOAAT("v_res_d_dildo_d"), RAGE_JOAAT("v_res_d_dildo_e"), RAGE_JOAAT("v_res_d_dildo_f"), RAGE_JOAAT("v_res_skateboard"), RAGE_JOAAT("prop_battery_01"), RAGE_JOAAT("prop_barbell_01"), RAGE_JOAAT("prop_barbell_02"), RAGE_JOAAT("prop_bandsaw_01"), RAGE_JOAAT("prop_bbq_3"), RAGE_JOAAT("v_med_curtainsnewcloth2"), RAGE_JOAAT("bh1_07_flagpoles"), RAGE_JOAAT("hash_058a7eb5_deihiws_collision"), RAGE_JOAAT("proc_dry_plants_01"), RAGE_JOAAT("proc_leafyplant_01"), RAGE_JOAAT("proc_grassplantmix_02"), RAGE_JOAAT("proc_dryplantsgrass_01"), RAGE_JOAAT("proc_dryplantsgrass_02"), RAGE_JOAAT("proc_dryplantsgrass_02"), RAGE_JOAAT("proc_grasses01"), RAGE_JOAAT("prop_dryweed_002_a"), RAGE_JOAAT("prop_fernba"), RAGE_JOAAT("prop_weed_001_aa"), RAGE_JOAAT("urbangrnfrnds_01"), RAGE_JOAAT("urbanweeds01"), RAGE_JOAAT("prop_dandy_b"), RAGE_JOAAT("v_proc2_temp"), RAGE_JOAAT("prop_fernbb"), RAGE_JOAAT("proc_drygrassfronds01"), RAGE_JOAAT("prop_log_ae"), RAGE_JOAAT("prop_grass_da"), RAGE_JOAAT("prop_fragtest_cnst_04") };

	void CrashPlayer(Menu::PlayerVars& Player, int ID) {
		if (Player.m_NetGamePlayer) {
			if (Player.m_NetGamePlayer->m_player_info) {
				//LOG_ERROR("Started a V%i crash against %s for 5 seconds", ID, Player.m_Name);

				m_Vars.m_TargetedCrashID = ID;
				m_Vars.m_TargetedCrashTimeout = GetTickCount() + 5000;
				m_Vars.m_TargetedCrashTarget = Player.m_NetGamePlayer->m_player_info->m_net_player_data.m_gamer_handle.m_rockstar_id;
				m_Vars.m_TargetedCrash = true;

				Math::Vector3<float> Coords = Native::GetOffsetFromEntityInWorldCoords(Player.m_Entity, Native::GetRandomFloatInRange(0.f, 10.f), Native::GetRandomFloatInRange(0.f, 10.f), 0.f);

				for (auto iterator : badObjects) {
					Native::RequestModel(iterator);
					Native::CreateObject(iterator, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_X, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_Y, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_Z, true, false, true);
					LOG_ERROR(std::format("{}", iterator).c_str());
				}


				Native::RequestModel(RAGE_JOAAT("prop_thindesertfiller_aa"));
				Native::CreateObject(RAGE_JOAAT("prop_thindesertfiller_aa"), Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_X, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_Y, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_Z, true, false, true);

				Native::RequestModel(RAGE_JOAAT("hei_bh1_08_details4_em_night"));
				Native::CreateObject(RAGE_JOAAT("hei_bh1_08_details4_em_night"), Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_X, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_Y, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_Z, true, false, true);

				Native::RequestModel(RAGE_JOAAT("prop_small_bushyba"));
				Native::CreateObject(RAGE_JOAAT("prop_small_bushyba"), Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_X, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_Y, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_Z, true, false, true);

				Native::RequestModel(RAGE_JOAAT("ss1_12_night_slod"));
				Native::CreateObject(RAGE_JOAAT("ss1_12_night_slod"), Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_X, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_Y, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_Z, true, false, true);

				Native::RequestModel(RAGE_JOAAT("bh1_07_flagpoles"));
				Native::CreateObject(RAGE_JOAAT("bh1_07_flagpoles"), Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_X, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_Y, Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, true).m_Z, true, false, true);

				Native::RequestModel(RAGE_JOAAT("avenger3"));
				Native::CreateVehicle(RAGE_JOAAT("avenger3"), Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(RAGE_JOAAT("arbitergt"));
				Native::CreateVehicle(RAGE_JOAAT("arbitergt"), Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(RAGE_JOAAT("astron2"));
				Native::CreateVehicle(RAGE_JOAAT("astron2"), Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(0x8E9254FB);
				Native::CreateVehicle(0x8E9254FB, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(0x67B3F020);
				Native::CreateVehicle(0x67B3F020, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(0x1ABA13B5);
				Native::CreateVehicle(0x1ABA13B5, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(0x5A82F9AE);
				Native::CreateVehicle(0x5A82F9AE, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(0x94204D89);
				Native::CreateVehicle(0x94204D89, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(0x9C9EFFD8);
				Native::CreatePed(21, 0x9C9EFFD8, Coords.m_X, Coords.m_Y, Coords.m_Z, Native::GetRandomFloatInRange(0.f, 100.f), true, false);

				Native::RequestModel(0x705E61F2);
				Native::CreatePed(21, 0x705E61F2, Coords.m_X, Coords.m_Y, Coords.m_Z, Native::GetRandomFloatInRange(0.f, 100.f), true, false);

				Native::RequestModel(0x028ABF95);
				Native::CreatePed(21, 0x028ABF95, Coords.m_X, Coords.m_Y, Coords.m_Z, Native::GetRandomFloatInRange(0.f, 100.f), true, false);

				Native::RequestModel(0x91CA3E2C);
				Native::CreatePed(21, 0x91CA3E2C, Coords.m_X, Coords.m_Y, Coords.m_Z, Native::GetRandomFloatInRange(0.f, 100.f), true, false);

				Native::RequestModel(0xe6bb31ab);
				Native::CreateObject(0xe6bb31ab, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, false);

				Native::RequestModel(0x7f2b2371);
				Native::CreateObject(0x7f2b2371, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, false);

				Native::RequestModel(0x91cd3b2);
				Native::CreateObject(0x91cd3b2, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, false);

				Native::RequestModel(0x76d281de);
				Native::CreateObject(0x76d281de, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, false);
			}
		}
	}

	void ScriptHostKick(Menu::PlayerVars& Player) {
		Utils::GetFiberPool()->Push([=] {
			if (Menu::Network::ForceScriptHost(Joaat("freemode"))) {
				Utils::WaitFiber(250);
				Menu::Global(1877042 + Player.m_ID + 1).As<int>() = 1;
			}
			});
	}

	void SessionHostKick(Menu::PlayerVars& Player) {
		if ((*Patterns::Vars::g_NetworkPlayerMgr)->m_local_net_player->is_host()) {
			if (Player.m_NetGamePlayer != Menu::GetLocalPlayer().m_NetGamePlayer) {
				Native::NetworkSessionKickPlayer(Player.m_ID);
			}
		}
		else Menu::Notify::stacked("You need to be host to use this kick");
	}
	struct weapons { std::string name; uint32_t id; };
	inline std::vector<weapons> WeaponsList = {
		{ "Pistol", 0x1b06d571 },
		{ "Pistol Mk II", 0xbfe256d4 },
		{ "Combat Pistol", 0x5ef9fec4 },
		{ "AP Pistol", 0x22d8fe39 },
		{ "Stun Gun", 0x3656c8c1 },
		{ "Pistol .50", 0x99aeeb3b },
		{ "SNS Pistol", 0xbfd21232 },
		{ "SNS Pistol Mk II", 0x88374054 },
		{ "Heavy Pistol", 0xd205520e },
		{ "Vintage Pistol", 0x83839c4 },
		{ "Flare Gun", 0x47757124 },
		{ "Marksman Pistol", 0xdc4db296 },
		{ "Heavy Revolver", 0xc1b3c3d1 },
		{ "Heavy Revolver Mk II", 0xcb96392f },
		{ "Double Action Revolver", 0x97ea20b8 },
		{ "Up-n-Atomizer", 0xaf3696a1 },
		{ "Ceramic Pistol", 0x2b5ef5ec },
		{ "Navy Revolver", 0x917f6c8c },
		{ "Gadget Pistol", 0x57a4368c },

		{ "Micro SMG", 0x13532244 },
		{ "SMG", 0x2be6766b },
		{ "SMG Mk II", 0x78a97cd0 },
		{ "Assault SMG", 0xefe7e2df },
		{ "Combat PDW", 0xa3d4d34 },
		{ "Machine Pistol", 0xdb1aa450 },
		{ "Mini SMG", 0xbd248b55 },
		{ "Unholy Hellbringer", 0x476bf155 },

		{ "Pump Shotgun", 0x1d073a89 },
		{ "Pump Shotgun Mk II", 0x555af99a },
		{ "Sawed-Off Shotgun", 0x7846a318 },
		{ "Assault Shotgun", 0xe284c527 },
		{ "Bullpup Shotgun", 0x9d61e50f },
		{ "Musket", 0xa89cb99e },
		{ "Heavy Shotgun", 0x3aabbbaa },
		{ "Double Barrel Shotgun", 0xef951fbb },
		{ "Sweeper Shotgun", 0x12e82d3d },
		{ "Combat Shotgun", 0x5a96ba4 },

		{ "Assault Rifle", 0xbfefff6d },
		{ "Assault Rifle Mk II", 0x394f415c },
		{ "Carbine Rifle", 0x83bf0278 },
		{ "Carbine Rifle Mk II", 0xfad1f1c9 },
		{ "Advanced Rifle", 0xaf113f99 },
		{ "Special Carbine", 0xc0a3098d },
		{ "Special Carbine Mk II", 0x969c3d67 },
		{ "Bullpup Rifle", 0x7f229f94 },
		{ "Bullpup Rifle Mk II", 0x84d6fafd },
		{ "Compact Rifle", 0x624fe830 },
		{ "Military Rifle", 0x9d1f17e6 },
		{ "Heavy Rifle", 0xC78D71B4 },
		{ "Tactical Rifle", 0xD1D5F52B },

		{ "MG", 0x9d07f764 },
		{ "Combat MG", 0x7fd62962 },
		{ "Combat MG Mk II", 0xdbbd7280 },
		{ "Gusenberg Sweeper", 0x61012683 },

		{ "Sniper Rifle", 0x5fc3c11 },
		{ "Heavy Sniper", 0xc472fe2 },
		{ "Heavy Sniper Mk II", 0xa914799 },
		{ "Marksman Rifle", 0xc734385a },
		{ "Marksman Rifle Mk II", 0x6a6c02e0 },
		{ "Precision Rifle", 0x6E7DDDEC },

		{ "RPG", 0xb1ca77b1 },
		{ "Grenade Launcher", 0xa284510b },
		{ "Grenade Launcher Smoke", 0x4dd2dc56 },
		{ "Minigun", 0x42bf8a85 },
		{ "Firework Launcher", 0x7f7497e5 },
		{ "Railgun", 0x6d544c99 },
		{ "Homing Launcher", 0x63ab0442 },
		{ "Compact Grenade Launcher", 0x781fe4a },
		{ "Widowmaker", 0xb62d1f67 },

		{ "Jerry Can", 0x34a67b97 },
		{ "Hazardous Jerry Can", 0xba536372 },
		{ "Parachute", 0xfbab5776 },
		{ "Fire Extinguisher", 0x60ec506 },
		{ "Digi Scanner", 0xfdbadced },

		{ "Grenade", 0x93e220bd },
		{ "BZ Gas", 0xa0973d5e },
		{ "Molotov Cocktail", 0x24b17070 },
		{ "Sticky Bomb", 0x2c3731d9 },
		{ "Proximity Mines", 0xab564b93 },
		{ "Snowballs", 0x787f0bb },
		{ "Pipe Bombs", 0xba45e8b8 },
		{ "Baseball", 0x23c9f95c },
		{ "Tear Gas", 0xfdbc8a50 },
		{ "Flare", 0x497facc3 },

		{ "Antique Cavalry Dagger", 0x92a27487 },
		{ "Baseball Bat", 0x958a4a8f },
		{ "Broken Bottle", 0xf9e6aa4b },
		{ "Crowbar", 0x84bd7bfd },
		{ "Flashlight", 0x8bb05fd7 },
		{ "Golf Club", 0x440e4788 },
		{ "Hammer", 0x4e875f73 },
		{ "Hatchet", 0xf9dcbf2d },
		{ "Brass Knuckles", 0xd8df3c3c },
		{ "Knife", 0x99b507ea },
		{ "Machete", 0xdd5df8d9 },
		{ "Switchblade", 0xdfe37640 },
		{ "Nightstick", 0x678b81b1 },
		{ "Pipe Wrench", 0x19044ee0 },
		{ "Battle Axe", 0xcd274149 },
		{ "Pool Cue", 0x94117305 },
		{ "Stone Hatchet", 0x3813fc08 },
	};
	void GiveAllWeapons(Menu::PlayerVars Player) {
		//Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s", TRANSLATE(T_Giving), Player.m_Name, TRANSLATE(T_AllWeapons)));

		Utils::GetFiberPool()->Push([=] {
			for (auto& Weapon : WeaponsList) {
				GiveWeapon(Player.m_Ped, Weapon.id, false, false);
				Utils::WaitFiber(10);
			}

			//Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %i %s", TRANSLATE(T_Gave), Player.m_Name, NUMOF(Global::Arrays::g_Weapons), TRANSLATE(T_Weapons)));
		});
	}

	void CommendPlayer(Menu::PlayerVars Player, int Commend) {
		if (!Player.m_NetGamePlayer) return;

	}

	void SpawnBodyguards(Menu::PlayerVars Player, int Count) {
		for (int i = 0; i < Count; i++) {
			Ped Ped = Native::ClonePed(Player.m_Ped, Player.m_Heading, true, false);
			if (Native::DoesEntityExist(Ped)) {
				Native::GiveWeaponToPed(Ped, 0x1B06D571, -1, false, true);
				Native::SetPedAsGroupMember(Ped, Native::GetPlayerGroup(Player.m_ID));
			}
		}
	}


	void ParachutePlayer(Menu::PlayerVars Player) {
		Math::Vector3<float> Rotation = Native::GetEntityRotation(Player.m_Ped, 0);
		Math::Vector3<float> Coords = Player.m_Coords;

		Native::SetEntityCoords(Menu::GetLocalPlayer().m_Ped, Coords.m_X + 10.0f, Coords.m_Y + 10.0f, Coords.m_Z + 24.0f, true, false, false, false);
		Native::SetEntityRotation(Menu::GetLocalPlayer().m_Ped, Rotation.m_X, Rotation.m_Y, Rotation.m_Z, 0, true);
		Native::TaskParachuteToTarget(Menu::GetLocalPlayer().m_Ped, Coords.m_X, Coords.m_Y, Coords.m_Z);
	}

	void GiveOffTheRadar(Menu::PlayerVars Player) {
		std::vector<int> List = {
			5315346,
			Player.m_ID,
			Native::GetNetworkTime(),
			Native::GetNetworkTime() + 60000,
			1,	// has start
			1,	// has end
			Menu::Global(1630317).At(Player.m_ID, 595).At(506).As<int>()
		};

		//TriggerScriptEvent(List, 7, Player.m_ID);
	}

	void RemoveWantedLevel(Menu::PlayerVars Player) {
		std::vector<int> List = {
			531462,
			Player.m_ID,
			Menu::Global(1630317).At(Player.m_ID, 595).At(506).As<int>()
		};

		//TriggerScriptEvent(List, 3, Player.m_ID);
	}
	void RemoveAttachments(Menu::PlayerVars Player) {
		/*Menu::Pools::GetObjectPool()->Run([&](Object Obj) {
			uint64_t Address = Engine::GetEntityAddress(Obj);
			if (Address) {
				if (*(uint64_t*)(Address + 0x340) == 0) { // Weapon Object
					if (Native::GetEntityModel(Obj) != 0x4FAA899A) {
						if (Native::IsEntityAttachedToEntity(Obj, Player.m_Ped)) {
							Menu::GetControlManager()->RequestControl(Obj, [&](Entity Ent) {
								Native::SetEntityAsMissionEntity(Ent, true, true);
								Native::DetachEntity(Ent, true, true);
								Native::DeleteEntity(&Ent);
								}, true);
						}
					}
				}
			}*/
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

			core->addOption(Framework::Options::SubmenuOption("Friendly")
				.setTarget("network_selected_player_friendly"));

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

		Framework::addPlayerSubmenu(&NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer, "network_selected_player_friendly", [=](Framework::Options::PCore* core) {
			core->addOption(Framework::Options::ToggleOption("Demi Godmode")
				.addToggle(&m_Vars.m_DemiGodmode));

			core->addOption(Framework::Options::ToggleOption("Give Off the Radar")
				.addToggle(&m_Vars.m_DemiGodmode));
		
			core->addOption(Framework::Options::ToggleOption("Disable Police")
				.addToggle(&m_Vars.m_DisablePolice));

			core->addOption(Framework::Options::ToggleOption("Karma")
				.addToggle(&Menu::GetSelectedPlayer().m_Revenge));

			core->addOption(Framework::Options::ButtonOption("Parachute")
				.addClick([] { ParachutePlayer(Menu::GetSelectedPlayer()); }));

			core->addOption(Framework::Options::ButtonOption(("Give All Weapons"))
				.addClick([] { GiveAllWeapons(Menu::GetSelectedPlayer()); }));

			core->addOption(Framework::Options::ButtonOption(("Remove Attachments"))
				.addClick([] { RemoveAttachments(Menu::GetSelectedPlayer()); }));

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
			}));

			core->addOption(Framework::Options::ButtonOption("Script Host Kick")
				.addClick([] { ScriptHostKick(Menu::GetSelectedPlayer()); }));

			core->addOption(Framework::Options::ButtonOption("Session Host Kick")
				.addClick([] { SessionHostKick(Menu::GetSelectedPlayer()); }));

			core->addOption(Framework::Options::ButtonOption("Crash")
				.addClick([] { CrashPlayer(Menu::GetSelectedPlayer(), m_Vars.m_Crash + 1); }));
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

	/*if (m_Vars.m_TargetedCrash) {
		if (GetTickCount() > m_Vars.m_TargetedCrashTimeout) {
			m_Vars.m_TargetedCrash = false;
			m_Vars.m_BrokenEntities.clear();
			LOG_WARN("Disabling crash for player");
		}
	}*/

	GetNetworkPlayersSelectedGriefingMenu()->Update();
}