#include "stdafx.hpp"
#include "fiber_pool.hpp"
#include "fiber.hpp"

namespace Utils {
	void FiberPool::Initialize() {
		static std::function<void()> Job1;
		static std::function<void()> Job2;
		static std::function<void()> Job3;
		static std::function<void()> Job4;
		static std::function<void()> Job5;
		static std::function<void()> Job6;
		static std::function<void()> Job7;
		static std::function<void()> Job8;

		GetFiberManager()->Add("P1", [] { GetFiberPool()->Update(Job1); });
		GetFiberManager()->Add("P2", [] { GetFiberPool()->Update(Job2); });
		GetFiberManager()->Add("P3", [] { GetFiberPool()->Update(Job3); });
		GetFiberManager()->Add("P4", [] { GetFiberPool()->Update(Job4); });
		GetFiberManager()->Add("P5", [] { GetFiberPool()->Update(Job5); });
		GetFiberManager()->Add("P6", [] { GetFiberPool()->Update(Job6); });
		GetFiberManager()->Add("P7", [] { GetFiberPool()->Update(Job7); });
		GetFiberManager()->Add("P8", [] { GetFiberPool()->Update(Job8); });
	}

	void FiberPool::Update(std::function<void()>& Func) {
		if (m_Jobs.empty()) return;
		Func = m_Jobs.front();
		m_Jobs.pop();
		Func();
	}

	void FiberPool::Push(std::function<void()> Func) {
		if (m_Jobs.size() >= 200) {
			return;
		}

		m_Jobs.emplace(Func);
	}

	FiberPool* GetFiberPool() {
		static FiberPool Instance;
		return &Instance;
	}
}