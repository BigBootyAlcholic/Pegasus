#pragma once
#include "core/patterns.hpp"
#include "rage/invoker/natives.hpp"
#include "rage/classes/network/netShopping.hpp"
#include "util/caller.hpp"
#include "rage/classes/rage/joaat.hpp"
#include "rage/classes/script/scrThread.hpp"
#include "rage/classes/script/TlsContext.hpp"
#include "rage/classes/script/GtaThread.hpp"
namespace Menu::NetShoppingManager {

	inline bool AddBasketToQueue(Rage::netShopping::CNetworkShoppingMgr* Queue, int* TransactionID, int Category, int Action, int Flag) {
		return Caller::Call<bool>(Patterns::Vars::g_ConstructBasket, Queue, TransactionID, Category, Action, Flag);
	}

	inline void AddItemToBasket(Rage::netShopping::CNetworkShoppingMgr* Queue, int* Data) {
		Caller::Call<int>(Patterns::Vars::g_AddItemToBasket, Queue, Data);
	}


	template<typename F, typename... Args>
	inline void execute_as_script(Rage::scrThread* thread, F&& callback, Args&&... args) {
		auto tls_ctx = Rage::tlsContext::get();
		auto og_thread = tls_ctx->m_script_thread;

		tls_ctx->m_script_thread = thread;
		tls_ctx->m_is_script_thread_active = true;

		std::invoke(std::forward<F>(callback), std::forward<Args>(args)...);

		tls_ctx->m_script_thread = og_thread;
		tls_ctx->m_is_script_thread_active = og_thread != nullptr;
	}

	template<typename F, typename... Args>
	inline void execute_as_script(Rage::joaat_t script_hash, F&& callback, Args&&... args) {
		for (auto thread : *Patterns::Vars::g_ScriptThreads) {
			if (!thread || !thread->m_context.m_thread_id || thread->m_context.m_script_hash != script_hash)
				continue;

			execute_as_script(thread, callback, args...);
			return;
		}
	}

	inline void ProcessBasketTransaction(Rage::joaat_t Category, Rage::joaat_t Action, int Destination, std::vector<Rage::netShopping::netTransaction> Items) {
		execute_as_script(joaat("shop_controller"), [&] {
			if (Native::_NetworkShopBasketEnd()) {
				_i<int>(0xFA336E7F40C0A0D0);
			}

			int transaction_id = -1;
			if (Caller::Call<bool>(Patterns::Vars::g_ConstructBasket, Patterns::Vars::g_NetShopMgr, &transaction_id, Category, Action, Destination)) {
				for (auto& item : Items) {
					Caller::Call<void>(Patterns::Vars::g_AddItemToBasket, Patterns::Vars::g_NetShopMgr, (int*)&item);
				}
				Native::_NetworkShopCheckoutStart(transaction_id);
			}
			});
	}

	inline void ProcessServiceTransaction(Rage::joaat_t Category, Rage::joaat_t ItemHash, Rage::joaat_t Action, int Value, int Flag) {
		execute_as_script(joaat("shop_controller"), [&] {
			if (Native::_NetworkShopBasketEnd()) {
				_i<int>(0xFA336E7F40C0A0D0);
			}
			int transaction_id{};

			if (Native::_NetworkShopBeginService(&transaction_id, Category, ItemHash, Action, Value, Flag)) {
				Native::_NetworkShopCheckoutStart(transaction_id);
			}
		});
	}
}