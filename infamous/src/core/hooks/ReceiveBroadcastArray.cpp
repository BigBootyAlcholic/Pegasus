#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/util/global.hpp"
#include "cheat/util/shop_manager.hpp"
#include "rage/classes/script/scriptManager.hpp"
namespace Hooks {
	enum class eFreemodeState
	{
		NONE = 0,
		UNK_2 = 2,
		RUNNING = 4,
		CLOSING = 5,
		UNK_10 = 10,
		UNK_11 = 11
	};

	bool ReceiveBroadcastArrayHook(Rage::netArrayHandlerBase* netArray, CNetGamePlayer* fromPlayer, Rage::datBitBuffer* msgBuffer, int msgSize, std::int16_t msgCycle) {
		bool result = OgReceiveBroadcastArray(netArray, fromPlayer, msgBuffer, msgSize, msgCycle);


		if (netArray->m_array == Menu::Global(2648918).As<void*>() && Menu::Global(2648918).As<eFreemodeState>() == eFreemodeState::CLOSING) {
			Menu::Global(2648918).As<eFreemodeState>() = eFreemodeState::RUNNING;

			LOG_WARN("Broadcast Variable: Drop Kick");
		}

		return result;
	}


}