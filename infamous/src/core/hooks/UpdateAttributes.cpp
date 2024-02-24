#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/menu/submenus/protection.hpp"
#include "cheat/menu/submenus/network.hpp"
#include "rage/engine.hpp"
#include "cheat/util/notify.hpp"
namespace Hooks {

	void SetSessionType() {
		if ((*Patterns::Vars::g_NetworkPlayerMgr)) {
			std::string gstype{ Engine::gsTypeToString(NetworkMenuVars::m_Vars.m_SessionType) };
			NetworkMenuVars::m_Vars.m_GsType = gstype.c_str();
		}
		else {
			NetworkMenuVars::m_Vars.m_GsType = "Offline";
		}
	}


	bool UpdatePresenceAttributeIntHook(void* _This, int Index, char* Attr, uint64_t Value) {

		u32 Hash = Rage::LowerCaseJoaat(Attr);

		if (NetworkMenuVars::m_Vars.m_ToggleSessionType && Hash == RAGE_JOAAT("gs_type")) {
			int previousType{ NetworkMenuVars::m_Vars.m_SessionType };
			NetworkMenuVars::m_Vars.m_SessionType = Value;
			SetSessionType();
			if (previousType != NetworkMenuVars::m_Vars.m_SessionType) {
				if (previousType != -1) {
					Menu::Notify::stacked(std::format("{}", Engine::gsTypeToString(previousType), NetworkMenuVars::m_Vars.m_GsType).c_str());
				}
				else {
					Menu::Notify::stacked("Joined Session From Story Mode");
				}
			}
		}

		if (ProtectionMenuVars::m_Vars.m_BlockJoin && (Hash == RAGE_JOAAT("gstok") || Hash == RAGE_JOAAT("gsid") || Hash == RAGE_JOAAT("gstype") || Hash == RAGE_JOAAT("gshost") || Hash == RAGE_JOAAT("gsjoin"))) {
			return true;
		}

		return OgUpdatePresenceAttributeInt(_This, Index, Attr, Value);
	}

	bool UpdatePresenceAttributeStringHook(void* _This, int Index, char* Attr, char* Value) {
		u32 Hash = Rage::LowerCaseJoaat(Attr);
		if (ProtectionMenuVars::m_Vars.m_BlockJoin && Hash == RAGE_JOAAT("gsinfo")) {
			return true;
		}
		if (Hash == RAGE_JOAAT("peeraddr"))
		{
			Value = (char*)"";
		}

		return OgUpdatePresenceAttributeString(_This, Index, Attr, Value);
	}
}