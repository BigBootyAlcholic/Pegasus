#pragma once
#include "core/patterns.hpp"
#include "rage/classes/script/scriptManager.hpp"
#include "cheat/util/notify.hpp"
#include "util/fiber.hpp"
#include "rage/classes/rage/joaat.hpp"

namespace Menu::Network {
	bool ForceScriptHost(Rage::joaat_t Script);
	void PreventStealing();
	namespace Vars {
		inline bool m_PreventStealing;
		inline bool m_ForcedScriptHost;
	}
}