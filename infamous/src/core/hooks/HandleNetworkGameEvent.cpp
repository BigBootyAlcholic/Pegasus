#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/menu/submenus/network.hpp"
#include "rage/engine.hpp"
#include "rage/classes/network/Network.hpp"
#include "cheat/util/notify.hpp"
namespace Hooks {

	Rage::CEventNetwork* HandleNetworkGameEventHook(uint64_t Rcx, Rage::CEventNetwork* Event) {
		switch (Event->get_type()) {
		case Rage::eEventNetworkType::CEventNetworkRemovedFromSessionDueToComplaints: {
			if (NetworkMenuVars::m_Vars.m_RejoinDesyncedSessions && !Native::NetworkIsActivitySession() && Native::_GetNumberOfInstancesOfScriptWithNameHash(RAGE_JOAAT("maintransition")) == 0 && !Native::IsPlayerSwitchInProgress()) {
				Engine::JoinSession((*Patterns::Vars::g_Network)->m_last_joined_session.m_session_info);
				Menu::Notify::stacked("Desynced From Session, Rejoining");
			}
			else if (NetworkMenuVars::m_Vars.m_NotifyDesyncKicks) {
				Menu::Notify::stacked("Desynced From Session");
			}
			break;
		}
		}

		return OgHandleNetworkGameEvent(Rcx, Event);
	}

}