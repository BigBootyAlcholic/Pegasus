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
	struct SkyColor {
		bool m_SaveToConfig;
		bool m_RainbowToggle;
		float m_Luminance = 1.f;

		Color m_InternalColor;
		Math::Vector3_<float> m_Color;
		Math::Vector4<float>* m_MemoryColor;
		bool m_Rainbow;
		std::vector<uint8_t> m_Cache;
	};

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

		bool sync_time;
		bool freeze_time;
		bool always_day;
		bool always_night;

		SkyColor m_AzimuthEastColor;
		SkyColor m_AzimuthTransitionColor;
		SkyColor m_AzimuthWestColor;
		SkyColor m_CloudBaseMinusMidColour;
		SkyColor m_CloudMidColor;
		SkyColor m_CloudShadowMinusBaseColourTimesShadowStrength;
		SkyColor m_MoonColor;
		SkyColor m_SkyPlaneColor;
		SkyColor m_SmallCloudColorHdr;
		SkyColor m_SunColor;
		SkyColor m_ZenithColor;
		SkyColor m_ZenithTransitionColor;

		int m_SelectedSkyColor;

		bool m_WaypointTrail;
		bool m_ARWaypoint;

		bool m_AestheticLights;
		bool m_AestheticLightsRainbow;
		Color m_m_AestheticLightsColor;
		int mm_AestheticLightsIntensity = 10;



	};

	extern Vars_ m_Vars;
}
