#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/number.hpp"
#include "core/core.hpp"

class SettingsMenu {
public:
	void Run();
	void Update();
};

static SettingsMenu* GetSettingsMenu() {
	static SettingsMenu instance;
	return &instance;
}
