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
	Math::Vector3<float> GetCameraDirection(Math::Vector3<float> Rotation = Native::GetGameplayCamRot(0));
};

static VehicleMenu* GetVehicleMenu() {
	static VehicleMenu instance;
	return &instance;
}

namespace VehicleMenuVars {

	struct Vars_ {
		bool m_GodMode = false;
		bool m_Invisibility = false;
		bool m_Seatbelt = false;

		bool m_FlyingVehicle;
		bool m_DisableMaxSpeed;
		bool m_LimitMaxSpeed;

		float m_FlyingVehicleSpeed = 1.f;
		float m_LimitMaxSpeedVal;

		Vehicle m_Container;

		bool m_AutoDeploy;
		int m_BoostSpeed = 40;
		bool m_HornBoost;

		bool m_NoTurbulence = false;
		bool m_HandBrake = false;
		bool m_InstantlyEnter = false;
		bool m_InstantlyExit = false;
		int m_Seat;
		bool m_RocketBoost;
		bool m_VerticalRocketBoost;

		bool m_DisableFullCollision;
		bool m_DisableVehicleCollision;

		bool m_AutoRepair;
		bool m_AutoWash;

		bool m_DisableLockOn;
		bool m_DrivebyInAllVehicles;
		bool m_Transparency;
		bool m_FrontRamp;
		bool m_RearRamp;
		bool m_LeftRamp;
		bool m_RightRamp;

		int m_RampType = 1;
		int m_Gravity;
		bool m_SlippyWheels;
		bool m_DriveOnWater;
		bool m_Freeze;
		bool m_StickToGround;

		Entity m_DriveOnWaterEntity;
	};

	extern Vars_ m_Vars;
}
