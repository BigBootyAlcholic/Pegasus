#include "stdafx.hpp"
#include "network_players_selected.hpp"
#include "network_players_selected_griefing.hpp"
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
using namespace NetworkPlayersSelectedGriefingMenuVars;

namespace NetworkPlayersSelectedGriefingMenuVars {
	Vars_ m_Vars;

	void FakeMoneyPlayer(Menu::PlayerVars Player) {
		Native::CreateAmbientPickup(0x1E9A99F8, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 0, 0, 0x113FD533, 0, 1);
	}

	void RamPlayer(Menu::PlayerVars Player) {
		Menu::GetControlManager()->RequestModel(0x1FD824AF, [=](uint32_t Model) {
			Vehicle Vehicle = Native::CreateVehicle(Model, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 0.f, true, false, 1);
			if (Native::DoesEntityExist(Vehicle)) {
				Native::SetVehicleOutOfControl(Vehicle, true, true);
				Native::SetEntityHeading(Vehicle, Player.m_Heading - 90.f);
				Native::SetVehicleForwardSpeed(Vehicle, 100.f);
				Native::SetModelAsNoLongerNeeded(Model);
			}
			});
	}

	void CagePlayer(Menu::PlayerVars Player) {
		Menu::GetControlManager()->RequestModel(0x466EB7B8, [=](uint32_t Model) {
			Math::Vector3<float> MinDimensions;
			Math::Vector3<float> MaxDimensions;

			Native::GetModelDimensions(Model, &MinDimensions, &MaxDimensions);
			Math::Vector3<float> Diameter = MaxDimensions - MinDimensions;

			Math::Vector3<float> SpawnCoords = Native::GetOffsetFromEntityInWorldCoords(Player.m_Ped, -1.5f, -1.5f, 0.f);
			Native::GetGroundZFor3DCoord(SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, &SpawnCoords.m_Z, false);

			Object FirstObject = Native::CreateObject(Model, SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, true, true, false);
			if (Native::DoesEntityExist(FirstObject)) {
				float Rotation = Player.m_Heading;
				Native::SetEntityRotation(FirstObject, 0.f, 0.f, Rotation, 2, false);
				Native::FreezeEntityPosition(FirstObject, true);

				Object NextObject = FirstObject;
				for (int i = 0; i < 3; i++) {
					Rotation += 90.f;

					Math::Vector3<float> Next = Native::GetOffsetFromEntityInWorldCoords(NextObject, Diameter.m_X, 0.f, 0.f);
					NextObject = Native::CreateObject(Model, Next.m_X, Next.m_Y, Next.m_Z, true, true, false);
					if (Native::DoesEntityExist(NextObject)) {
						Native::SetEntityRotation(NextObject, 0.f, 0.f, Rotation, 2, false);
						Native::FreezeEntityPosition(NextObject, true);
					}
				}
			}
			});
	}

		

	void TriggerScriptEvent(int event_group, int64_t* args, int arg_count, int player_bits) {
		Caller::Call<void>(Patterns::Vars::g_TriggerScriptEvent, event_group, args, arg_count, player_bits);
	}

	void GiveWantedLevel(Menu::PlayerVars Player, int Level) {
		Native::ReportCrime(Player.m_ID, 8, Native::GetWantedLevelThreshold(Level));
	}

	void SetOnFire(Menu::PlayerVars Player) {
		if (!Player.m_InVehicle) {
			if (Native::GetEntitySpeed(Player.m_Ped) > 1.5f) {
				Math::Vector3<float> FVector = Native::GetEntityForwardVector(Player.m_Ped);

				for (int i = 1; i <= 3; i++) {
					Math::Vector3<float> Coords = Player.m_Coords;// + (FVector * i);
					Native::StartScriptFire(Coords.m_X, Coords.m_Y, Coords.m_Z, 5, true);
				}
			}
			else {
				Native::StartScriptFire(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 5, true);
			}
		}
	}

	void Explode(Menu::PlayerVars Player, int Type) {
		switch (Type) {
		case 0: // Normal
			Native::AddExplosion(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 29, 10000.f, true, false, 0.f, false);
			break;

		case 1: // Stealth
			Native::AddExplosion(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 29, 10000.f, false, true, 0.f, false);
			break;

		case 2: // Airstrike
			Menu::GetControlManager()->SimpleRequestWeaponAsset(0x47757124);
			Menu::GetControlManager()->SimpleRequestWeaponAsset(0xF8A3939F);

			if (Native::GetEntitySpeed(Player.m_Entity) > 1.5f) {
				Math::Vector3<float> FVector = Player.m_Coords;// + (Native::GetEntityForwardVector(Player.m_Entity) * 5.f);
				Native::ShootSingleBulletBetweenCoordsIgnoreEntity(FVector.m_X, FVector.m_Y, FVector.m_Z, FVector.m_X, FVector.m_Y, FVector.m_Z, Native::GetWeaponDamageType(0x47757124), false, 0x47757124, 0, true, false, 500.0f, Menu::GetLocalPlayer().m_Entity, 0);
				Native::ShootSingleBulletBetweenCoordsIgnoreEntity(FVector.m_X, FVector.m_Y, FVector.m_Z + 60.0f, FVector.m_X, FVector.m_Y, FVector.m_Z, Native::GetWeaponDamageType(0xF8A3939F), false, 0xF8A3939F, 0, true, false, 500.0f, Menu::GetLocalPlayer().m_Entity, 0);
			}
			else {
				Native::ShootSingleBulletBetweenCoordsIgnoreEntity(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, Native::GetWeaponDamageType(0x47757124), false, 0x47757124, 0, true, false, 500.0f, Menu::GetLocalPlayer().m_Entity, 0);
				Native::ShootSingleBulletBetweenCoordsIgnoreEntity(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z + 60.0f, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, Native::GetWeaponDamageType(0xF8A3939F), false, 0xF8A3939F, 0, true, false, 500.0f, Menu::GetLocalPlayer().m_Entity, 0);
			}

			break;
		}
	}

	void CEOBan(Menu::PlayerVars Player) {
		int64_t ceokick[4] = { (int64_t)eRemoteEvent::CeoBan, Native::PlayerId(), 1 };
		Engine::TriggerScriptEvent(1, ceokick, 3, 1 << Player.m_ID);
	}

	void CEOKick(Menu::PlayerVars Player) {
		int64_t ceokick[4] = { (int64_t)eRemoteEvent::CeoKick, Native::PlayerId(), 1, 5 };
		Engine::TriggerScriptEvent(1, ceokick, 4, 1 << Player.m_ID);
	}

	void Blame(Menu::PlayerVars Player) {
		Menu::GetPlayerManager()->Foreach([=](Menu::PlayerVars& P) {
			Native::AddOwnedExplosion(Player.m_Ped, P.m_Coords.m_X, P.m_Coords.m_Y, P.m_Coords.m_Z, 1, 10.f, true, false, 0.f);
			});
	}

	void Rape(Menu::PlayerVars Player) {
		Native::DetachEntity(Menu::GetLocalPlayer().m_Ped, true, true);
		// Native::ClearPedTasksImmediately(Menu::GetLocalPlayer().m_Ped);

		if (Player.m_Rape) {
			Menu::GetControlManager()->RequestAnimation("rcmpaparazzo_2", [=] {
				Native::AttachEntityToEntity(Menu::GetLocalPlayer().m_Ped, Player.m_Ped, -1, 0.f, Player.m_RapeType == 0 ? -0.3f : 0.3f, 0.f, 0.f, 0.f, 0.f, false, false, false, false, 0, false);
				Native::TaskPlayAnim(Menu::GetLocalPlayer().m_Ped, "rcmpaparazzo_2", Player.m_RapeType == 0 ? "shag_loop_a" : "shag_loop_poppy", 8.f, 0.f, -1, 1, 0, false, false, false);
				});
		}
	}

	void BlackScreen(Menu::PlayerVars Player) {
		//Menu::Helpers::TriggerScriptEvent({ Rage::Global::ApartmentTeleport, Player.m_ID, 0, 0 }, 4, Player.m_ID);
		int64_t ceokick[4] = { (int64_t)eRemoteEvent::Teleport, Native::PlayerId(), 0, 0 };
		Engine::TriggerScriptEvent(1, ceokick, 4, 1 << Player.m_ID);
	}

	void Quit(Menu::PlayerVars Player) {
		//Menu::Helpers::TriggerScriptEvent({ Rage::Global::ApartmentTeleport, Player.m_ID, 0, 0 }, 4, Player.m_ID);
		int64_t ceokick[4] = { (int64_t)eScriptEvents::ScriptEventFmmcInteriorDestruction, Native::PlayerId(), 0, 0 };
		Engine::TriggerScriptEvent(1, ceokick, 4, 1 << Player.m_ID);

		int64_t ceokick1[4] = { (int64_t)eScriptEvents::ScriptEventLaunchFreemodeEvent, Native::PlayerId(), 0, 0 };
		Engine::TriggerScriptEvent(1, ceokick1, 4, 1 << Player.m_ID);

		int64_t ceokick2[4] = { (int64_t)eScriptEvents::ScriptEventLaunchFmTriggerTut, Native::PlayerId(), 0, 0 };
		Engine::TriggerScriptEvent(1, ceokick2, 4, 1 << Player.m_ID);

		int64_t ceokick3[4] = { (int64_t)eScriptEvents::ScriptEventLaunchCustomPedUpdateAction, Native::PlayerId(), 0, 0 };
		Engine::TriggerScriptEvent(1, ceokick3, 4, 1 << Player.m_ID);

		int64_t ceokick4[4] = { (int64_t)eScriptEvents::ScriptEventFmmcPedRetaskHandshake, Native::PlayerId(), 0, 0 };
		Engine::TriggerScriptEvent(1, ceokick4, 4, 1 << Player.m_ID);
	}

	void GlitchPhysics(Menu::PlayerVars Player) {
		if (Player.m_Ped != Menu::GetLocalPlayer().m_Ped && Player.m_ID != Menu::GetLocalPlayer().m_ID) {
			Menu::GetControlManager()->RequestModel(0xd5c0bc07, [=](uint32_t Model) { // prop_acc_guitar_01:0xd5c0bc07
				Object Guitar = Native::CreateObject(Model, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, true, true, false);
				if (Native::DoesEntityExist(Guitar)) {
					Native::SetEntityVisible(Guitar, false, false);
					Native::AttachEntityToEntity(Guitar, Player.m_Ped, Native::GetPedBoneIndex(Player.m_Ped, 23553), 0.5f, -0.2f, 0.f, 0.f, 75.0f, 0.f, true, true, false, true, 1, true);
					Native::SetEntityCollision(Guitar, true, true);
					Native::SetModelAsNoLongerNeeded(Model);
				}
				});
		}
	}

	void TazePlayer(Menu::PlayerVars Player) {
		Menu::GetControlManager()->RequestWeaponAsset(0x3656c8c1, [=](uint32_t Hash) {
			Native::ShootSingleBulletBetweenCoordsIgnoreEntity(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z + 1.f, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 10, 0, Hash, Menu::GetLocalPlayer().m_Ped, true, false, 100.f, Menu::GetLocalPlayer().m_Ped, 0);
			});
	}

	void KickFromVehiclePlayer(Menu::PlayerVars Player) {
		if (Player.m_InVehicle) {
			Menu::GetControlManager()->RequestControl(Player.m_Vehicle, [](Entity) {}, true);
		}
	}

	void RagdollPlayer(Menu::PlayerVars Player) {
		Engine::SendRagdollEvent(Player.m_ID);
	}

	void RainRocketsPlayer(Menu::PlayerVars& Player) {
		Menu::Timers::RunTimedFunction(&Player.m_RainRocketsTimer, 750, [=] {
			Menu::GetControlManager()->RequestWeaponAsset(0x13579279, [=](uint32_t Model) {
				Math::Vector3<float> StartPosition = Player.m_Coords;
				StartPosition.m_X += Native::GetRandomFloatInRange(-20.f, 20.f);
				StartPosition.m_Y += Native::GetRandomFloatInRange(-20.f, 20.f);

				Native::ShootSingleBulletBetweenCoordsIgnoreEntity(StartPosition.m_X, StartPosition.m_Y, StartPosition.m_Z + Native::GetRandomFloatInRange(50.f, 150.f), StartPosition.m_X, StartPosition.m_Y, StartPosition.m_Z, 250, 0, Model, Menu::GetLocalPlayer().m_Ped, true, false, 1000.f, Menu::GetLocalPlayer().m_Entity, 0);
				});
			});
	}

	void SendPlayerTo(Menu::PlayerVars Player) {
		int64_t ceokick[4] = { (int64_t)eRemoteEvent::SendToCayoPerico, Native::PlayerId(), 20, 20 };
		Engine::TriggerScriptEvent(1, ceokick, 3, 1 << Player.m_ID);
	}

	int explosion_ids[39] = {
	  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39
	}; inline int explosion_id = 0;

	const char* explosion_type[] = {
   "Grenade", "Grenade Launcher", "Sticky Bomb", "Molotov", "Tank Shell", "Octane", "Car", "Plane", "Petrol Pump", "Bike",
   "Steam", "Flame", "Water", "Gas", "Boat", "Ship", "Truck", "Bullet", "Smoke Launcher", "Smoke Grenade", "BZ Gas", "Flae",
   "Gas Can", "Extinguisher", "Programmable Ar", "Train", "Barrel", "Propane", "Blimp", "Flame Explosion", "Tanker", "Rocket",
   "Vehicle Bullet", "Gas Tank", "Bird Crap", "Snow Ball"
	};

	void ClonePlayer(Menu::PlayerVars Player) {
		Ped Cloned = Native::ClonePed(Player.m_Ped, Player.m_Heading, true, true);
		if (Player.m_CloneType == 1 && Native::DoesEntityExist(Cloned)) {
			Native::GiveWeaponToPed(Cloned, 0x7FD62962, 9999, true, true);
			Native::TaskCombatPed(Cloned, Player.m_Ped, 0, 16);
			Native::SetPedKeepTask(Cloned, true);
		}
	}
	struct tsounds { const char* name; std::pair<const char*, const char*> hash; };
	std::vector<tsounds> g_Sounds = {
		{ "Alien", { "SPAWN", "BARRY_01_SOUNDSET" } },
		{ "Whistle", { "Franklin_Whistle_For_Chop", "SPEECH_RELATED_SOUNDS" } },
		{ "EMP", { "EMP_Vehicle_Hum", "DLC_HEIST_BIOLAB_DELIVER_EMP_SOUNDS" } },
		{ "Helicopter", { "Helicopter_Wind", "BASEJUMPS_SOUNDS" } },
		{ "Scan", { "SCAN", "EPSILONISM_04_SOUNDSET" } },
		{ "Leafblower", { "GARDENING_LEAFBLOWER_ANIM_TRIGGERED", "" } },
		{ "Unlock", { "Bar_Unlock_And_Raise", "DLC_IND_ROLLERCOASTER_SOUNDS" } },
		{ "Brakes", { "MOD_SHOP_BRAKES_ONESHOT", "" } },

		{ "Carwash", { "SPRAY", "CARWASH_SOUNDS" } },
		{ "Carwash 2", { "SPRAY_CAR", "CARWASH_SOUNDS" } },
		{ "Carwash 3", { "DRYER", "CARWASH_SOUNDS" } },

		{ "Keycard", { "Keycard_Success", "DLC_HEISTS_BIOLAB_FINALE_SOUNDS" } },
		{ "Keycard 2", { "Keycard_Fail", "DLC_HEISTS_BIOLAB_FINALE_SOUNDS" } },

		{ "Ringtone", { "Remote_Ring", "Phone_SoundSet_Michael" } },
		{ "Ringtone 2", { "PED_PHONE_DIAL_01", "" } },

		{ "Beep", { "Crate_Beeps", "MP_CRATE_DROP_SOUNDS" } },
		{ "Beep 2", { "Crate_Collect", "MP_CRATE_DROP_SOUNDS" } },
		{ "Endless Beep", { "CONTINUAL_BEEP", "EPSILONISM_04_SOUNDSET" } },
		{ "Short Beep", { "IDLE_BEEP", "EPSILONISM_04_SOUNDSET" } },
		{ "Long Beep", { "Beep_Green", "DLC_HEIST_HACKING_SNAKE_SOUNDS" } },
		{ "Solo Fast Beep", { "Beep_Red", "DLC_HEIST_HACKING_SNAKE_SOUNDS" } },
		{ "Confirm Beep", { "CONFIRM_BEEP", "HUD_MINI_GAME_SOUNDSET" } },

		{ "5s Timer", { "5_Second_Timer", "DLC_HEISTS_GENERAL_FRONTEND_SOUNDS" } },
		{ "5s Timer 2", { "MP_5_SECOND_TIMER", "HUD_FRONTEND_DEFAULT_SOUNDSET" } },
		{ "Idle Timer", { "MP_IDLE_TIMER", "HUD_FRONTEND_DEFAULT_SOUNDSET" } },
		{ "Out of Bounds Timer", { "Out_Of_Bounds_Timer", "DLC_HEISTS_GENERAL_FRONTEND_SOUNDS" } },
		{ "Timer Stop", { "TIMER_STOP", "HUD_MINI_GAME_SOUNDSET" } },

		{ "Pickup", { "PICK_UP", "HUD_FRONTEND_DEFAULT_SOUNDSET" } },
		{ "Pickup 2", { "PICK_UP_SOUND", "HUD_FRONTEND_CUSTOM_SOUNDSET" } },
		{ "Pickup 3", { "PICKUP_WEAPON_SMOKEGRENADE", "HUD_FRONTEND_WEAPONS_PICKUPS_SOUNDSET" } },

		{ "Silence", { "3_2_1_NON_RACE", "HUD_MINI_GAME_SOUNDSET" } },
		{ "Silence 2", { "Airhorn", "DLC_TG_Running_Back_Sounds" } },

		{ "Screen Flash", { "ScreenFlash", "MissionFailedSounds" } },
		{ "Screen Flash 2", { "ScreenFlash", "WastedSounds" } },

		{ "Click", { "Click", "DLC_HEIST_HACKING_SNAKE_SOUNDS" } },
		{ "Click Fail", { "Click_Fail", "WEB_NAVIGATION_SOUNDS_PHONE" } },
		{ "Click Special", { "Click_Special", "WEB_NAVIGATION_SOUNDS_PHONE" } },

		{ "Nav Arrow Ahead", { "Nav_Arrow_Ahead", "DLC_HEISTS_GENERAL_FRONTEND_SOUNDS" } },
		{ "Nav Arrow Left", { "Nav_Arrow_Left", "DLC_HEISTS_GENERAL_FRONTEND_SOUNDS" } },
		{ "Nav Arrow Right", { "Nav_Arrow_Right", "DLC_HEISTS_GENERAL_FRONTEND_SOUNDS" } },

		{ "Checkpoint Missed", { "CHECKPOINT_MISSED", "HUD_MINI_GAME_SOUNDSET" } },
		{ "Checkpoint Ahead", { "CHECKPOINT_AHEAD", "HUD_MINI_GAME_SOUNDSET" } },
		{ "Checkpoint Perfect", { "CHECKPOINT_PERFECT", "HUD_MINI_GAME_SOUNDSET" } },
		{ "Checkpoint Under Bridge", { "CHECKPOINT_UNDER_THE_BRIDGE", "HUD_MINI_GAME_SOUNDSET" } },

		{ "Menu Select", { "Apt_Style_Purchase", "DLC_APT_Apartment_SoundSet" } },
		{ "Menu Select 2", { "SELECT", "HUD_MINI_GAME_SOUNDSET" } },

		{ "Hack Failed", { "Hack_Failed", "DLC_HEIST_BIOLAB_PREP_HACKING_SOUNDS" } },
		{ "Hack Success", { "Hack_Success", "DLC_HEIST_BIOLAB_PREP_HACKING_SOUNDS" } },

		{ "Pin Bad", { "Pin_Bad", "DLC_HEIST_BIOLAB_PREP_HACKING_SOUNDS" } },
		{ "Pin Button", { "PIN_BUTTON", "ATM_SOUNDS" } },
		{ "Pin Good", { "Pin_Good", "DLC_HEIST_BIOLAB_PREP_HACKING_SOUNDS" } },

		{ "Tennis Match Point", { "TENNIS_MATCH_POINT", "HUD_AWARDS" } },
		{ "Tennis Point Won", { "TENNIS_POINT_WON", "HUD_AWARDS" } },

		{ "Start Squelch", { "Start_Squelch", "CB_RADIO_SFX" } },
		{ "End Squelch", { "End_Squelch", "CB_RADIO_SFX" } },

		{ "Garage Door Open", { "RAMP_UP", "TRUCK_RAMP_DOWN" } },
		{ "1st Person Transition", { "1st_Person_Transition", "PLAYER_SWITCH_CUSTOM_SOUNDSET" } },
		{ "Emphasis", { "3_2_1", "HUD_MINI_GAME_SOUNDSET" } },
		{ "5s Warning", { "5_SEC_WARNING", "HUD_MINI_GAME_SOUNDSET" } },
		{ "Notification", { "ATM_WINDOW", "HUD_FRONTEND_DEFAULT_SOUNDSET" } },
		{ "Challenge Unlocked", { "CHALLENGE_UNLOCKED", "HUD_AWARDS" } },
		{ "Mission Success", { "BASE_JUMP_PASSED", "HUD_AWARDS" } },
		{ "Wasted", { "Bed", "WastedSounds" } },
		{ "Bus Pickup", { "Bus_Schedule_Pickup", "DLC_PRISON_BREAK_HEIST_SOUNDS" } },
		{ "Taken Photo", { "Camera_Shoot", "Phone_Soundset_Franklin" } },
		{ "Switching Characters", { "CHARACTER_SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET" } },
		{ "Close Window", { "CLOSE_WINDOW", "LESTER1A_SOUNDS" } },
		{ "Crash", { "Crash", "DLC_HEIST_HACKING_SNAKE_SOUNDS" } },
		{ "Delete", { "DELETE", "HUD_DEATHMATCH_SOUNDSET" } },
		{ "Truck Rev", { "DiggerRevOneShot", "BulldozerDefault" } },
		{ "Drill Pin Break", { "Drill_Pin_Break", "DLC_HEIST_FLEECA_SOUNDSET" } },
		{ "Edit", { "EDIT", "HUD_DEATHMATCH_SOUNDSET" } },
		{ "Error", { "ERROR", "HUD_AMMO_SHOP_SOUNDSET" } },
		{ "Failure", { "Failure", "DLC_HEIST_HACKING_SNAKE_SOUNDS" } },
		{ "Falling Crates", { "Falling_Crates", "EXILE_1" } },
		{ "Car Breakdown", { "FAMILY_1_CAR_BREAKDOWN", "FAMILY1_BOAT" } },
		{ "First Place", { "FIRST_PLACE", "HUD_MINI_GAME_SOUNDSET" } },
		{ "Flight School Passed", { "FLIGHT_SCHOOL_LESSON_PASSED", "HUD_AWARDS" } },
		{ "Focus In", { "FocusIn", "HintCamSounds" } },
		{ "Go", { "GO", "HUD_MINI_GAME_SOUNDSET" } },
		{ "Goal", { "Goal", "DLC_HEIST_HACKING_SNAKE_SOUNDS" } },
		{ "Grab Parachute", { "Grab_Parachute", "BASEJUMPS_SOUNDS" } },
		{ "Hang Up", { "Hang_Up", "Phone_SoundSet_Michael" } },
		{ "Highlight Error", { "Highlight_Error", "DLC_HEIST_PLANNING_BOARD_SOUNDS" } },
		{ "Hit", { "Hit", "RESPAWN_SOUNDSET" } },
		{ "Hit 2", { "Hit_1", "LONG_PLAYER_SWITCH_SOUNDS" } },
		{ "Hit Out", { "Hit_Out", "PLAYER_SWITCH_CUSTOM_SOUNDSET" } },
		{ "Cash Counter Complete", { "LOCAL_PLYR_CASH_COUNTER_COMPLETE", "DLC_HEISTS_GENERAL_FRONTEND_SOUNDS" } },
		{ "Lose Match", { "LOOSE_MATCH", "HUD_MINI_GAME_SOUNDSET" } },
		{ "Loser", { "LOSER", "HUD_AWARDS" } },
		{ "Menu Accept", { "Menu_Accept", "Phone_SoundSet_Default" } },
		{ "Mission Pass Notify", { "Mission_Pass_Notify", "DLC_HEISTS_GENERAL_FRONTEND_SOUNDS" } },
		{ "Award", { "MP_AWARD", "HUD_FRONTEND_DEFAULT_SOUNDSET" } },
		{ "Idle Kick", { "MP_IDLE_KICK", "HUD_FRONTEND_DEFAULT_SOUNDSET" } },
		{ "Rank Up", { "MP_RANK_UP", "HUD_FRONTEND_DEFAULT_SOUNDSET" } },
		{ "Wave Complete", { "MP_WAVE_COMPLETE", "HUD_FRONTEND_DEFAULT_SOUNDSET" } },
		{ "On", { "ON", "NOIR_FILTER_SOUNDS" } },
		{ "On Call Player Join", { "On_Call_Player_Join", "DLC_HEISTS_GENERAL_FRONTEND_SOUNDS" } },
		{ "Open Window", { "OPEN_WINDOW", "LESTER1A_SOUNDS" } },
		{ "Other Text", { "OTHER_TEXT", "HUD_AWARDS" } },
		{ "Player Collect", { "Player_Collect", "DLC_PILOT_MP_HUD_SOUNDS" } },
		{ "Power Down", { "Power_Down", "DLC_HEIST_HACKING_SNAKE_SOUNDS" } },
		{ "Pre Screen Stinger", { "Pre_Screen_Stinger", "DLC_HEISTS_FAILED_SCREEN_SOUNDS" } },
		{ "Property Purchase", { "PROPERTY_PURCHASE", "HUD_AWARDS" } },
		{ "Money Lost", { "PS2A_MONEY_LOST", "PALETO_SCORE_2A_BANK_SS" } },
		{ "Put Away", { "Put_Away", "Phone_SoundSet_Michael" } },
		{ "Round Ending", { "ROUND_ENDING_STINGER_CUSTOM", "CELEBRATION_SOUNDSET" } },
		{ "Heartbeat", { "Short_Transition_In", "PLAYER_SWITCH_CUSTOM_SOUNDSET" } },
		{ "Bomb Loading", { "STUN_COLLECT", "MINUTE_MAN_01_SOUNDSET" } },
	};

	void StartSoundPlayer(Menu::PlayerVars Player, std::pair<const char*, const char*> Sound, bool Add, int OptionID) {
		/*if (!Native::HasSoundFinished(m_Vars.m_Sounds[OptionID])) {
			Native::StopSound(m_Vars.m_Sounds[OptionID]);
			return;
		}*/

		int SoundID = Native::GetSoundId();
		Native::PlaySoundFromCoord(SoundID, Sound.first, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, Sound.second, true, 0, true);

		if (Add) {
			m_Vars.m_Sounds[OptionID] = SoundID;
		}
	}

	void SpawnAttackers(Menu::PlayerVars Player) {
		std::vector<Entity> Entities;

		if (Player.m_AttackerType == 1) { // Military Buzzards
			Menu::GetControlManager()->SimpleRequestModel(0x2f03547b); // buzzard
			Menu::GetControlManager()->SimpleRequestModel(0x613E626C); // mw

			for (int i = 0; i < Player.m_AttackerCount; i++) {
				Math::Vector3_<float> SpawnCoords = { Player.m_Coords.m_X + Native::GetRandomFloatInRange(0.f, 100.f), Player.m_Coords.m_Y + Native::GetRandomFloatInRange(0.f, 100.f), Player.m_Coords.m_Z + 100.f + (i * 10.f) };

				Vehicle Vehicle = Native::CreateVehicle(0x2f03547b, SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, Player.m_Heading, true, true, 0);
				if (Native::DoesEntityExist(Vehicle)) {
					Native::ApplyForceToEntity(Vehicle, 1, 0.f, 0.f, 50.f, 0.f, 0.f, 0.f, 0, true, true, true, false, true);

					Entities.push_back(Vehicle);

					Ped Ped = Native::CreateRandomPedAsDriver(Vehicle, true);
					if (Native::DoesEntityExist(Ped)) {
						Entities.push_back(Ped);

						Native::TaskHeliChase(Ped, Player.m_Ped, 0.f, 0.f, 50.f);
						Native::TaskCombatPed(Ped, Player.m_Ped, 0, 16);
						Native::SetPedKeepTask(Ped, true);

						for (int j = 0; j < Native::GetVehicleMaxNumberOfPassengers(Vehicle); j++) {
							Ped = Native::CreatePedInsideVehicle(Vehicle, 29, 0x613E626C, i, true, false);
							if (Native::DoesEntityExist(Ped)) {
								Entities.push_back(Ped);

								Native::SetPedCombatAbility(Ped, 100);
								Native::GiveDelayedWeaponToPed(Ped, 0x394f415c, 9999, true);
								Native::TaskCombatPed(Ped, Player.m_Ped, 0, 16);
								Native::SetPedKeepTask(Ped, true);
							}
						}
					}
				}
			}
		}

		for (auto& E : Entities) {
			Native::SetEntityInvincible(E, Player.m_AttackerInvincibility);
			Native::SetEntityVisible(E, !Player.m_AttackerInvisibility, true);
		}
	}
}

void NetworkPlayersSelectedGriefingMenu::Run() {

	Framework::addPlayerSubmenu(&NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer, "network_selected_player_griefing", [=](Framework::Options::PCore* core) {
		g_EnablePlayerInfo = true;

		core->addOption(Framework::Options::SubmenuOption("Explosions")
			.setTarget("selected_explosions"));

		core->addOption(Framework::Options::SubmenuOption("Sounds")
			.setTarget("selected_sounds"));

		core->addOption(Framework::Options::SubmenuOption("Attackers")
			.setTarget("selected_attackers"));

		core->addOption(Framework::Options::ToggleOption("Freeze")
			.addToggle(&m_Vars.m_DisableTasks));

		core->addOption(Framework::Options::ToggleOption("Taze")
			.addToggle(&m_Vars.m_Taze));

		core->addOption(Framework::Options::ToggleOption("Always Wanted")
			.addToggle(&m_Vars.m_AlwaysWanted));

		core->addOption(Framework::Options::ToggleOption("Ragdoll")
			.addToggle(&m_Vars.m_Ragdoll));

		core->addOption(Framework::Options::ToggleOption("Kick from Vehicle")
			.addToggle(&m_Vars.m_KickFromVehicle));

		core->addOption(Framework::Options::ToggleOption("Glitch Physics")
			.addToggle(&m_Vars.m_GlitchPhysics));	

		core->addOption(Framework::Options::ToggleOption("Forcefield")
			.addToggle(&m_Vars.m_Forcefield));

		core->addOption(Framework::Options::ToggleOption("Rain Rockets")
			.addToggle(&m_Vars.m_RainRockets));

		core->addOption(Framework::Options::ToggleOption("Fake Money Drop")
			.addToggle(&m_Vars.m_FakeMoneyDrop));

		core->addOption(Framework::Options::ButtonOption("Glitch Physics")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					Native::RemoveAllPedWeapons(Player.m_Ped, true);
					});
				}));

		core->addOption(Framework::Options::ButtonOption("Remove Weapons")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					Native::RemoveAllPedWeapons(Player.m_Ped, true);
					});
				}));

		core->addOption(Framework::Options::ButtonOption("Ragdoll")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					RagdollPlayer(Player);
					});
				}));


		core->addOption(Framework::Options::ButtonOption("Taze")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					TazePlayer(Player);
					});
				}));

		core->addOption(Framework::Options::ButtonOption("Give Wanted Level")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					GiveWantedLevel(Player, 5);
				});
			}));

		core->addOption(Framework::Options::ButtonOption("Kick From Vehicle")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					KickFromVehiclePlayer(Player);
					});
			}));

		core->addOption(Framework::Options::ButtonOption("Ram")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					RamPlayer(Player);
				});
			}));


		core->addOption(Framework::Options::ButtonOption("Cage")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					CagePlayer(Player);
				});
			}));

		core->addOption(Framework::Options::ButtonOption("Clone")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					ClonePlayer(Player);
					});
			}));

		core->addOption(Framework::Options::ButtonOption("Black Screen")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					BlackScreen(Player);
					});
				}));

		core->addOption(Framework::Options::ButtonOption("Set on Fire")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					SetOnFire(Player);
					});
				}));

		core->addOption(Framework::Options::ButtonOption("Explode")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					Explode(Player, m_Vars.m_Explode);
					});
			}));

		core->addOption(Framework::Options::ButtonOption("Ceo Kick")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					CEOKick(Player);
				});
			}));

		core->addOption(Framework::Options::ButtonOption("Ceo Ban")
			.addClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					CEOBan(Player);
					});
			}));

	});


	Framework::addPlayerSubmenu(&NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer, "selected_sounds", [=](Framework::Options::PCore* core) {
		g_EnablePlayerInfo = true;
		core->addOption(Framework::Options::ButtonOption("Stop All")
			.addClick([] {
				for (auto& Sound : m_Vars.m_Sounds) {
					Native::StopSound(Sound.second);
				}
				}));

		core->addOption(Framework::Options::BreakOption("Sounds"));

		for (int i = 0; i < g_Sounds.size(); i++) {
			auto Arr = g_Sounds[i];

			core->addOption(Framework::Options::ButtonOption(Arr.name)
				.addClick([=] {

					Menu::GetPlayerManager()->Foreach([=](Menu::PlayerVars& Player) {
						StartSoundPlayer(Player, Arr.hash, true, i);
						});
					
				}));
		}
	});
	
	Framework::addPlayerSubmenu(&NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer, "selected_attackers", [=](Framework::Options::PCore* core) {
		g_EnablePlayerInfo = true;
		core->addOption(Framework::Options::ToggleOption("Invincibility")
			.addToggle(&Menu::GetSelectedPlayer().m_AttackerInvincibility));

		core->addOption(Framework::Options::ToggleOption("Invisibility")
			.addToggle(&Menu::GetSelectedPlayer().m_AttackerInvisibility));

		core->addOption(Framework::Options::NumberOption<int>("Camera Shake")
			.addNumber(&Menu::GetSelectedPlayer().m_AttackerCount)
			.addMin(1).addMax(100).addStep(1));

		core->addOption(Framework::Options::ButtonOption(("Spawn Attackers"))
			.addClick([] { SpawnAttackers(Menu::GetSelectedPlayer()); }));
	});

	Framework::addPlayerSubmenu(&NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer, "selected_explosions", [=](Framework::Options::PCore* core) {
		g_EnablePlayerInfo = true;
		core->addOption(Framework::Options::scrollOption<const char*, int>("Explosion Type")
			.addScroll(&explosion_type).setPosition(&explosion_id));

		core->addOption(Framework::Options::NumberOption<float>("Camera Shake")
			.addNumber(&m_Vars.g_camera_shake)
			.addMin(0.f).addMax(100.f).addStep(1.f).setPrecision(0));

		core->addOption(Framework::Options::NumberOption<float>("Radius")
			.addNumber(&m_Vars.g_camera_shake)
			.addMin(0.f).addMax(500.f).addStep(1.f).setPrecision(0));

		core->addOption(Framework::Options::ToggleOption("Invisible")
			.addToggle(&m_Vars.g_visible));
		core->addOption(Framework::Options::ToggleOption("Audible")
			.addToggle(&m_Vars.g_audible));
		core->addOption(Framework::Options::ToggleOption("Loop")
			.addToggle(&m_Vars.g_explosion_loop));
		core->addOption(Framework::Options::ButtonOption("Explode")
			.addClick([] {
				Math::Vector3<float> c = Native::GetEntityCoords(Menu::GetSelectedPlayer().m_Ped, 0);
				Native::AddExplosion(c.m_X, c.m_Y, c.m_Z, explosion_ids[explosion_id], 1000, m_Vars.g_audible, m_Vars.g_visible, m_Vars.g_camera_shake, false);
			}));
	});
	

}


void NetworkPlayersSelectedGriefingMenu::Update() {
	Run();
	Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
		if (Player.m_AlwaysWanted || m_Vars.m_AlwaysWanted) {
			GiveWantedLevel(Player, 5);
		}

		if (Player.m_DisableTasks || m_Vars.m_DisableTasks) {
			Native::ClearPedTasksImmediately(Player.m_Ped);
		}

		if (Player.m_Taze || m_Vars.m_Taze) {
			Menu::Timers::RunTimedFunction(&Player.m_TazeTimer, 5000, [=] {
				TazePlayer(Player);
				});
		}

		if (Player.m_KickFromVehicle || m_Vars.m_KickFromVehicle) {
			Menu::Timers::RunTimedFunction(&Player.m_KickFromVehicleTimer, 1000, [=] {
				KickFromVehiclePlayer(Player);
				});
		}

		if (Player.m_GlitchPhysics || m_Vars.m_GlitchPhysics) {
			Menu::Timers::RunTimedFunction(&Player.m_GlitchPhysicsTimer, 1000, [=] {
				Object Guitar = Native::GetClosestObjectOfType(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 5.f, 0xd5c0bc07, false, false, false);
				if (!Guitar || !Native::DoesEntityExist(Guitar) || !Native::IsEntityAttachedToEntity(Guitar, Player.m_Ped)) {
					GlitchPhysics(Player);
				}
				});
		}

		if (Player.m_Ragdoll || m_Vars.m_Ragdoll) {
			Menu::Timers::RunTimedFunction(&Player.m_RagdollTimer, 1000, [=] {
				RagdollPlayer(Player);
				});
		}

		if (Player.m_FakeMoneyDrop || m_Vars.m_FakeMoneyDrop) {
			Menu::Timers::RunTimedFunction(&Player.m_FakeMoneyDropTimer, 500, [=] {
				FakeMoneyPlayer(Player);
				});
		}

		if (Player.m_RainRockets || m_Vars.m_RainRockets) {
			RainRocketsPlayer(Player);
		}

		if (Player.m_Forcefield || m_Vars.m_Forcefield) {
			Native::AddExplosion(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 29, 100.f, false, true, 0.f, true);
		}
		}, true);
}