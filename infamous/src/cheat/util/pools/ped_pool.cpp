#include "stdafx.hpp"
#include "ped_pool.hpp"
#include "rage/engine.hpp"
#include "rage/invoker/natives.hpp"
#include "cheat/util/player_manager.hpp"
#include "cheat/util/control_manager.hpp"
#include "core/core.hpp"

namespace Menu::Pools {
	void PedPool::Update() {
		std::vector<Ped> Peds;

		auto Pool = Patterns::Vars::g_PedPool;
		if (Pool) {
			if (!Pool->m_Address) return;
			if (!Pool->m_MaskArray) return;
			if (Pool->m_Count > 2000) return;

			for (uint32_t i = 0; i < Pool->m_Count; i++) {
				if (Patterns::Vars::g_EntityPool) {
					if (Patterns::Vars::g_EntityPool->IsFull()) {
						break;
					}
				}

				uint64_t Address = Pool->GetEntity(i);
				if (Address) {
					Entity Handle = Engine::GetEntityHandleFromAddress(Address);
					if (Handle) {
						Peds.push_back(Handle);
					}
				}
			}
		}

		for (std::size_t i = 0; i < Peds.size(); i++) {
			if (std::find(m_Storage.begin(), m_Storage.end(), Peds[i]) == m_Storage.end()) {
				m_Storage.push_back(Peds[i]);
			}
		}

		for (std::size_t i = 0; i < m_Storage.size(); i++) {
			if (!Native::DoesEntityExist(m_Storage[i])) {
				m_Storage.erase(m_Storage.begin() + i);
			}
		}
	}

	void PedPool::Run(std::function<void(Ped)> Callback, bool RequestControl) {
		Update();

		for (std::size_t i = 0; i < m_Storage.size(); i++) {
			if (m_Storage[i]) {
				if (Native::DoesEntityExist(m_Storage[i])) {
					if (m_Storage[i] != Menu::GetLocalPlayer().m_Ped) {
						if (RequestControl) {
							Menu::GetControlManager()->RequestControl(m_Storage[i], [=](Entity E) {
								Callback(E);
								}, !Native::IsPedAPlayer(m_Storage[i]));
						}
						else {
							Callback(m_Storage[i]);
						}
					}
				}
			}
		}
	}

	PedPool* GetPedPool() {
		static PedPool Instance;
		return &Instance;
	}
}