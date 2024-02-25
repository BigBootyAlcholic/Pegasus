#include "../stdafx.hpp"
#include "core.hpp"
#include "joaat.hpp"
#include "exports/exports.hpp"
namespace core {
	void initiInitializealize() {
	}
	void Loop() {
		std::filesystem::path path{ std::getenv("appdata") };
		path /= "Pegasus";
		path /= "Cache";
		path /= "Unload.txt";
		std::ifstream file{};
		while (Global::g_Running) {
			file.open(path);
			if (std::filesystem::exists(path) && file.is_open()) {
				file.close();
				std::filesystem::remove(path);
				Global::g_Running = false;
			}
			Sleep(5000);
		}
	}
	void UnInitialize(HMODULE hmod) {
		FreeLibraryAndExitThread(hmod, NULL);
	}
}
namespace Thread {
	DWORD Entry(LPVOID param) {
		Core::Initialize();
		Core::Loop();
		Core::UnInitialize((HMODULE)param);
		return 0;
	}
}