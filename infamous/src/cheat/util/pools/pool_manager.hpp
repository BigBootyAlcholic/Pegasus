#pragma once
#include "rage/engine.hpp"
#include "util/math.hpp"
#include "ped_pool.hpp"
#include "vehicle_pool.hpp"
#include "object_pool.hpp"
/*#include "pickup_pool.hpp"
#include "attachment_pool.hpp"
#include "net_event_queue_pool.hpp"*/

namespace Menu::Pools {
	class PoolManager {
	public:
		void Initialize();

		uint64_t GetPedClass() { return m_PedClass; }
		uint64_t GetObjectClass() { return m_ObjectClass; }
		uint64_t GetVehicleClass() { return m_VehicleClass; }
		uint64_t GetPickupClass() { return m_PickupClass; }
	private:
		uint64_t m_PedClass;
		uint64_t m_ObjectClass;
		uint64_t m_VehicleClass;
		uint64_t m_PickupClass;
	};

	PoolManager* GetPoolManager();
}