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

}

void NetworkPlayersSelectedGriefingMenu::Run() {

	Framework::addPlayerSubmenu(&NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer, "network_selected_player_griefing", [=](Framework::Options::PCore* core) {
		g_EnablePlayerInfo = true;

		core->addOption(Framework::Options::SubmenuOption("Explosions")
			.setTarget("selected_explosions"));

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

		if (Player.m_RainRockets || m_Vars.m_RainRockets) {
			RainRocketsPlayer(Player);
		}

		if (Player.m_Forcefield || m_Vars.m_Forcefield) {
			Native::AddExplosion(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 29, 100.f, false, true, 0.f, true);
		}
		}, true);
}