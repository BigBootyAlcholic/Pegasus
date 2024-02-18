#pragma once
#include "core/patterns.hpp"
#include "util/caller.hpp"

namespace Menu::Event {
	void TriggerScriptEvent(int A, int64_t* B, int C, int Bitset, int eventId) {
		Caller::Call<void>(Patterns::Vars::g_TriggerScriptEvent, A, B, C, Bitset, eventId);
	}
}