#include "stdafx.hpp"
#include "vehicle.hpp"
#include "cheat/util/notify.hpp"
#include "cheat/util/control_manager.hpp"
#include "core/patterns.hpp"
#include "rage/engine.hpp"
#include "util/fiber.hpp"
#include "util/fiber_pool.hpp"
using namespace VehicleMenuVars;

namespace VehicleMenuVars {
	Vars_ m_Vars;

	void Boost(Vehicle Veh, bool Forwards) {
		Native::RequestScriptAudioBank("DLC_IMPORTEXPORT/STEAL_STUNT", false, -1);
		Native::PlaySoundFromEntity(-1, "Speed_Boost", Veh, "DLC_IE_Stunt_Man_Sounds", false, 0);
		Native::SetVehicleForwardSpeed(Veh, m_Vars.m_BoostSpeed + Native::GetEntitySpeed(Veh));

	}

	void BypassMaxSpeed() {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			Native::SetEntityMaxSpeed(Menu::GetLocalPlayer().m_Vehicle, 9999999.f);
		}
	}

	void LimitMaxSpeed() {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			Native::SetEntityMaxSpeed(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_LimitMaxSpeed ? m_Vars.m_LimitMaxSpeedVal / g_SelectedMetricSystem.first : 500.f);
		}
	}

	void RunAcrobatic(Vehicle Veh, std::pair<Math::Vector3_<float>, Math::Vector3_<float>> Data) {
		if (Veh) {
			Native::ApplyForceToEntity(Veh, 1, Data.first.m_X, Data.first.m_Y, Data.first.m_Z, Data.second.m_X, Data.second.m_Y, Data.second.m_Z, 0, true, true, true, false, true);
		}
	}

	void MaxUpgrades(Vehicle veh) {
		Native::SetVehicleFixed(veh);
		Native::SetVehicleDirtLevel(veh, 0);
		Native::SetVehicleTyresCanBurst(veh, false);
		Native::SetVehicleModKit(veh, 0);

		for (int i = MOD_SPOILER; i <= MOD_ARMOR; i++) {
			if (Native::GetNumVehicleMods(veh, i) > 0) {
				Native::SetVehicleMod(veh, i, Native::GetNumVehicleMods(veh, i) - 1, 0);
			}
		}

		for (int i = MOD_PLATEHOLDER; i <= MOD_LIVERY; i++) {
			if (Native::GetNumVehicleMods(veh, i) > 0) {
				Native::SetVehicleMod(veh, i, Native::GetNumVehicleMods(veh, i) - 1, 0);
			}
		}

		Native::SetVehicleMod(veh, MOD_HORNS, 30, 0);

		if (Native::GetEntityModel(veh) != joaat("vigilante")) {
			Native::SetVehicleWheelType(veh, 7);
			Native::SetVehicleMod(veh, MOD_FRONTWHEELS, 39, 1);
			Native::SetVehicleMod(veh, MOD_BACKWHEELS, 39, 1);
		}

		for (int i = MOD_UNK17; i <= MOD_XENONLIGHTS; i++) {
			Native::ToggleVehicleMod(veh, i, true);
		}

		Native::SetVehicleColours(veh, 160, 160);
		Native::SetVehicleExtraColours(veh, 160, 160);
		Native::SetVehicleWindowTint(veh, 5);

		Color temp;
		temp.r = Native::GetRandomIntInRange(0, 255);
		temp.g = Native::GetRandomIntInRange(0, 255);
		temp.b = Native::GetRandomIntInRange(0, 255);

		Native::SetVehicleTyreSmokeColor(veh, temp.r, temp.g, temp.b);
		Native::SetVehicleCustomPrimaryColour(veh, temp.b, temp.g, temp.r);
		Native::SetVehicleCustomSecondaryColour(veh, temp.r, temp.g, temp.b);

		Native::_SetVehicleNeonLightEnabled(veh, 0, true);
		Native::_SetVehicleNeonLightEnabled(veh, 1, true);
		Native::_SetVehicleNeonLightEnabled(veh, 2, true);
		Native::_SetVehicleNeonLightEnabled(veh, 3, true);
		Native::_SetVehicleNeonLightsColour(veh, temp.r, temp.b, temp.b);

		Native::SetVehicleNumberPlateTextIndex(veh, 5);
	}

	void Downgrade(Vehicle veh) {
		Native::SetVehicleFixed(veh);
		Native::SetVehicleDirtLevel(veh, 0);
		Native::SetVehicleTyresCanBurst(veh, true);
		Native::SetVehicleModKit(veh, 0);

		for (int i = MOD_SPOILER; i <= MOD_ARMOR; i++) {
			Native::RemoveVehicleMod(veh, i);
		}

		for (int i = MOD_PLATEHOLDER; i <= MOD_LIVERY; i++) {
			Native::RemoveVehicleMod(veh, i);
		}

		Native::RemoveVehicleMod(veh, MOD_HORNS);
		Native::RemoveVehicleMod(veh, MOD_FRONTWHEELS);

		for (int i = MOD_UNK17; i <= MOD_XENONLIGHTS; i++) {
			Native::ToggleVehicleMod(veh, i, false);
		}

		Native::ClearVehicleCustomPrimaryColour(veh);
		Native::ClearVehicleCustomSecondaryColour(veh);
		Native::SetVehicleColours(veh, 135, 135);
		Native::SetVehicleExtraColours(veh, 135, 135);
		Native::SetVehicleWindowTint(veh, 0);
		Native::SetVehicleNumberPlateTextIndex(veh, 1);

		Native::_SetVehicleNeonLightEnabled(veh, 0, false);
		Native::_SetVehicleNeonLightEnabled(veh, 1, false);
		Native::_SetVehicleNeonLightEnabled(veh, 2, false);
		Native::_SetVehicleNeonLightEnabled(veh, 3, false);
	}

	Vehicle CloneVehicle(Vehicle Veh) {
		Vehicle SpawnedVehicle = 0;

		if (Native::DoesEntityExist(Veh)) {
			uint32_t M = Native::GetEntityModel(Veh);
			Menu::GetControlManager()->SimpleRequestModel(M);

			Math::Vector3<float> Coords = Native::GetEntityCoords(Veh, true);
			Math::Vector3<float> SpawnCoords = Coords;// (Native::GetEntityForwardVector(Veh) * 5.f);

			*(unsigned short*)Patterns::Vars::g_SetNetworkedThread = 0x9090;
			SpawnedVehicle = Native::CreateVehicle(M, SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, Native::GetEntityHeading(Veh), true, true, 0);
			*(unsigned short*)Patterns::Vars::g_SetNetworkedThread = 0x0574;
			if (Native::DoesEntityExist(SpawnedVehicle)) {
				Native::SetVehicleDirtLevel(SpawnedVehicle, Native::GetVehicleDirtLevel(Veh));
				Native::SetVehicleTyresCanBurst(SpawnedVehicle, Native::GetVehicleTyresCanBurst(Veh));
				Native::SetVehicleModKit(SpawnedVehicle, 0);

				for (int i = MOD_SPOILER; i <= MOD_ARMOR; i++) {
					if (Native::GetNumVehicleMods(Veh, i) > 0) {
						Native::SetVehicleMod(SpawnedVehicle, i, Native::GetVehicleMod(Veh, i), 0);
					}
				}

				for (int i = MOD_PLATEHOLDER; i <= MOD_LIVERY; i++) {
					if (Native::GetNumVehicleMods(Veh, i) > 0) {
						Native::SetVehicleMod(SpawnedVehicle, i, Native::GetVehicleMod(Veh, i), 0);
					}
				}

				Native::SetVehicleWheelType(SpawnedVehicle, Native::GetVehicleWheelType(Veh));
				Native::SetVehicleMod(SpawnedVehicle, MOD_FRONTWHEELS, Native::GetVehicleMod(Veh, MOD_FRONTWHEELS), Native::GetVehicleModVariation(Veh, MOD_FRONTWHEELS));
				Native::SetVehicleMod(SpawnedVehicle, MOD_BACKWHEELS, Native::GetVehicleMod(Veh, MOD_BACKWHEELS), Native::GetVehicleModVariation(Veh, MOD_FRONTWHEELS));

				for (int i = MOD_UNK17; i <= MOD_XENONLIGHTS; i++) {
					Native::ToggleVehicleMod(SpawnedVehicle, Native::GetVehicleModVariation(Veh, i), true);
				}

				int Colors[2];
				int ExtraColors[2];
				Color TyreSmoke;
				Color CustomPrimary;
				Color CustomSecondary;
				Color Neon;

				Native::GetVehicleCustomPrimaryColour(Veh, &CustomPrimary.r, &CustomPrimary.g, &CustomPrimary.b);
				Native::GetVehicleCustomSecondaryColour(Veh, &CustomSecondary.r, &CustomSecondary.g, &CustomSecondary.b);
				Native::_GetVehicleNeonLightsColour(Veh, &Neon.r, &Neon.g, &Neon.b);
				Native::GetVehicleTyreSmokeColor(Veh, &TyreSmoke.r, &TyreSmoke.g, &TyreSmoke.b);
				Native::GetVehicleColours(Veh, &Colors[0], &Colors[1]);
				Native::GetVehicleExtraColours(Veh, &ExtraColors[0], &ExtraColors[1]);

				Native::SetVehicleColours(SpawnedVehicle, Colors[0], Colors[1]);
				Native::SetVehicleExtraColours(SpawnedVehicle, ExtraColors[0], ExtraColors[1]);
				Native::SetVehicleWindowTint(SpawnedVehicle, Native::GetVehicleWindowTint(Veh));

				Native::SetVehicleTyreSmokeColor(SpawnedVehicle, TyreSmoke.r, TyreSmoke.g, TyreSmoke.b);
				Native::SetVehicleCustomPrimaryColour(SpawnedVehicle, CustomPrimary.r, CustomPrimary.g, CustomPrimary.b);
				Native::SetVehicleCustomSecondaryColour(SpawnedVehicle, CustomSecondary.r, CustomSecondary.g, CustomSecondary.b);

				for (int i = 0; i < 4; i++) {
					Native::_SetVehicleNeonLightEnabled(SpawnedVehicle, i, Native::_IsVehicleNeonLightEnabled(Veh, i));
				}

				Native::_SetVehicleNeonLightsColour(SpawnedVehicle, Neon.r, Neon.g, Neon.b);
				Native::SetVehicleNumberPlateTextIndex(SpawnedVehicle, Native::GetVehicleNumberPlateTextIndex(Veh));
				Native::SetVehicleNumberPlateText(SpawnedVehicle, Native::GetVehicleNumberPlateText(Veh));
			}
		}

		return SpawnedVehicle;
	}
	void Deploy() {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			if (Native::GetEntityHeightAboveGround(Menu::GetLocalPlayer().m_Vehicle) > 5.f) {
				if (Native::IsThisModelAHeli(Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle))) return;

				uint64_t Address = Engine::GetEntityAddress(Menu::GetLocalPlayer().m_Vehicle);
				if (Address) {
					uint64_t VehicleModelInfo = *(uint64_t*)(Address + 0x20);
					if (VehicleModelInfo) {
						*(uint8_t*)(VehicleModelInfo + 0x58C) |= 1;
					}

					*(bool*)(Address + 0x1A5D) = true;
					Native::_SetVehicleParachuteActive(Menu::GetLocalPlayer().m_Vehicle, true);
				}
			}
		}
	}
}

void VehicleMenu::Run() {
	Framework::addSubmenu("Vehicle", "vehicle-menu", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Movement")
			.setTarget("vehicle-movement"));

		core->addOption(Framework::Options::SubmenuOption("Doors")
			.setTarget("vehicle-doors"));

		core->addOption(Framework::Options::ToggleOption("Godmode")
			.addToggle(&m_Vars.m_GodMode));

		core->addOption(Framework::Options::ToggleOption("Invisibility")
			.addToggle(&m_Vars.m_Invisibility));

		core->addOption(Framework::Options::ToggleOption("Seatbelt")
			.addToggle(&m_Vars.m_Seatbelt));

		core->addOption(Framework::Options::ToggleOption("Instant Enter")
			.addToggle(&m_Vars.m_InstantlyEnter));

		core->addOption(Framework::Options::ToggleOption("Instant Exit")
			.addToggle(&m_Vars.m_InstantlyExit));

		core->addOption(Framework::Options::ButtonOption("Max Upgrades")
			.addClick([] { MaxUpgrades(Menu::GetLocalPlayer().m_Vehicle); }));

		core->addOption(Framework::Options::ButtonOption("Downgrade")
			.addClick([] { Downgrade(Menu::GetLocalPlayer().m_Vehicle); }));

		core->addOption(Framework::Options::ButtonOption("Clone")
			.addClick([] { CloneVehicle(Menu::GetLocalPlayer().m_Vehicle); }));
		});

	Framework::addSubmenu("Doors", "vehicle-doors", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ButtonOption("Open All Doors")
			.addClick([] {
				Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, 0, true, true);
				Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, 1, true, true);
				Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, 2, true, true);
				Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, 3, true, true);
				Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, 4, true, true);
				Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, 5, true, true);
				Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, 6, true, true);
				Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, 7, true, true);
				}));

		core->addOption(Framework::Options::ButtonOption("Close All Doors")
			.addClick([] { Native::SetVehicleDoorShut(Menu::GetLocalPlayer().m_Vehicle, true, true); }));

		core->addOption(Framework::Options::ButtonOption("Open Front Left Door")
			.addClick([] { Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, 0, true, true); }));

		core->addOption(Framework::Options::ButtonOption("Open Front Right Door")
			.addClick([] { Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, 1, true, true); }));

		core->addOption(Framework::Options::ButtonOption("Open Back Left Door")
			.addClick([] { Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, 2, true, true); }));

		core->addOption(Framework::Options::ButtonOption("Open Back Right Door")
			.addClick([] { Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, 3, true, true); }));
		});

	Framework::addSubmenu("Movement", "vehicle-movement", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Horn Boost")
			.setTarget("movement-hornboost"));

		core->addOption(Framework::Options::SubmenuOption("Parachute")
			.setTarget("movement-parachute"));

		core->addOption(Framework::Options::ToggleOption("Instant Handbrake")
			.addToggle(&m_Vars.m_HandBrake));

		core->addOption(Framework::Options::ToggleOption("Bypass Max Speed")
			.addToggle(&m_Vars.m_DisableMaxSpeed));

		core->addOption(Framework::Options::ToggleOption("No Turbulence")
			.addToggle(&m_Vars.m_NoTurbulence));

		core->addOption(Framework::Options::ToggleNumberOption<float>("Flying Vehicle")
			.addToggle(&m_Vars.m_FlyingVehicle)
			.addNumber(&m_Vars.m_FlyingVehicleSpeed).addStep(0.01f).setPrecision(2.f).addMin(0.5f).addMax(1000.f)
			.addClick([] { if (!m_Vars.m_FlyingVehicle) if (Menu::GetLocalPlayer().m_InVehicle) Native::SetEntityCollision(Menu::GetLocalPlayer().m_Vehicle, true, true); }));
	});

	Framework::addSubmenu("Parachute", "movement-parachute", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Auto Deploy")
			.addToggle(&m_Vars.m_AutoDeploy));

		core->addOption(Framework::Options::ButtonOption("Deploy")
			.addClick([] {
				Deploy();
			}));
	});

	Framework::addSubmenu("Horn Boost", "movement-hornboost", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Toggle")
			.addToggle(&m_Vars.m_HornBoost));

		core->addOption(Framework::Options::NumberOption<int>("Force Multiplier")
			.addNumber(&m_Vars.m_BoostSpeed).addMin(0).addMax(1000));
	});
}

bool IsKeyPressed(std::uint16_t key) {
	if (GetForegroundWindow() == Core::g_GameWindow) {
		if (GetAsyncKeyState(key) & 0x8000) {
			return true;
		}
	}
	return false;
}
Math::Vector3<float> VehicleMenu::GetCameraDirection(Math::Vector3<float> Rotation) {
	Math::Vector3<float> Tmp;
	Tmp.m_Y = Rotation.m_Z * 0.0174532924f;
	Tmp.m_X = Rotation.m_X * 0.0174532924f;
	Tmp.m_Z = abs(cos(Tmp.m_X));

	return Math::Vector3<float>((-sin(Tmp.m_Y)) * Tmp.m_Z, (cos(Tmp.m_Y)) * Tmp.m_Z, sin(Tmp.m_X));
}
void VehicleMenu::Update() {
	Run();

	if (m_Vars.m_InstantlyEnter && !Native::IsPedInAnyVehicle(Menu::GetLocalPlayer().m_Ped, FALSE))
	{
		if (Native::GetIsTaskActive(Menu::GetLocalPlayer().m_Ped, 160))
		{
			const auto Vehicle = Native::GetVehiclePedIsTryingToEnter(Menu::GetLocalPlayer().m_Ped);
			const auto Index = Native::GetSeatPedIsTryingToEnter(Menu::GetLocalPlayer().m_Ped);
			Native::TaskEnterVehicle(Menu::GetLocalPlayer().m_Ped, Vehicle, 1, Index, 1.f, 16, 0);
		}
	}

	if (m_Vars.m_InstantlyExit && Native::IsPedInAnyVehicle(Menu::GetLocalPlayer().m_Ped, FALSE))
	{
		Native::DisableControlAction(0, INPUT_VEH_EXIT, TRUE);
		if (Native::IsDisabledControlPressed(0, INPUT_VEH_EXIT)) {
			Native::ClearPedTasks(Menu::GetLocalPlayer().m_Ped);
			Native::ClearPedTasksImmediately(Menu::GetLocalPlayer().m_Ped);
			Native::ClearPedSecondaryTask(Menu::GetLocalPlayer().m_Ped);
		}
	}

	if (m_Vars.m_AutoDeploy) {
		Deploy();
	}

	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_FlyingVehicle) {
			Native::SetEntityCollision(Menu::GetLocalPlayer().m_Vehicle, false, true);
			Native::FreezeEntityPosition(Menu::GetLocalPlayer().m_Vehicle, false);

			if (IsKeyPressed(0x57) || Native::IsControlPressed(false, ControlAttack)) {
				Math::Vector3<float> Coords = Menu::GetLocalPlayer().m_Coords;// + (GetCameraDirection() * m_Vars.m_FlyingVehicleSpeed);
				Math::Vector3<float> CameraRotation = Native::GetGameplayCamRot(2);

				Native::SetEntityCoords(Menu::GetLocalPlayer().m_Vehicle, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, true, false);
				Native::SetEntityRotation(Menu::GetLocalPlayer().m_Vehicle, CameraRotation.m_X, CameraRotation.m_Y, CameraRotation.m_Z, 2, true);
			}
		}

		if (m_Vars.m_DisableMaxSpeed) {
			BypassMaxSpeed();
		}

		if (m_Vars.m_LimitMaxSpeed) {
			LimitMaxSpeed();
		}
	}
	static bool Reset = false;
	if (m_Vars.m_HornBoost) {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			auto Veh = Menu::GetLocalPlayer().m_Vehicle;
			if (Native::IsHornActive(Veh)) {
				Native::_SetVehicleEnginePowerMultiplier(Menu::GetLocalPlayer().m_Vehicle, 10.f);

				uint64_t Address = Engine::GetEntityAddress(Veh);
				if (Address) {
					*(float*)(Address + 0x1360) = 30.f;
				}

				Boost(Veh, true);

				Reset = true;
			}
			else {
				if (Reset) {
					uint64_t Address = Engine::GetEntityAddress(Veh);
					if (Address) {
						*(float*)(Address + 0x1360) = 1.f;
					}

					Reset = false;
				}
			}
		}
	}

	if (m_Vars.m_GodMode) {
		Native::SetEntityInvincible(Menu::GetLocalPlayer().m_Vehicle, true);
		Native::SetEntityProofs(Menu::GetLocalPlayer().m_Vehicle, true, true, true, true, true, true, true, true);
		Native::SetVehicleCanBeVisiblyDamaged(Menu::GetLocalPlayer().m_Vehicle, false);
		Native::SetVehicleEngineOn(Menu::GetLocalPlayer().m_Vehicle, true, true, true);
	}
	else {
		Native::SetEntityInvincible(Menu::GetLocalPlayer().m_Vehicle, false);
		Native::SetEntityProofs(Menu::GetLocalPlayer().m_Vehicle, false, false, false, false, false, false, false, false);
		Native::SetVehicleCanBeVisiblyDamaged(Menu::GetLocalPlayer().m_Vehicle, true);
		Native::SetVehicleEngineOn(Menu::GetLocalPlayer().m_Vehicle, false, false, false);
	}

	if (m_Vars.m_Invisibility) {
		Native::SetEntityVisible(Menu::GetLocalPlayer().m_Vehicle, false, false);
	}
	else {
		Native::SetEntityVisible(Menu::GetLocalPlayer().m_Vehicle, true, true);
	}

	if (m_Vars.m_Seatbelt) {
		Native::SetPedCanBeKnockedOffVehicle(Menu::GetLocalPlayer().m_Ped, 1);
	}
	else {
		Native::SetPedCanBeKnockedOffVehicle(Menu::GetLocalPlayer().m_Ped, 0);
	}
	if (m_Vars.m_NoTurbulence)
	{
		if (Native::IsThisModelAPlane(Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle))) {
			Native::_0xAD2D28A1AFDFF131(Menu::GetLocalPlayer().m_Vehicle, 0.f);
		}
	}
	if (m_Vars.m_HandBrake) {
		if (Native::IsPedInAnyVehicle(Menu::GetLocalPlayer().m_Ped, FALSE) && Native::IsControlPressed(0, INPUT_VEH_HANDBRAKE))
			Native::SetVehicleForwardSpeed(Native::GetVehiclePedIsIn(Menu::GetLocalPlayer().m_Ped, FALSE), 0.f);
	}
}