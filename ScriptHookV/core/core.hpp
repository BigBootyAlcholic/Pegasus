#pragma once
#include "../stdafx.hpp"

namespace Core {
	extern void Initialize();
	extern void Loop();
	extern void UnInitialize(HMODULE hmod);
}
namespace Thread {
	extern DWORD Entry(LPVOID param);
	extern BOOL Create(HMODULE hmod, DWORD callReason);
}