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
}

void VehicleMenu::Run() {
	Framework::addSubmenu("Vehicle", "vehicle-menu", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Godmode")
			.addToggle(&m_Vars.m_GodMode));

		core->addOption(Framework::Options::ToggleOption("Invisibility")
			.addToggle(&m_Vars.m_Invisibility));

		core->addOption(Framework::Options::ToggleOption("Seatbelt")
			.addToggle(&m_Vars.m_Seatbelt));

		core->addOption(Framework::Options::ButtonOption("Max Upgrades")
			.addClick([] { MaxUpgrades(Menu::GetLocalPlayer().m_Vehicle); }));

		core->addOption(Framework::Options::ButtonOption("Downgrade")
			.addClick([] { Downgrade(Menu::GetLocalPlayer().m_Vehicle); }));

		core->addOption(Framework::Options::ButtonOption("Clone")
			.addClick([] { CloneVehicle(Menu::GetLocalPlayer().m_Vehicle); }));
	});
}

void VehicleMenu::Update() {
	Run();

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
}