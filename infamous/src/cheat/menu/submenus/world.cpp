#include "stdafx.hpp"
#include "world.hpp"
#include "cheat/util/notify.hpp"
#include "cheat/util/control_manager.hpp"
#include "core/patterns.hpp"
#include "rage/engine.hpp"
using namespace WorldMenuVars;
using namespace Framework::Options;
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

	void NoOcean(bool enable)
	{
		for (int Iterator = 0; Iterator < Patterns::Vars::g_WaterQuads.m_oceanQuads->m_quadCount; Iterator++)
		{
			const std::uint64_t QuadStruct = Patterns::Vars::g_WaterQuads.m_oceanQuads->m_quadPool + (Iterator * 0x1C);
			const auto Height = *(float*)(QuadStruct + 0x14);
			if (enable && Height == 0.f)
				*(float*)(QuadStruct + 0x14) = -1000.f;
			else if (!enable && Height == -1000.f)
				*(float*)(QuadStruct + 0x14) = 0.f;
		}
	}

	void RaiseOcean(bool enable)
	{
		for (int Iterator = 0; Iterator < Patterns::Vars::g_WaterQuads.m_oceanQuads->m_quadCount; Iterator++)
		{
			const std::uint64_t QuadStruct = Patterns::Vars::g_WaterQuads.m_oceanQuads->m_quadPool + (Iterator * 0x1C);
			const auto Height = *(float*)(QuadStruct + 0x14);
			if (enable && Height == 0.f)
				*(float*)(QuadStruct + 0x14) = 10.f;
			else if (!enable && Height == 10.f)
				*(float*)(QuadStruct + 0x14) = 0.f;
		}
	}

	void TransparentOcean(bool enable)
	{
		for (int Iterator = 0; Iterator < Patterns::Vars::g_WaterQuads.m_oceanQuads->m_quadCount; Iterator++)
		{
			const std::uint64_t QuadStruct = Patterns::Vars::g_WaterQuads.m_oceanQuads->m_quadPool + (Iterator * 0x1C);
			if (enable)
				*(int*)(QuadStruct + 0x8) = 0;
			else
				*(int*)(QuadStruct + 0x8) = 0x1A1A1A1A;
		}
	}

	const char* SkyColorsNames[] = {
		"Azimuth East Color",
		"Azimuth West Color",
		"Azimuth Transition Color",
		"Cloud Base Minus Mid Color",
		"Cloud Mid Color",
		"Cloud Shadow Base Color",
		"Moon Color",
		"Sky Plane Color",
		"Sun Color",
		"Zenith Color",
		"Zenith Transition Color"
	};
	struct SkyColorNamesT { const char* Name; SkyColor Result; };
	std::vector<SkyColorNamesT> SkyColors = {
		{ "Azimuth East Color", m_Vars.m_AzimuthEastColor },
		{ "Azimuth West Color", m_Vars.m_AzimuthWestColor },
		{ "Azimuth Transition Color", m_Vars.m_AzimuthTransitionColor },
		{ "Cloud Base Minus Mid Color", m_Vars.m_CloudBaseMinusMidColour },
		{ "Cloud Mid Color", m_Vars.m_CloudMidColor },
		{ "Cloud Shadow Base Color", m_Vars.m_CloudShadowMinusBaseColourTimesShadowStrength },
		{ "Moon Color", m_Vars.m_MoonColor },
		{ "Sky Plane Color", m_Vars.m_SkyPlaneColor },
		{ "Sun Color", m_Vars.m_SunColor },
		{ "Zenith Color", m_Vars.m_ZenithColor },
		{ "Zenith Transition Color", m_Vars.m_ZenithTransitionColor }
	};

	const char* weather_type[] = {
		   "clear", "clearing", "clouds", "extrasunny", "foggy", "halloween", "neutral", "overcast", "rain", "smog", "snow", "snowlight", "Blizzard", "thunder"
	}; inline std::size_t weather_id = 0;

	inline const char* cloud_type[] = {

		"shower", "Snowy 01", "Stormy 01", "Stratoscumulus", "Stripey", "Wispy", "Altostratus", "Cirrocumulus", "Altostratus", "Clear 01", "Cloudy 01" ,"Contrails", "NoClouds", "Puffs", "Rain", "Snow"
	}; inline std::size_t cloud_id = 0;

	void SetClouds(const char* cloudtexture) {
		Native::_SetCloudHatTransition(cloudtexture, 3.f);
	}
	void WorldRgb() {
		if (m_Vars.m_m_AestheticLightsColor.r > 0 && m_Vars.m_m_AestheticLightsColor.b == 0) {
			m_Vars.m_m_AestheticLightsColor.r--;
			m_Vars.m_m_AestheticLightsColor.g++;
		}
		if (m_Vars.m_m_AestheticLightsColor.g > 0 && m_Vars.m_m_AestheticLightsColor.r == 0) {
			m_Vars.m_m_AestheticLightsColor.g--;
			m_Vars.m_m_AestheticLightsColor.b++;
		}
		if (m_Vars.m_m_AestheticLightsColor.b > 0 && m_Vars.m_m_AestheticLightsColor.g == 0) {
			m_Vars.m_m_AestheticLightsColor.r++;
			m_Vars.m_m_AestheticLightsColor.b--;
		}
	}
}

bool Test = false;

void WorldMenu::Run() {
	Framework::addSubmenu("World", "world-menu", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Weather")
			.setTarget("world-weather"));

		core->addOption(Framework::Options::SubmenuOption("Time")
			.setTarget("world-time"));

		core->addOption(Framework::Options::SubmenuOption("Clearing")
			.setTarget("world-clearing"));

		core->addOption(Framework::Options::SubmenuOption("Aesthetic Lights")
			.setTarget("world-lights"));

		core->addOption(Framework::Options::SubmenuOption("Visual Modifiers")
			.setTarget("tc_modifiers"));

		core->addOption(Framework::Options::SubmenuOption("Sky")
			.setTarget("world-sky"));

		core->addOption(Framework::Options::SubmenuOption("Ped Color")
			.setTarget("ped_color"));

		core->addOption(ToggleOption("AR Waypoint")
			.addToggle(&m_Vars.m_ARWaypoint));

		core->addOption(ToggleOption("Waypoint Trail")
			.addToggle(&m_Vars.m_WaypointTrail));
	});

	Framework::addSubmenu("Aesthetic Lights", "world-lights", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Toggle")
			.addToggle(&m_Vars.m_AestheticLights));

		core->addOption(NumberOption<int>("[R]ed")
			.addNumber(&m_Vars.m_m_AestheticLightsColor.r)
			.addMin(0).addMax(255).addStep(1).setPrecision(0));

		core->addOption(NumberOption<int>("[G]reen")
			.addNumber(&m_Vars.m_m_AestheticLightsColor.g)
			.addMin(0).addMax(255).addStep(1).setPrecision(0));

		core->addOption(NumberOption<int>("[B]lue")
			.addNumber(&m_Vars.m_m_AestheticLightsColor.b)
			.addMin(0).addMax(255).addStep(1).setPrecision(0));

		core->addOption(NumberOption<int>("Luminance")
			.addNumber(&m_Vars.mm_AestheticLightsIntensity)
			.addMin(0).addMax(100).addStep(0.1).setPrecision(2));

		core->addOption(Framework::Options::ToggleOption("Rainbow")
			.addToggle(&SkyColors[m_Vars.m_SelectedSkyColor].Result.m_RainbowToggle));
	});

	Framework::addSubmenu("Clearing", "world-clearing", [](Framework::Options::Core* core) {
		auto coords = Native::GetEntityCoords(Native::PlayerPedId(), 0);
		float range = 150;
		Native::DrawMarker(28, coords.m_X, coords.m_Y, coords.m_Z,  0.f, 0.f, 0.f ,  0.f, 0.f, 0.f ,  range, range, range , 0, 0, 255, 160, false, false, 0, false, NULL, NULL, false);

		core->addOption(ButtonOption("Clear Everything")
			.addClick([=] { Native::_ClearAreaOfEverything(coords.m_X, coords.m_Y, coords.m_Z, range, 0, 0, 0, 0); }));

		core->addOption(ButtonOption("Clear Cops")
			.addClick([=] { Native::ClearAreaOfCops(coords.m_X, coords.m_Y, coords.m_Z, range, 0); }));

		core->addOption(ButtonOption("Clear Objects")
			.addClick([=] { Native::ClearAreaOfObjects(coords.m_X, coords.m_Y, coords.m_Z, range, 0); }));

		core->addOption(ButtonOption("Clear Peds")
			.addClick([=] { Native::ClearAreaOfPeds(coords.m_X, coords.m_Y, coords.m_Z, range, 0); }));

		core->addOption(ButtonOption("Clear Projectiles")
			.addClick([=] { Native::ClearAreaOfProjectiles(coords.m_X, coords.m_Y, coords.m_Z, range, 0); }));

		core->addOption(ButtonOption("Clear Vehicles")
			.addClick([=] { Native::ClearAreaOfVehicles(coords.m_X, coords.m_Y, coords.m_Z, range, 0, 0, 0, 0, 0, 0); }));
	});

	Framework::addSubmenu("Time", "world-time", [](Framework::Options::Core* core) {
		static int H = 0, M = 0, S = 0;

		core->addOption(ToggleOption("Always Day")
			.addToggle(&m_Vars.always_day));

		core->addOption(ToggleOption("Always Night")
			.addToggle(&m_Vars.always_night));

		core->addOption(ToggleOption("Freeze Time")
			.addToggle(&m_Vars.freeze_time));

		core->addOption(ToggleOption("Sync with System Time")
			.addToggle(&m_Vars.sync_time));

		core->addOption(NumberOption<int>("Hour")
			.addNumber(&H).addMin(0).addMax(24).addStep(1)
			.addClick([] { Native::NetworkOverrideClockTime(H, M, S); }));

		core->addOption(NumberOption<int>("Minute")
			.addNumber(&M).addMin(0).addMax(24).addStep(1)
			.addClick([] { Native::NetworkOverrideClockTime(H, M, S); }));

		core->addOption(NumberOption<int>("Second")
			.addNumber(&S).addMin(0).addMax(24).addStep(1)
			.addClick([] { Native::NetworkOverrideClockTime(H, M, S); }));
	});

	Framework::addSubmenu("Weather", "world-weather", [](Framework::Options::Core* core) {
		core->addOption(scrollOption<const char*, std::size_t>("Modify Weather")
			.addScroll(&weather_type).setPosition(&weather_id).addClick([] { Native::SetWeatherTypeNowPersist(weather_type[weather_id]); }));

		core->addOption(scrollOption<const char*, std::size_t>("Modify Clouds")
			.addScroll(&cloud_type).setPosition(&cloud_id).addClick([] { SetClouds(cloud_type[cloud_id]); }));

		core->addOption(ButtonOption("Flash Lightning")
			.addClick([] { Native::_CreateLightningThunder(); }));
	});

	Framework::addSubmenu("Sky", "world-sky", [](Framework::Options::Core* core) {
		core->addOption(scrollOption<const char*, int>("Element")
			.addScroll(&SkyColorsNames).setPosition(&m_Vars.m_SelectedSkyColor));

		core->addOption(NumberOption<float>("[R]ed")
			.addNumber(&SkyColors[m_Vars.m_SelectedSkyColor].Result.m_Color.m_X)
			.addMin(0).addMax(255).addStep(1).setPrecision(0));

		core->addOption(NumberOption<float>("[G]reen")
			.addNumber(&SkyColors[m_Vars.m_SelectedSkyColor].Result.m_Color.m_Y)
			.addMin(0).addMax(255).addStep(1).setPrecision(0));

		core->addOption(NumberOption<float>("[B]lue")
			.addNumber(&SkyColors[m_Vars.m_SelectedSkyColor].Result.m_Color.m_Z)
			.addMin(0).addMax(255).addStep(1).setPrecision(0));

		core->addOption(NumberOption<float>("Luminance")
			.addNumber(&SkyColors[m_Vars.m_SelectedSkyColor].Result.m_Luminance)
			.addMin(0).addMax(100).addStep(0.1).setPrecision(1));

		core->addOption(Framework::Options::ToggleOption("Rainbow")
			.addToggle(&SkyColors[m_Vars.m_SelectedSkyColor].Result.m_RainbowToggle));
	});

	Framework::addSubmenu("Ocean", "world-ocean", [](Framework::Options::Core* core) {
		//no worky, either bad offsets or sig
		core->addOption(Framework::Options::ButtonOption("No Ocean")
			.addClick([] { NoOcean(true); }));

		core->addOption(Framework::Options::ButtonOption("Raise Ocean")
			.addClick([] { RaiseOcean(true); }));

		core->addOption(Framework::Options::ButtonOption("Transparent Ocean")
			.addClick([] { TransparentOcean(true); }));

		core->addOption(Framework::Options::ButtonOption("Restore")
			.addClick([] { TransparentOcean(false); RaiseOcean(false); NoOcean(false); }));
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
	RGBFade();

	if (m_Vars.m_AestheticLights) {
		Math::Vector3<float> coords = Native::GetEntityCoords(Native::PlayerPedId(), true);
		if (m_Vars.m_AestheticLightsRainbow) {
			WorldRgb();
			Native::DrawLightWithRange(coords.m_X, coords.m_Y, coords.m_Z, m_Vars.m_m_AestheticLightsColor.r, m_Vars.m_m_AestheticLightsColor.g, m_Vars.m_m_AestheticLightsColor.b, 1000, m_Vars.mm_AestheticLightsIntensity);
		}
		else {
			Native::DrawLightWithRange(coords.m_X, coords.m_Y, coords.m_Z, m_Vars.m_m_AestheticLightsColor.r, m_Vars.m_m_AestheticLightsColor.g, m_Vars.m_m_AestheticLightsColor.b, 1000, m_Vars.mm_AestheticLightsIntensity);
		}
	}

	if (m_Vars.sync_time) {
		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);
		Native::SetClockTime(t.tm_hour, t.tm_min, t.tm_sec);
	}
	if (m_Vars.freeze_time) {
		Native::PauseClock(true);
	}
	else {
		Native::PauseClock(false);
	}
	if (m_Vars.always_day) {
		Native::NetworkOverrideClockTime(15, 0, 0);
	}
	if (m_Vars.always_night) {
		Native::NetworkOverrideClockTime(1, 0, 0);
	}

	if (m_Vars.always_day == true)
		m_Vars.always_night = false;

	if (m_Vars.always_night == true)
		m_Vars.always_day = false;

	if (m_Vars.m_WaypointTrail && Patterns::Vars::g_WaypointData) {
		if (Patterns::Vars::g_WaypointData->m_points && Patterns::Vars::g_WaypointData->m_count) {
			for (uint32_t i = 0; i < Patterns::Vars::g_WaypointData->m_count; i++) {
				if (i >= 40) break;

				Math::Vector4<float> point = Patterns::Vars::g_WaypointData->m_points[i];
				Native::DrawMarker(28, point.m_X, point.m_Y, point.m_Z + 0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.5f, 0.5f, 0.5f, 255, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0);
			}
		}
	}

	if (m_Vars.m_ARWaypoint) {
		Math::Vector3<float> coords = Native::GetBlipInfoIdCoord(Native::GetFirstBlipInfoId(8));
		Native::DrawMarker(28, coords.m_X + 0.5f, coords.m_Y + 0.5f, coords.m_Z + 0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 5.f, 5.f, 9999.f, 255, 0, 0, 150, 0, 0, 0, 0, 0, 0, 0);
	}

	//real nigga shit
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_EAST_COL_B, (m_Vars.m_AzimuthEastColor.m_Color.m_X / 255.f) * m_Vars.m_AzimuthEastColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_EAST_COL_G, (m_Vars.m_AzimuthEastColor.m_Color.m_Y / 255.f) * m_Vars.m_AzimuthEastColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_EAST_COL_B, (m_Vars.m_AzimuthEastColor.m_Color.m_Z / 255.f) * m_Vars.m_AzimuthEastColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_WEST_COL_R, (m_Vars.m_AzimuthWestColor.m_Color.m_X / 255.f) * m_Vars.m_AzimuthWestColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_WEST_COL_G, (m_Vars.m_AzimuthWestColor.m_Color.m_Y / 255.f) * m_Vars.m_AzimuthWestColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_WEST_COL_B, (m_Vars.m_AzimuthWestColor.m_Color.m_Z / 255.f) * m_Vars.m_AzimuthWestColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_TRANSITION_COL_R, (m_Vars.m_AzimuthTransitionColor.m_Color.m_X / 255.f) * m_Vars.m_AzimuthTransitionColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_TRANSITION_COL_G, (m_Vars.m_AzimuthTransitionColor.m_Color.m_Y / 255.f) * m_Vars.m_AzimuthTransitionColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_AZIMUTH_TRANSITION_COL_B, (m_Vars.m_AzimuthTransitionColor.m_Color.m_Z / 255.f) * m_Vars.m_AzimuthTransitionColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_ZENITH_COL_R, (m_Vars.m_ZenithColor.m_Color.m_X / 255.f) * m_Vars.m_ZenithColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_ZENITH_COL_G, (m_Vars.m_ZenithColor.m_Color.m_Y / 255.f) * m_Vars.m_ZenithColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_ZENITH_COL_B, (m_Vars.m_ZenithColor.m_Color.m_Z / 255.f) * m_Vars.m_ZenithColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_ZENITH_TRANSITION_COL_R, (m_Vars.m_ZenithTransitionColor.m_Color.m_X / 255.f) * m_Vars.m_ZenithTransitionColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_ZENITH_TRANSITION_COL_G, (m_Vars.m_ZenithTransitionColor.m_Color.m_Y / 255.f) * m_Vars.m_ZenithTransitionColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_ZENITH_TRANSITION_COL_B, (m_Vars.m_ZenithTransitionColor.m_Color.m_Z / 255.f) * m_Vars.m_ZenithTransitionColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_PLANE_COL_R, (m_Vars.m_SkyPlaneColor.m_Color.m_X / 255.f) * m_Vars.m_SkyPlaneColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_PLANE_COL_G, (m_Vars.m_SkyPlaneColor.m_Color.m_Y / 255.f) * m_Vars.m_SkyPlaneColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_PLANE_COL_B, (m_Vars.m_SkyPlaneColor.m_Color.m_Z / 255.f) * m_Vars.m_SkyPlaneColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_CLOUD_MID_COL_R, (m_Vars.m_CloudMidColor.m_Color.m_X / 255.f) * m_Vars.m_CloudMidColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_CLOUD_MID_COL_G, (m_Vars.m_CloudMidColor.m_Color.m_Y / 255.f) * m_Vars.m_CloudMidColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_CLOUD_MID_COL_B, (m_Vars.m_CloudMidColor.m_Color.m_Z / 255.f) * m_Vars.m_CloudMidColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_CLOUD_BASE_COL_R, (m_Vars.m_CloudBaseMinusMidColour.m_Color.m_X / 255.f) * m_Vars.m_CloudBaseMinusMidColour.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_CLOUD_BASE_COL_G, (m_Vars.m_CloudBaseMinusMidColour.m_Color.m_Y / 255.f) * m_Vars.m_CloudBaseMinusMidColour.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_CLOUD_BASE_COL_B, (m_Vars.m_CloudBaseMinusMidColour.m_Color.m_Z / 255.f) * m_Vars.m_CloudBaseMinusMidColour.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_CLOUD_SHADOW_COL_R, (m_Vars.m_CloudShadowMinusBaseColourTimesShadowStrength.m_Color.m_X / 255.f) * m_Vars.m_CloudShadowMinusBaseColourTimesShadowStrength.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_CLOUD_SHADOW_COL_G, (m_Vars.m_CloudShadowMinusBaseColourTimesShadowStrength.m_Color.m_Y / 255.f) * m_Vars.m_CloudShadowMinusBaseColourTimesShadowStrength.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_CLOUD_SHADOW_COL_B, (m_Vars.m_CloudShadowMinusBaseColourTimesShadowStrength.m_Color.m_Z / 255.f) * m_Vars.m_CloudShadowMinusBaseColourTimesShadowStrength.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_MOON_COL_R, (m_Vars.m_MoonColor.m_Color.m_X / 255.f) * m_Vars.m_MoonColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_MOON_COL_G, (m_Vars.m_MoonColor.m_Color.m_Y / 255.f) * m_Vars.m_MoonColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_MOON_COL_B, (m_Vars.m_MoonColor.m_Color.m_Z / 255.f) * m_Vars.m_MoonColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_SUN_COL_R, (m_Vars.m_SunColor.m_Color.m_X / 255.f) * m_Vars.m_SunColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_SUN_COL_G, (m_Vars.m_SunColor.m_Color.m_Y / 255.f) * m_Vars.m_SunColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_SUN_COL_B, (m_Vars.m_SunColor.m_Color.m_Z / 255.f) * m_Vars.m_SunColor.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_SMALL_CLOUD_COL_R, (m_Vars.m_SmallCloudColorHdr.m_Color.m_X / 255.f) * m_Vars.m_SmallCloudColorHdr.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_SMALL_CLOUD_COL_G, (m_Vars.m_SmallCloudColorHdr.m_Color.m_Y / 255.f) * m_Vars.m_SmallCloudColorHdr.m_Luminance);
	SetTimeCycleVar(eTimeCycleVar::TCVAR_SKY_SMALL_CLOUD_COL_B, (m_Vars.m_SmallCloudColorHdr.m_Color.m_Z / 255.f) * m_Vars.m_SmallCloudColorHdr.m_Luminance);

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