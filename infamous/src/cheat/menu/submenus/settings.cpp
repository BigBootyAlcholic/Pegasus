#include "stdafx.hpp"
#include "settings.hpp"
#include "framework/framework.hpp"
#include "cheat/util/texture.hpp"
void SettingsMenu::Run() {
	Framework::addSubmenu("Settings", "settings-menu", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::NumberOption<float>("Horizontal Position")
			.addNumber(&Framework::GetFrameWork()->m_Position.m_X).addMin(0).addMax(1)
			.addStep(0.01f).setPrecision(2));

		core->addOption(Framework::Options::NumberOption<float>("Vertical Position")
			.addNumber(&Framework::GetFrameWork()->m_Position.m_Y).addMin(0).addMax(1)
			.addStep(0.01f).setPrecision(2));

		core->addOption(Framework::Options::NumberOption<float>("Wrap")
			.addTooltip("Hotkeys Supported. Press F12 to bind. Test test estsetssssssss testestes tse setse tsetes tse test sets etset estset setsetsetests").addNumber(&Framework::GetFrameWork()->m_Wrap).addMin(-1).addMax(1)
			.addStep(0.001f).setPrecision(3));
		core->addOption(Framework::Options::ButtonOption("Unload")
			.addClick([] { Core::g_Running = false; }));
	});
}

void SettingsMenu::Update() {
	Run();
}