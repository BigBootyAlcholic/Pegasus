#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "core/core.hpp"


class PlayerMenu {
public:
	void Run();
	void Update();
};

static PlayerMenu* GetPlayerMenu() {
	static PlayerMenu instance;
	return &instance;
}

namespace PlayerMenuVars {
	struct Vars_ {

		bool m_contort;
		bool m_controllable;
		bool m_GodMode;
		bool m_DisablePolice;
		bool m_AlwaysWanted;
		bool m_DisableRagdoll;
		bool m_Invisibility;
		bool m_SuperMan;
		bool m_ReducedCollision;
		bool m_Dragon;
		int m_BreatheFireHandle;
		bool m_BadSport;

		bool m_CopsTurnBlindEye;
		bool m_OffTheRadar;
		bool m_RevealHiddenPlayers;

		uint32_t m_LastModel = 0;

		//movement
		bool m_SuperJump;
		bool m_BeastJump;
		bool m_AlwaySwim;
		bool m_NoFallAnim;
		bool m_GraceFullLanding;
		bool m_Noclip;
		bool m_SuperRun;
		bool m_SuperRunAlternative;
		bool m_SuperSwim;

		float m_SuperRunSpeed = 1.49f;
		float m_SuperRunAlternativeSpeed = 5.f;
		float m_SuperSwimSpeed = 1.49f;
		float m_NoClipSpeed = 1.f;

		float m_scale = 0.2f;
		std::map<std::string, bool> m_particleToggles;

	};

	extern Vars_ m_Vars;
}
