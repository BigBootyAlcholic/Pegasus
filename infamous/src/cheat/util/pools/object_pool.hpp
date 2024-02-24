#pragma once
#include "stdafx.hpp"
#include "rage/types/base_types.hpp"

namespace Menu::Pools {
	class ObjectPool {
	public:
		void Update();
		void Run(std::function<void(Object)> Callback);
		std::vector<Object>& GetStorage() { return m_Storage; }
	private:
		std::vector<Object> m_Storage;
	};

	ObjectPool* GetObjectPool();
}