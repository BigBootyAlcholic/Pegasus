#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/menu/submenus/protection.hpp"
namespace Hooks {
	bool UpdatePresenceAttributeIntHook(void* _This, int Index, char* Attr, uint64_t Value) {

		u32 Hash = Rage::LowerCaseJoaat(Attr);

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