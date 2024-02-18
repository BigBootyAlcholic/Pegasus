#pragma once
#include "stdafx.hpp"

namespace Core {

	class CCore {
	public:
		void Init();
		void Cleanup();
	};

	inline CCore* GetCore() {
		static CCore Instance;
		return &Instance;
	}

	
	/* core variables
	inline to make compiler happy */
	inline bool g_Running = false;
	inline HMODULE g_ModuleHandle;
	inline HWND g_GameWindow;
	inline u64 g_MenuAddress;
	inline u64 g_MenuSize;
	inline u64 g_GameAddress;
	inline u64 g_GameSize;
	
}