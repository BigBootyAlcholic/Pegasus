#include "stdafx.hpp"
#include "vehicle_pool.hpp"
#include "rage/engine.hpp"
#include "rage/invoker/natives.hpp"
#include "cheat/util/player_manager.hpp"
#include "cheat/util/control_manager.hpp"
#include "core/core.hpp"
namespace Menu::Pools {
	void VehiclePool::Update() {
		std::vector<Vehicle> Vehicles;

		auto Pool = Patterns::Vars::g_VehiclePool;
		if (Pool) {
			if (Pool->m_Count > 4000) return;
			if (!Pool->m_Address) return;

			for (uint32_t i = 0; i < Pool->m_Count; i++) {
				if (Patterns::Vars::g_EntityPool) {
					if (Patterns::Vars::g_EntityPool->IsFull()) {
						break;
					}
				}

				uint64_t Address = Pool->GetEntity(i);
				if (Address) {
					if (*(uint64_t*)Address > Core::g_GameAddress) {
						Entity Handle = Engine::GetEntityHandleFromAddress(Address);
						if (Handle) {
							Vehicles.push_back(Handle);
						}
					}
				}
			}
		}

		for (std::size_t i = 0; i < Vehicles.size(); i++) {
			if (std::find(m_Storage.begin(), m_Storage.end(), Vehicles[i]) == m_Storage.end()) {
				m_Storage.push_back(Vehicles[i]);
			}
		}

		for (std::size_t i = 0; i < m_Storage.size(); i++) {
			if (!Native::DoesEntityExist(m_Storage[i])) {
				m_Storage.erase(m_Storage.begin() + i);
			}
		}
	}

	void VehiclePool::Run(std::function<void(Vehicle)> Callback, bool RequestControl) {
		Update();

		for (std::size_t i = 0; i < m_Storage.size(); i++) {
			if (m_Storage[i] != Menu::GetLocalPlayer().m_Entity) {
				if (RequestControl) {
					bool TakeOwner = true;
					Ped Driver = Native::GetPedInVehicleSeat(m_Storage[i], -1, 0);
					if (Driver) {
						if (Native::IsPedAPlayer(Driver)) {
							TakeOwner = false;
						}
					}

					Menu::GetControlManager()->RequestControl(m_Storage[i], [=](Entity E) {
						Callback(E);
						}, TakeOwner);
				}
				else {
					Callback(m_Storage[i]);
				}
			}
		}
	}

	void VehiclePool::RunLocal(std::function<void(Vehicle)> Callback) {
		Vehicle LocalVehicles[500];
		LocalVehicles[0] = 75;

		int Count = Native::GetPedNearbyVehicles(Menu::GetLocalPlayer().m_Ped, LocalVehicles);
		if (Count > 0) {
			for (int i = 0; i < Count; i++) {
				Vehicle Veh = LocalVehicles[i];
				if (Veh && Native::DoesEntityExist(Veh)) {
					Callback(Veh);
				}
			}
		}
	}

	VehiclePool* GetVehiclePool() {
		static VehiclePool Instance;
		return &Instance;
	}
}