#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/menu/submenus/network.hpp"

enum eProfanity {
	PROFANITY_CHAT_CLEAN = -1,
	PROFANITY_CLEAN,
	PROFANITY_DIRTY,
	PROFANITY_UNK,
	PROFANITY_ERROR
};


namespace Hooks {
	int GetChatProfanityStatusHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8) {
		if (NetworkMenuVars::m_Vars.m_DisableProfanityFilter) {
			return PROFANITY_CHAT_CLEAN;
		}

		return OgGetChatProfanityStatus(Rcx, Rdx, R8);
	}
}