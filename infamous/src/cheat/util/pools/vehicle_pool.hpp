#pragma once
#include "stdafx.hpp"
#include "rage/types/base_types.hpp"

namespace Menu::Pools {
	class VehiclePool {
	public:
		void Update();
		void Run(std::function<void(Vehicle)> Callback, bool RequestControl = false);
		void RunLocal(std::function<void(Vehicle)> Callback);
		std::vector<Vehicle>& GetStorage() { return m_Storage; }
	private:
		std::vector<Vehicle> m_Storage;
	};

	VehiclePool* GetVehiclePool();
}