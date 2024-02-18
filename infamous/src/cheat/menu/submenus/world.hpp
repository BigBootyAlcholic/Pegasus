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

class WorldMenu {
public:
	void Run();
	void Update();
};

static WorldMenu* GetWorldMenu() {
	static WorldMenu instance;
	return &instance;
}

namespace WorldMenuVars {
	struct Vars_ {

		bool m_ShadowLight = false;
		bool m_Reflection = false;
		bool m_Gamma = false;
		bool m_MotionBlur = false;
		bool m_SkyIntensity = false;
		bool m_Desaturation = false;

		float m_ShadowLightVal = 1.f;
		float m_ReflectionVal = 1.f;
		float m_GammaVal = 1.f;
		float m_MotionBlurVal = 1.f;
		float m_SkyIntensityVal = 1.f;
		float m_DesaturationVal = 1.f;

		bool m_PedColor;
		bool m_PedRainbow;
		float m_PedMult = 100.f;
		int m_PedR = 255.f;
		int m_PedG;
		int m_PedB;


	};

	extern Vars_ m_Vars;
}
