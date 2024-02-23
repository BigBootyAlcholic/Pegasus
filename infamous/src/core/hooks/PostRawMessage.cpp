#include "stdafx.hpp"
#include "../hooks.hpp"

namespace Hooks {
	bool PostRawMessageHook(int localGamerindex, Rage::rlGamerHandle* recipients, unsigned int numRecipients, const char* message, unsigned int ttlSeconds) {
		bool _Return = OgPostRawMessage(localGamerindex, recipients, numRecipients, message, ttlSeconds);
		LOG_WARN(std::format("DEBUG MESSAGE: {}", message).c_str());
		if (_Return) {
			LOG_WARN("DEBUG MESSAGE CONTINUE");
		}
		return _Return;
	}
}