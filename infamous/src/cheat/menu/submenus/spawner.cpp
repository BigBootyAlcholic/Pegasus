#include "stdafx.hpp"
#include "spawner.hpp"
#include "cheat/util/notify.hpp"
#include "cheat/util/control_manager.hpp"
#include "core/patterns.hpp"
using namespace SpawnerMenuVars;

namespace SpawnerMenuVars {
	Vars_ m_Vars;


	inline float deg_to_rad(float degs) {
		return degs * 3.141592653589793f / 180.f;
	}

	inline Vehicle spawn_vehicle(std::uint32_t hash) {
		if (!Native::IsModelValid(hash)) {
			return NULL;
		}
		Menu::GetControlManager()->RequestModel(hash, [](uint32_t ModelHash) {
			float forward = 5.f;
			Math::Vector3<float> coords = Native::GetEntityCoords(Native::PlayerPedId(), false);
			float heading = Native::GetEntityHeading(Native::PlayerPedId());
			float x = forward * sin(deg_to_rad(heading)) * -1.f;
			float y = forward * cos(deg_to_rad(heading));

			if (m_Vars.m_DeleteCurrent) {
			if (Native::IsPedInAnyVehicle(Native::PlayerPedId(), false)) {
				if (Native::GetPedInVehicleSeat(Native::GetVehiclePedIsIn(Native::PlayerPedId(), false), -1, 0) == Native::PlayerPedId()) {
					Native::SetEntityAsMissionEntity(Native::GetVehiclePedIsIn(Native::PlayerPedId(), false), true, true);
					Entity ent = Native::GetVehiclePedIsIn(Native::PlayerPedId(), false);
					Native::DeleteEntity(&ent);
				}
			}
			}

			*(unsigned short*)Patterns::Vars::g_SetNetworkedThread = 0x9090; // we set this thread as networked so we can spawn the vehicle / u can add rage classes and use excute under thread instead of this
			Vehicle the_vehicle = Native::CreateVehicle(ModelHash, coords.m_X + x, coords.m_Y + y, coords.m_Z, heading, Native::NetworkIsSessionActive(), false, false);
			*(unsigned short*)Patterns::Vars::g_SetNetworkedThread = 0x0574; // We restore it so we don't get detected 
			if (m_Vars.m_Fade) {
				Native::NetworkFadeInEntity(the_vehicle, true, false);
			}

			std::int32_t net_id = Native::VehToNet(the_vehicle);
			Native::SetNetworkIdExistsOnAllMachines(net_id, TRUE);

			if (m_Vars.m_Blip) {
					Blip blip = Native::AddBlipForEntity(the_vehicle);
					Native::SetBlipSprite(blip, 227);
					Native::SetBlipDisplay(blip, 8);
					Native::SetBlipCategory(blip, 4);
			}

			if (m_Vars.m_TeleportInside) {
				Native::SetPedIntoVehicle(Native::PlayerPedId(), the_vehicle, -1);
				Native::SetVehicleEngineOn(the_vehicle, true, true, false);
			}

			if (m_Vars.m_Godmode) {
				Native::SetEntityInvincible(the_vehicle, true);
				Native::SetEntityProofs(the_vehicle, true, true, true, true, true, true, true, true);
				Native::SetVehicleCanBeVisiblyDamaged(the_vehicle, false);
				Native::SetVehicleEngineOn(the_vehicle, true, true, true);
			}

			if (m_Vars.m_Notification) {
				Menu::Notify::stacked(std::format("{} {}", Native::_GetLabelText(Native::GetDisplayNameFromVehicleModel(ModelHash)), "Spawned").c_str());
			}

			bool is_air_veh = Native::IsThisModelAPlane(ModelHash) || Native::IsThisModelAHeli(ModelHash);

			if (m_Vars.m_AirVehiclesInAir && is_air_veh) {
				Native::SetEntityCoords(Native::NetToVeh(net_id), coords.m_X, coords.m_Y, coords.m_Z + m_Vars.m_SpawnHeight, false, false, false, false);
				Native::SetVehicleEngineOn(Native::NetToVeh(net_id), true, true, false);
			}

			if (Native::IsEntityVisibleToScript(Native::NetToVeh(net_id))) {
				Native::SetPedIntoVehicle(Native::PlayerPedId(), Native::NetToVeh(net_id), -1);
				float speed = Native::GetEntitySpeed(the_vehicle);
				Native::SetVehicleForwardSpeed(Native::NetToVeh(net_id), speed);
				Native::SetVehicleEngineOn(Native::NetToVeh(net_id), TRUE, TRUE, TRUE);
				Native::DecorSetInt(Native::NetToVeh(net_id), "MPBitset", (1 << 10));
				return the_vehicle;
			}
		});
		return NULL;
	}

}

void SpawnerMenu::Run() {
	Framework::addSubmenu("Spawner", "spawner-menu", [](Framework::Options::Core* core) {

		core->addOption(Framework::Options::SubmenuOption("Settings")
			.setTarget("vehicle-spawn-settings"));

		core->addOption(Framework::Options::BreakOption("Vehicles"));

		for (std::int32_t i = 0; i < 23; i++) {
			core->addOption(Framework::Options::SubmenuOption(get_vehicle_class_name(i))
				.addClick([=] { m_Vars.m_SelectedVehicleClass = i; })
				.setTarget(get_vehicle_class_name(m_Vars.m_SelectedVehicleClass)));
		}

	});

	Framework::addSubmenu("Settings", "vehicle-spawn-settings", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Notification")
			.addToggle(&m_Vars.m_Notification));

		core->addOption(Framework::Options::ToggleOption("Blip")
			.addToggle(&m_Vars.m_Blip));

		core->addOption(Framework::Options::ToggleOption("Particle FX")
			.addToggle(&m_Vars.m_ParticleFX));

		core->addOption(Framework::Options::ToggleOption("Teleport Inside")
			.addToggle(&m_Vars.m_TeleportInside));

		core->addOption(Framework::Options::ToggleOption("Godmode")
			.addToggle(&m_Vars.m_Godmode));

		core->addOption(Framework::Options::ToggleOption("Fade In")
			.addToggle(&m_Vars.m_Fade));

		core->addOption(Framework::Options::ToggleOption("Air Vehicles In Air")
			.addToggle(&m_Vars.m_AirVehiclesInAir));

		if (m_Vars.m_AirVehiclesInAir) {
			core->addOption(Framework::Options::NumberOption<float>("Air Vehicles Height")
				.addNumber(&m_Vars.m_SpawnHeight).addStep(1.f).setPrecision(0)
				.addMin(0.f).addMax(1000.f));
		}

		core->addOption(Framework::Options::ToggleOption("Delete Current")
			.addToggle(&m_Vars.m_DeleteCurrent));
	

	});

	Framework::addVehicleSubmenu(&m_Vars.m_SelectedVehicleClass, Rage::LowerCaseJoaat(get_vehicle_class_name(m_Vars.m_SelectedVehicleClass)), [](Framework::Options::VCore* core) {
		if (Patterns::Vars::g_GetHashList != nullptr) {
			for (std::int32_t i = 0; i < Patterns::Vars::g_GetHashList->m_Max; i++) {
				std::uint64_t info = Patterns::Vars::g_GetHashList->GetHashFromList(i);
				if (info != NULL) {
					if ((*(BYTE*)(info + 157) & 0x1F) == 5) {
						std::string make_ptr = (char*)((uintptr_t)info + 0x2A4);
						std::string model_ptr = (char*)((uintptr_t)info + 0x298);
						if (Native::GetVehicleClassFromName(Rage::LowerCaseJoaat(model_ptr)) == m_Vars.m_SelectedVehicleClass) {
							std::stringstream ss;
							std::string make(make_ptr);
							std::string model(model_ptr);
							if (make[0] || model[0]) {
								model = Native::_GetLabelText(model.c_str());
								if (model != "NULL") {
									ss << model;
								}
								else {
									ss << "Unknown";
								}
							}
							core->addOption(Framework::Options::VehicleOption((ss.str().c_str()))
								.addClick([=] { spawn_vehicle(*(std::uint32_t*)(info + 0x18)); }));
						}
					}
				}
			}
		}
		});
}


void SpawnerMenu::Update() {
	Run();
}