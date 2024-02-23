#include "stdafx.hpp"
#include "../hooks.hpp"
#include "rage/invoker/native_invoker.hpp"
#include "cheat/menu/main.hpp"
#include "cheat/util/scr_globals.hpp"
#include "cheat/menu/submenus/network.hpp"
#include "rage/classes/enums/enums.hpp"
#include "rage/engine.hpp"
namespace Hooks {
	void SetWarningMessageWithHeader(Rage::Invoker::NativeContext* src) {
		if (auto entry_line = src->GetArgument<const char*>(1); !strcmp(entry_line, "CTALERT_F_2")) {
			/**ScriptGlobal(4536673).As<int*>() = 0;*/

			return;
		}

		Native::_SetWarningMessage2(src->GetArgument<const char*>(0), src->GetArgument<const char*>(1), src->GetArgument<int>(2), src->GetArgument<const char*>(3), src->GetArgument<BOOL>(4), src->GetArgument<Any>(5), src->GetArgument<Any*>(6), src->GetArgument<Any*>(7), src->GetArgument<BOOL>(8));
	}

	void NetworkSessionHost(Rage::Invoker::NativeContext* Context) {
		if (NetworkMenuVars::m_Vars.m_JoinQueue) {
			Engine::JoinSessionByInfo(*Patterns::Vars::g_Network, &NetworkMenuVars::m_Vars.m_SessionInfo, 1, 1 | 2 | 4, nullptr, 0);
			NetworkMenuVars::m_Vars.m_JoinQueue = false;
			Context->SetReturn(0, TRUE);
		}
		else {
			Context->SetReturn(0, Native::NetworkSessionHost(Context->GetArgument<int>(0), Context->GetArgument<int>(1), Context->GetArgument<bool>(2)));
		}
	}

	void _0x6BFB12CE158E3DD4(Rage::Invoker::NativeContext* Context) {
		if (MainMenuVars::m_Vars.m_SeamlessJoin) {
			Native::_0x6BFB12CE158E3DD4(false);
		}
		else {
			Native::_0x6BFB12CE158E3DD4(Context->GetArgument<Any>(0));
		}
	}
	void _0xFE4C1D0D3B9CC17E(Rage::Invoker::NativeContext* Context) {
		if (MainMenuVars::m_Vars.m_SeamlessJoin) {
			Native::_0xFE4C1D0D3B9CC17E(false, false);
		}
		else {
			Native::_0xFE4C1D0D3B9CC17E(Context->GetArgument<Any>(0), Context->GetArgument<Any>(0));
		}
	}

	void IsSwitchToMultiFirstPartFinished(Rage::Invoker::NativeContext* Context) {
		if (MainMenuVars::m_Vars.m_SeamlessJoin) {
			//Context->SetReturn<BOOL>(true, 0);
		}
		else {
			Native::_0x933BBEEB8C61B5F4();
		}
	}

	void SetFocusEntity(Rage::Invoker::NativeContext* Context) {
		if (MainMenuVars::m_Vars.m_SeamlessJoin) {
			Native::SetFocusEntity(Native::PlayerPedId());
		}
		else {
			Native::SetFocusEntity(Context->GetArgument<Entity>(0));
		}
	}

	void HideHudAndRadar(Rage::Invoker::NativeContext* Context) {
		if (!MainMenuVars::m_Vars.m_SeamlessJoin) {
			Native::HideHudAndRadarThisFrame();
		}
	}

	void ActivateFrontendMenu(Rage::Invoker::NativeContext* Context) {
		Hash menuhash = Context->GetArgument<Hash>(0);
		BOOL togglePause = Context->GetArgument<BOOL>(1);
		int component = Context->GetArgument<int>(2);

		if (MainMenuVars::m_Vars.m_SeamlessJoin && menuhash != RAGE_JOAAT("FE_MENU_VERSION_EMPTY_NO_BACKGROUND")) {
			Native::ActivateFrontendMenu(menuhash, togglePause, component);
		}
		if (!MainMenuVars::m_Vars.m_SeamlessJoin) {
			Native::ActivateFrontendMenu(menuhash, togglePause, component);
		}
	}

	void RestartFrontendMenu(Rage::Invoker::NativeContext* Context) {
		Hash menuhash = Context->GetArgument<Hash>(0);
		int p1 = Context->GetArgument<int>(1);

		if (MainMenuVars::m_Vars.m_SeamlessJoin) {
			if (menuhash != RAGE_JOAAT("FE_MENU_VERSION_EMPTY_NO_BACKGROUND"))
			{
				Native::RestartFrontendMenu(menuhash, p1);
			}
		}
		else {
			Native::RestartFrontendMenu(menuhash, p1);
		}
	}

	void TogglePauseRenderPhase(Rage::Invoker::NativeContext* Context) {
		if (MainMenuVars::m_Vars.m_SeamlessJoin) {
			Native::_0xE1C8709406F2C41C();
		}
		else {
			Native::_SetFrozenRenderingDisabled(Context->GetArgument<int>(0));
		}
	}

	void SetEntityVisible(Rage::Invoker::NativeContext* Context) {
		if (MainMenuVars::m_Vars.m_SeamlessJoin && Context->GetArgument<Entity>(0) == Native::PlayerPedId()) {
			Native::SetEntityVisible(Native::PlayerPedId(), true, false);
		}
		else {
			Native::SetEntityVisible(Context->GetArgument<Entity>(0), Context->GetArgument<BOOL>(1), Context->GetArgument<BOOL>(2));
		}
	}

	void SetEntityCoords(Rage::Invoker::NativeContext* Context) {
		if (!MainMenuVars::m_Vars.m_SeamlessJoin
			|| Menu::Global(1575008).As<eTransitionState>() == eTransitionState::TRANSITION_STATE_CONFIRM_FM_SESSION_JOINING // This check is for character selection if i remember correctly.
			|| Context->GetArgument<Entity>(0) != Native::PlayerPedId())
			Native::SetEntityCoords(Context->GetArgument<Entity>(0), Context->GetArgument<float>(1), Context->GetArgument<float>(2), Context->GetArgument<float>(3), Context->GetArgument<BOOL>(4), Context->GetArgument<BOOL>(5), Context->GetArgument<BOOL>(6), Context->GetArgument<BOOL>(7));
	}


	void SetEntityCollision(Rage::Invoker::NativeContext* Context) {
		if (!MainMenuVars::m_Vars.m_SeamlessJoin || Context->GetArgument<Entity>(0) != Native::PlayerPedId())
			Native::SetEntityCollision(Context->GetArgument<Entity>(0), Context->GetArgument<BOOL>(1), Context->GetArgument<BOOL>(2));
	}

	void SetPlayerControl(Rage::Invoker::NativeContext* Context) {
		if (!MainMenuVars::m_Vars.m_SeamlessJoin) {
			Native::SetPlayerControl(Context->GetArgument<Player>(0), Context->GetArgument<BOOL>(1), Context->GetArgument<int>(2));
		}
	}

	void FreezeEntityPosition(Rage::Invoker::NativeContext* Context) {
		if (!MainMenuVars::m_Vars.m_SeamlessJoin || Context->GetArgument<Entity>(0) != Native::PlayerPedId()) {
			Native::FreezeEntityPosition(Context->GetArgument<Entity>(0), Context->GetArgument<BOOL>(1));
		}
	}
	void ReviveLocalPlayer(Rage::Invoker::NativeContext* Context) {
		if (!MainMenuVars::m_Vars.m_SeamlessJoin && Native::IsPedDeadOrDying(Native::PlayerPedId(), 0))
			Native::NetworkResurrectLocalPlayer(Context->GetArgument<float>(0), Context->GetArgument<float>(1), Context->GetArgument<float>(2), Context->GetArgument<float>(3), Context->GetArgument<BOOL>(4), Context->GetArgument<BOOL>(5), Context->GetArgument<BOOL>(6));
	}

/*	


	
	

	
	

*/
}