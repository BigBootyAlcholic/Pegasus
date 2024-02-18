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

class VehicleMenu {
public:
	void Run();
	void Update();
};

static VehicleMenu* GetVehicleMenu() {
	static VehicleMenu instance;
	return &instance;
}

namespace VehicleMenuVars {
	struct Vars_ {
		bool m_GodMode = false;
		bool m_RenderBurnt = false;
		bool m_AutoRepair = false;
		bool m_AutoWash = false;
		bool m_AutoFlip = false;
		bool m_AntiLockon = false;
		bool m_AntiStickyBomb = false;
		bool m_EngineAlwaysOn = false;
		bool m_DisableFullCollision = false;
		bool m_DisableCollisionWithVehicles = false;
		bool m_DisableCollisionWithPeds = false;
		bool m_DisableCollisionWithObjects = false;
		bool m_NoClip = false;
		float m_NoClipSpeed = 1.f;
		bool m_VehicleFly = false;
		bool m_Hydraulics = false;
		bool m_DriftMode = false;
		bool m_TumbleDryer = false;
		bool m_StickyTires = false;
		bool m_SlippyWheels = false;
		bool m_NoGravity = false;
		bool m_SuperGrip = false;
		bool m_DriveOnWater = false;
		bool m_DriveThroughWalls = false;
		bool m_DriveOnAir = false;
		bool m_BypassMaxSpeed = false;
		bool m_NoTurbulence = false;
		bool m_HandBrake = false;
		bool m_InstantlyEnter = false;
		bool m_InstantlyExit = false;
		bool m_Invisibility = false;
		bool m_Seatbelt = false;
	};

	extern Vars_ m_Vars;
}
