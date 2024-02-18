#include "stdafx.hpp"
#include "fiber.hpp"
#include "util/caller.hpp"
#include "core/core.hpp"
namespace Utils {
	void FiberManager::Initialize() {
		if (m_Initialized) return;
		m_Initialized = true;

		ConvertThreadToFiber(nullptr);
		m_MainFiberHandle = GetCurrentFiber();
	}

	void FiberManager::Update() {
		if (!m_Initialized || !m_CurrentFiber) return;

		for (auto& Fiber : m_Fibers) {
			if (Fiber.m_Running) {
				if (timeGetTime() >= Fiber.m_WakeTime) {
					SwitchToFiber(Fiber.m_FiberHandle);
				}
			}
		}

		m_CurrentFiber = m_CurrentFiber->m_NextFiber;
	}

	void* FiberManager::Add(std::string Name, void(*Callback)()) {
		uint64_t* Arguments = new uint64_t[2];
		Arguments[0] = (uint64_t)Callback;
		Arguments[1] = (uint64_t)m_MainFiberHandle;

		void* FiberHandle = CreateFiber(0, [](void* Param) {
			uint64_t* Args = (uint64_t*)Param;

			while (Core::g_Running) {
				Caller::Call<int>(Args[0]);
				SwitchToFiber((void*)Args[1]);
			}

			delete[] Args;
			}, Arguments);

		m_Fibers.push_back({ nullptr, FiberHandle, timeGetTime(), true, Name });

		for (auto Fiber = m_Fibers.begin(); Fiber != m_Fibers.end(); ++Fiber) {
			auto Next = Fiber + 1;
			if (Next != m_Fibers.end()) Fiber->m_NextFiber = &*Next;
			else Fiber->m_NextFiber = &m_Fibers.front();
		}

		m_CurrentFiber = &m_Fibers.front();
		return FiberHandle;
	}

	void FiberManager::Sleep(uint32_t Time) {
		auto Condition = std::find_if(m_Fibers.begin(), m_Fibers.end(), [](Fiber& f) -> bool { return f.m_FiberHandle == GetCurrentFiber(); });
		if (Condition != m_Fibers.end()) {
			Condition->m_WakeTime = timeGetTime() + Time;
			SwitchToFiber(m_MainFiberHandle);
		}
	}

	void FiberManager::Unload() {
		for (auto& Fiber : m_Fibers) {
			Fiber.m_Running = false;
			DeleteFiber(Fiber.m_FiberHandle);
		}
	}

	void FiberManager::GoToMainFiber() {
		SwitchToFiber(m_MainFiberHandle);
	}

	FiberManager* GetFiberManager() {
		static FiberManager Instance;
		return &Instance;
	}

	FiberManager* GetScriptHookFiberManager() {
		static FiberManager Instance;
		return &Instance;
	}
}