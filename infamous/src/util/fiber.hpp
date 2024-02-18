#pragma once
#include "stdafx.hpp"

namespace Utils {
	struct Fiber {
		Fiber* m_NextFiber;
		void* m_FiberHandle;
		uint32_t m_WakeTime;
		bool m_Running = true;
		std::string m_Name;
	};

	class FiberManager {
	public:
		void Initialize();
		void Update();
		void* Add(std::string Name, void(*Callback)());
		void Sleep(uint32_t Time);
		void Unload();
		void GoToMainFiber();

		std::vector<Fiber>& GetFibers() { return m_Fibers; }
	private:
		bool m_Initialized;
		void* m_MainFiberHandle;
		Fiber* m_CurrentFiber;
		std::vector<Fiber> m_Fibers;
	};

	FiberManager* GetFiberManager();
	FiberManager* GetScriptHookFiberManager();

	static void WaitFiber(uint32_t Time) {
		GetFiberManager()->Sleep(Time);
	}
}