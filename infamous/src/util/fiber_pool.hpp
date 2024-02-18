#pragma once
#include "stdafx.hpp"
#include <queue>

namespace Utils {
	class FiberPool {
	public:
		void Initialize();
		void Update(std::function<void()>& Func);
		void Push(std::function<void()> Func);
	private:
		std::queue<std::function<void()>> m_Jobs;
	};

	FiberPool* GetFiberPool();
}