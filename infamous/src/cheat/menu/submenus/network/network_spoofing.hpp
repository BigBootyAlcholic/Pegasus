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

class NetworkSpoofingMenu {
public:
	void Run();
	void Update();
};

static NetworkSpoofingMenu* GetNetworkSpoofingMenu() {
	static NetworkSpoofingMenu instance;
	return &instance;
}

namespace NetworkSpoofingMenuVars {
	struct Vars_ {

		bool m_HideFromPlayerlist;
		bool m_SpoofName;
		std::string m_Name = "Pegasus";
		int m_Color;
		int m_Style;
		int m_Icon;

		bool m_PedSpinbot;
		bool m_PedJitter;
		bool m_PedFaceDirection;
		bool m_PedRelative;

		int m_PedFaceDirectionVal;
		float m_PedSpinbotSpeed = 10.f;

		Math::Vector2<float> m_PedHookValue;

		bool m_MoveAnimation;

		bool m_VehGhost;
		bool m_VehSpinbot;
		bool m_VehJitter;
		bool m_VehStatic;
		bool m_VehRelative;

		float m_VehSpinbotSpeed = 10.f;

		int m_VehSpinbotAxis;
		int m_VehJitterAxis;

		Math::Vector3_<float> m_VehStaticValue;
		Math::Vector3_<bool> m_VehStaticToggle;
		Math::Vector3_<float> m_VehHookValue;

		Vehicle m_VehGhostHandle;
	};

	extern Vars_ m_Vars;
}