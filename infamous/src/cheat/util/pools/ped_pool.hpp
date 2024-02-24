#pragma once
#include "stdafx.hpp"
#include "rage/types/base_types.hpp"

namespace Menu::Pools {
	class PedPool {
	public:
		void Update();
		void Run(std::function<void(Ped)> Callback, bool RequestControl = false);
		std::vector<Ped>& GetStorage() { return m_Storage; }
	private:
		std::vector<Ped> m_Storage;
	};

	PedPool* GetPedPool();
}