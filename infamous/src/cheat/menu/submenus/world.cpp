#include "stdafx.hpp"
#include "world.hpp"
#include "cheat/util/notify.hpp"
#include "cheat/util/control_manager.hpp"
#include "core/patterns.hpp"
#include "rage/engine.hpp"
using namespace WorldMenuVars;

namespace WorldMenuVars {
	Vars_ m_Vars;

	void SetTimeCycleVar(int index, float value) {
		Patterns::Vars::g_TimeCycleMgr->SetVar(index, value);
	}

	int GetTimeCycleVar(int index) {
		return Patterns::Vars::g_TimeCycleMgr->GetVar(index);
	}

	int r = 255, g = 0, b = 0;
	void RGBFade()
	{
		if (r > 0 && b == 0) {
			r--;
			g++;
		}
		if (g > 0 && r == 0) {
			g--;
			b++;
		}
		if (b > 0 && g == 0) {
			r++;
			b--;
		}
	}

	void RGBFadee()
	{
		if (m_Vars.m_PedR > 0 && m_Vars.m_PedB == 0) {
			m_Vars.m_PedR--;
			m_Vars.m_PedG++;
		}
		if (m_Vars.m_PedG > 0 && m_Vars.m_PedR == 0) {
			m_Vars.m_PedG--;
			m_Vars.m_PedB++;
		}
		if (m_Vars.m_PedB > 0 && m_Vars.m_PedG == 0) {
			m_Vars.m_PedR++;
			m_Vars.m_PedB--;
		}
	}
}

bool Test = false;

void WorldMenu::Run() {
	Framework::addSubmenu("World", "world-menu", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Timecycle Modifiers")
			.setTarget("tc_modifiers"));

		core->addOption(Framework::Options::SubmenuOption("Ped Color")
			.setTarget("ped_color"));
		});

	Framework::addSubmenu("Timecycle Modifiers", "tc_modifiers", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleNumberOption<float>("Width")
			.addToggle(&m_Vars.m_ShadowLight).addNumber(&m_Vars.m_ShadowLightVal)
			.addMin(-100.f).addMax(100.f).addStep(0.01f).setPrecision(2)
			.addClick([] {
				if (!m_Vars.m_ShadowLight) {
					SetTimeCycleVar(eTimeCycleVar::TCVAR_SHADOW_DISTANCE_MULT, 1.f);
				}
				}));

		core->addOption(Framework::Options::ToggleNumberOption<float>("Reflection")
			.addToggle(&m_Vars.m_Reflection).addNumber(&m_Vars.m_ReflectionVal)
			.addMin(-100.f).addMax(100.f).addStep(0.01f).setPrecision(2)
			.addClick([] {
				if (!m_Vars.m_ShadowLight) {
					SetTimeCycleVar(eTimeCycleVar::TCVAR_REFLECTION_HDR_MULT, 1.f);
				}
				}));

		core->addOption(Framework::Options::ToggleNumberOption<float>("Gamma")
			.addToggle(&m_Vars.m_Gamma).addNumber(&m_Vars.m_GammaVal)
			.addMin(-100.f).addMax(100.f).addStep(0.01f).setPrecision(2)
			.addClick([] {

				}));

		core->addOption(Framework::Options::ToggleNumberOption<float>("Motion Blur")
			.addToggle(&m_Vars.m_MotionBlur).addNumber(&m_Vars.m_MotionBlurVal)
			.addMin(-100.f).addMax(100.f).addStep(0.01f).setPrecision(2)
			.addClick([] {
				if (!m_Vars.m_MotionBlur) {
					SetTimeCycleVar(eTimeCycleVar::TCVAR_POSTFX_MOTIONBLUR_LENGTH, 1.f);
				}
				}));

		core->addOption(Framework::Options::ToggleNumberOption<float>("Desaturation")
			.addToggle(&m_Vars.m_Desaturation).addNumber(&m_Vars.m_DesaturationVal)
			.addMin(-100.f).addMax(100.f).addStep(0.01f).setPrecision(2)
			.addClick([] {
				if (!m_Vars.m_Desaturation) {
					SetTimeCycleVar(eTimeCycleVar::TCVAR_POSTFX_DESATURATION, 1.f);
				}
				}));
		});
	Framework::addSubmenu("Ped Color", "ped_color", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Toggle")
			.addToggle(&m_Vars.m_PedColor));

		core->addOption(Framework::Options::ToggleOption("Rainbow")
			.addToggle(&m_Vars.m_PedRainbow));

		core->addOption(Framework::Options::NumberOption<float>("Intensity")
			.addNumber(&m_Vars.m_PedMult).addMin(0.f).addMax(1000.f).addStep(1.f).setPrecision(0.f));

		core->addOption(Framework::Options::BreakOption("Color"));

		core->addOption(Framework::Options::NumberOption<int>("[R]ed")
			.addNumber(&m_Vars.m_PedR).addMin(0.f).addMax(255).addStep(1.f).setPrecision(0.f));

		core->addOption(Framework::Options::NumberOption<int>("[G]reen")
			.addNumber(&m_Vars.m_PedG).addMin(0.f).addMax(255).addStep(1.f).setPrecision(0.f));

		core->addOption(Framework::Options::NumberOption<int>("[B]lue")
			.addNumber(&m_Vars.m_PedB).addMin(0.f).addMax(255).addStep(1.f).setPrecision(0.f));
		});

}


void WorldMenu::Update() {
	Run();

	//SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_EAST_COL_B, 255);

	RGBFade();

	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_WEST_COL_R, (r / 255.f) * 1.5f);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_WEST_COL_G, (g / 255.f) * 1.5f);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_WEST_COL_B, (b / 255.f) * 1.5f);

	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_EAST_COL_R, (r / 255.f) * 1.5f);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_EAST_COL_G, (g / 255.f) * 1.5f);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_EAST_COL_B, (b / 255.f) * 1.5f);

	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_TRANSITION_COL_R, (r / 255.f) * 1.5f);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_TRANSITION_COL_G, (g / 255.f) * 1.5f);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_TRANSITION_COL_B, (b / 255.f) * 1.5f);

	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_ZENITH_COL_R, (r / 255.f) * 1.5f);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_ZENITH_COL_G, (g / 255.f) * 1.5f);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_ZENITH_COL_B, (b / 255.f) * 1.5f);

	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_ZENITH_TRANSITION_COL_R, (r / 255.f) * 1.5f);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_ZENITH_TRANSITION_COL_G, (g / 255.f) * 1.5f);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_ZENITH_TRANSITION_COL_B, (b / 255.f) * 1.5f);

	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_STARS_ITEN, 1000.f);

	if (m_Vars.m_PedColor) {
		if (m_Vars.m_PedRainbow) {
			RGBFadee();
			SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_COL_R, (m_Vars.m_PedR / 255.f));
			SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_COL_G, (m_Vars.m_PedG / 255.f));
			SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_COL_B, (m_Vars.m_PedB / 255.f));
			SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_MULT, m_Vars.m_PedMult);
		}
		else {
			SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_COL_R, (m_Vars.m_PedR / 255.f));
			SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_COL_G, (m_Vars.m_PedG / 255.f));
			SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_COL_B, (m_Vars.m_PedB / 255.f));
			SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_MULT, m_Vars.m_PedMult);
		}
	}
	else {
		SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_MULT, 0);
	}

	if (m_Vars.m_ShadowLight) {
		SetTimeCycleVar(eTimeCycleVar::TCVAR_SHADOW_DISTANCE_MULT, m_Vars.m_ShadowLightVal);
	}
	if (m_Vars.m_Reflection) {
		SetTimeCycleVar(eTimeCycleVar::TCVAR_REFLECTION_HDR_MULT, m_Vars.m_ReflectionVal);
	}
	if (m_Vars.m_MotionBlur) {
		SetTimeCycleVar(eTimeCycleVar::TCVAR_POSTFX_MOTIONBLUR_LENGTH, m_Vars.m_MotionBlurVal);
	}
	if (m_Vars.m_Desaturation) {
		SetTimeCycleVar(eTimeCycleVar::TCVAR_POSTFX_DESATURATION, m_Vars.m_DesaturationVal);
	}

	/*if (m_Vars.m_PedLight) {
		RGBFadee();
		SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_COL_R, (r / 255.f));
		SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_COL_G, (g / 255.f));
		SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_COL_B, (b / 255.f));
		SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_MULT, 150);
		//SetTimeCycleVar(eTimeCycleVar::TCVAR_POSTFX_DESATURATION, 3.f);
	}
	else {
		SetTimeCycleVar(eTimeCycleVar::TCVAR_PED_LIGHT_MULT, 0);
	}*/
}