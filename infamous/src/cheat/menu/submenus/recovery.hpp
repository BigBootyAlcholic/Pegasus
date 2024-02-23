#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "framework/options/number.hpp"
#include "core/core.hpp"


class RecoveryMenu {
public:
	void Run();
	void Update();
};

static RecoveryMenu* GetRecoveryMenu() {
	static RecoveryMenu instance;
	return &instance;
}

namespace RecoveryMenuVars {
	struct Vars_ {
		int m_SelectedCharacter;
		bool m_MillionsLoop;
		bool m_MillionsLoopV2;
		bool m_KLoop;
	};

	extern Vars_ m_Vars;
}
