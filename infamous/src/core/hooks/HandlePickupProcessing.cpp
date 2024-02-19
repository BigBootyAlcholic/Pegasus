#include "stdafx.hpp"
#include "../hooks.hpp"
#include "framework/framework.hpp"
#include "cheat/util/notify.hpp"
namespace Hooks {
	bool HandlePickupProcessingHook(Rage::netObject* object, void* unk, CPed* ped) {
		Menu::Notify::stacked("Pickup Processing");
		return false;
		//return OgHandlePickupProcessingHook(object, unk, ped);
	}
}