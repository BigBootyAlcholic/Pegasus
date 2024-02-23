#include "stdafx.hpp"
#include "../hooks.hpp"

namespace Hooks {
	bool HasGameBeenAlteredHook() {
		bool FUNCTION__ = OgHasGameBeenAltered();
		if (FUNCTION__) {
			FUNCTION__ = false;
		}
		return FUNCTION__;
	}
}