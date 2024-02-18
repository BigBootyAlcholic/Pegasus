#pragma once

namespace Core::Init {
	DWORD WINAPI Start(LPVOID handle);
	DWORD WINAPI Cleanup(LPVOID handle);
}