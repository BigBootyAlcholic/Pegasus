#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "core/core.hpp"


class MainMenu {
public:
	void Run();
	void Update();
};

static MainMenu* GetMainMenu() {
	static MainMenu instance;
	return &instance;
}

inline void Run() {
	GetMainMenu()->Run();
}
namespace MainMenuVars {
	struct Vars_ {
		bool m_Crash = false;
		bool m_TestLoopV2;
		bool m_SeamlessJoin = false;


	};

	extern Vars_ m_Vars;
}
