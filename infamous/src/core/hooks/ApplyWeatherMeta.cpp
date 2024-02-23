#include "stdafx.hpp"
#include "../hooks.hpp"

namespace Hooks {
	void ApplyWeatherMetaHook(uint64_t Meta) {
		return OgApplyWeatherMeta(Meta);
	}
}