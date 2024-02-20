#include "stdafx.hpp"
#include "network_spoofing.hpp"
#include "cheat/util/player_manager.hpp"
#include "cheat/util/timer.hpp"
#include "cheat/util/global.hpp"
#include "util/fiber.hpp"
#include "util/fiber_pool.hpp"
using namespace NetworkSpoofingMenuVars;
using namespace Framework::Options;
namespace NetworkSpoofingMenuVars {
	Vars_ m_Vars;

	const char* textSettingsLabels[] = {
	"Normal",
	"Bold",
	"Italic"
	};

	const char* textSettingsValues[] = {
		"",
		"~h~",
		"~italic~"
	}; int textSettingsId = 0;

	const char* textColorsLabels[] = {
		"Default",
		"Black",
		"Blue",
		"Gold",
		"Green",
		"Grey",
		"Light Blue",
		"Orange",
		"Purple",
		"Red"
	};

	const char* textColorsValues[] = {
		"",
		"~v~",
		"~b~",
		"~y~",
		"~g~",
		"~c~",
		"~f~",
		"~o~",
		"~p~",
		"~r~"
	}; int textColorsId = 0;

	const char* textIconsLabels[] = {
		"None",
		"Wanted Star",
		"Rockstar Verified",
		"Rockstar"
	};

	const char* textIconsValues[] = {
		"",
		"~ws~",
		"~¦~",
		"~÷~"
	}; int textIconsId = 0;

	static bool active = false;
	inline static void SpoofName() {
		Utils::GetFiberPool()->Push([] {
			Native::DisplayOnscreenKeyboard(0, "Input", "", "", "", "", "", 100);
			while (Native::UpdateOnscreenKeyboard() == 0) {
				active = true;
				Framework::GetFrameWork()->ResetKeys();
				Utils::GetFiberManager()->GoToMainFiber();
			}
			active = false;
			if (!Native::GetOnscreenKeyboardResult())
				return;
			m_Vars.m_Name = Native::GetOnscreenKeyboardResult();
			});
	}

}

void NetworkSpoofingMenu::Run() {
	Framework::addSubmenu("Spoofing", "network-spoofing", [](Framework::Options::Core* core) {
		core->addOption(SubmenuOption("Name")
			.setTarget("spoofing-name"));

		core->addOption(SubmenuOption("Rotation")
			.setTarget("player-rotation"));

		core->addOption(SubmenuOption("Vehicle Rotation")
			.setTarget("vehicle-rotation"));

		core->addOption(ToggleOption("Movement Animation")
			.addToggle(&m_Vars.m_MoveAnimation));

		core->addOption(ToggleOption("Hide From Playerlist")
			.addToggle(&m_Vars.m_HideFromPlayerlist));
	});

	Framework::addSubmenu("Name", "spoofing-name", [](Framework::Options::Core* core) {
		const char* og_name = Native::GetPlayerName(Native::PlayerId());

		core->addOption(KeyboardOption("Input Name")
			.addClick([=] { SpoofName(); }));

		core->addOption(ButtonOption("Reset to Default")
			.addClick([=] { m_Vars.m_SpoofName = false; m_Vars.m_Name = og_name; }));

		std::string preview = std::format("Apply: {}{}{}{}", textSettingsValues[textSettingsId], textColorsValues[textColorsId], textIconsValues[textIconsId], m_Vars.m_Name.c_str());
		std::string name_to_apply = std::format("{}{}{}{}", textSettingsValues[textSettingsId], textColorsValues[textColorsId], textIconsValues[textIconsId], m_Vars.m_Name.c_str());

		core->addOption(ButtonOption(preview.c_str())
			.addClick([=] { m_Vars.m_SpoofName = true; m_Vars.m_Name = name_to_apply; }));

		core->addOption(BreakOption("Settings"));

		core->addOption(scrollOption<const char*, int>("Color")
			.addScroll(&textColorsLabels).setPosition(&textColorsId));

		core->addOption(scrollOption<const char*, int>("Style")
			.addScroll(&textSettingsLabels).setPosition(&textSettingsId));

		core->addOption(scrollOption<const char*, int>("Icon")
			.addScroll(&textIconsLabels).setPosition(&textIconsId));

	});

	Framework::addSubmenu("Vehicle Rotation", "vehicle-Rotation", [](Framework::Options::Core* core) {
		core->addOption(ToggleNumberOption<float>("Spinbot")
			.addToggle(&m_Vars.m_VehSpinbot).addNumber(&m_Vars.m_VehSpinbotSpeed)
			.addMin(0.f).addMax(100.f).addStep(1.f).setPrecision(0.f));

		core->addOption(ToggleOption("Jitter")
			.addToggle(&m_Vars.m_VehJitter));

		core->addOption(ToggleOption("Relative")
			.addToggle(&m_Vars.m_VehRelative)
			.addTooltip("Current rotation will be added to new rotation"));
	});

	Framework::addSubmenu("Rotation", "player-Rotation", [](Framework::Options::Core* core) {
		core->addOption(ToggleNumberOption<float>("Spinbot")
			.addToggle(&m_Vars.m_PedSpinbot).addNumber(&m_Vars.m_PedSpinbotSpeed)
			.addMin(0.f).addMax(14.f).addStep(1.f).setPrecision(0.f)
			.addClick([] { m_Vars.m_PedFaceDirection = false; m_Vars.m_PedJitter = false; }));

		core->addOption(ToggleOption("Jitter")
			.addToggle(&m_Vars.m_PedJitter)
			.addClick([ ]{ m_Vars.m_PedSpinbot = false; m_Vars.m_PedFaceDirection = false; }));

		core->addOption(ToggleOption("Relative")
			.addToggle(&m_Vars.m_PedRelative)
			.addTooltip("Current rotation will be added to new rotation"));
	});
}


void NetworkSpoofingMenu::Update() {
	Run();
	
	if (m_Vars.m_PedSpinbot) {
		static float Value = 0.f;
		Value += m_Vars.m_PedSpinbotSpeed;

		if (Value > 180.f) Value = -180.f + (Value - 180.f);
		m_Vars.m_PedHookValue = { Value * 0.0174532924f, Value * 0.0174532924f };
		return;
	}

	if (m_Vars.m_PedJitter) {
		static float Value = 0.f;
		Value += 40.f;
		if (Value > 180.f) Value = -180.f + (Value - 180.f);

		m_Vars.m_PedHookValue = { Value * 0.0174532924f, Value * 0.0174532924f };
		return;
	}

	if (Menu::GetLocalPlayer().m_InVehicle) {
		m_Vars.m_VehHookValue.m_X = Menu::GetLocalPlayer().m_Rotation.m_X;
		m_Vars.m_VehHookValue.m_Y = Menu::GetLocalPlayer().m_Rotation.m_Y;
		m_Vars.m_VehHookValue.m_Z = Menu::GetLocalPlayer().m_Rotation.m_Z;

		/*if (m_Vars.m_Static) {
			if (m_Vars.m_StaticToggle.m_X) m_Vars.m_HookValue.m_X = m_Vars.m_StaticValue.m_X + (m_Vars.m_Relative ? Menu::GetLocalPlayer().m_Rotation.m_X : 0.f);
			if (m_Vars.m_StaticToggle.m_Y) m_Vars.m_HookValue.m_Y = m_Vars.m_StaticValue.m_Y + (m_Vars.m_Relative ? Menu::GetLocalPlayer().m_Rotation.m_Y : 0.f);
			if (m_Vars.m_StaticToggle.m_Z) m_Vars.m_HookValue.m_Z = m_Vars.m_StaticValue.m_Z + (m_Vars.m_Relative ? Menu::GetLocalPlayer().m_Rotation.m_Z : 0.f);
		}*/

		if (m_Vars.m_VehSpinbot) {
			static float Value = 0.f;
			Value += m_Vars.m_VehSpinbotSpeed;
			if (Value > 360.f) Value = (Value - 360.f);

			//switch (m_Vars.m_SpinbotAxis) {
			//case 0:
				m_Vars.m_VehHookValue.m_X = Value;
			//	break;

			//case 1:
			//	m_Vars.m_HookValue.m_Y = Value;
			//	break;

			//case 2:
			//	m_Vars.m_HookValue.m_Z = Value;
				//break;
			//}
		}

		if (m_Vars.m_VehJitter) {
			static float Value = 0.f;
			Value += Native::GetRandomFloatInRange(5.f, 100.f);
			if (Value > 360.f) Value = (Value - 360.f);

			//switch (m_Vars.m_JitterAxis) {
			//case 0:
				m_Vars.m_VehHookValue.m_X = Value;
			//	break;

			//case 1:
			//	m_Vars.m_HookValue.m_Y = Value;
			//	break;

			//case 2:
			//	m_Vars.m_HookValue.m_Z = Value;
			//	break;
			//}
		}

		/*if (m_Vars.m_GhostHandle && Native::DoesEntityExist(m_Vars.m_GhostHandle)) {
			/*Rage::Types::Vehicle* GhostAddress = (Rage::Types::Vehicle*)Rage::Engine::GetEntityAddress(m_Vars.m_GhostHandle);
			if (GhostAddress) {
				memcpy(GhostAddress->GetMatrix(), &m_Vars.m_GhostMatrix, sizeof(m_Vars.m_GhostMatrix));
			}*/

		//Native::AttachEntityToEntity(m_Vars.m_GhostHandle, Menu::GetLocalPlayer().m_Vehicle, 0, 0.0f, 0.0f, 0.0f, m_Vars.m_HookValue.m_X - Menu::GetLocalPlayer().m_Rotation.m_X, m_Vars.m_HookValue.m_Y - Menu::GetLocalPlayer().m_Rotation.m_Y, m_Vars.m_HookValue.m_Z - Menu::GetLocalPlayer().m_Rotation.m_Z, 0, 0, 0, 1, 1, 1);
//	}*/

		m_Vars.m_VehHookValue.m_X *= 0.0174532924f;
		m_Vars.m_VehHookValue.m_Y *= 0.0174532924f;
		m_Vars.m_VehHookValue.m_Z *= 0.0174532924f;
	}
	
}