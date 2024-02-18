#include "stdafx.hpp"
#include "control_manager.hpp"
#include "rage/invoker/natives.hpp"
#include "util/fiber.hpp"

namespace Menu {
	void ControlManager::Update() {
		if (m_ControlRequests.size()) {
			RequestControlContext& Request = m_ControlRequests.front();

			int OldOwner = 0;
			if (Native::NetworkIsInSession()) {
				Native::NetworkRequestControlOfNetworkId(Native::NetworkGetNetworkIdFromEntity(Request.m_Entity));
				Native::SetNetworkIdCanMigrate(Native::NetworkGetNetworkIdFromEntity(Request.m_Entity), 1);

				/*if (Request.m_TakeOwner) {
					if (Rage::Entity::GetNetworkObjectOwner(Request.m_Entity) != GetLocalPlayer().m_ID) {
						OldOwner = Rage::Entity::GetNetworkObjectOwner(Request.m_Entity);

						uint64_t Managerrr = *(uint64_t*)Global::Vars::g_NetworkObjectManager;
						if (Manager) {
							if (*(uint64_t*)(Manager + 0x2710) > 0xFFFFFFFF) {
								if (!Native::IsPedAPlayer(Request.m_Entity)) {
									Rage::Entity::UpdateNetworkObjectOwner(Request.m_Entity, GetLocalPlayer().m_ID);
								}
							}
						}
					}
				}*/
			}

			for (int i = 0; i < 50; i++) {
				Native::NetworkRequestControlOfEntity(Request.m_Entity);
				if (/*Rage::Entity::GetNetworkObjectOwner(Request.m_Entity) == GetLocalPlayer().m_ID || */Native::NetworkHasControlOfEntity(Request.m_Entity)) {
					if (Request.m_Callback) Request.m_Callback(Request.m_Entity);
					if (Request.m_CallbackWithOwner) Request.m_CallbackWithOwner(Request.m_Entity, OldOwner);
					break;
				}
			}

			m_ControlRequests.pop();
		}

		if (m_ModelRequests.size()) {
			RequestModelContext& Request = m_ModelRequests.front();
			if (Native::HasModelLoaded(Request.m_Model) || Request.m_Tries > 30) {
				if (Request.m_Tries < 30) {
					Request.m_Callback(Request.m_Model);
					// Native::SetModelAsNoLongerNeeded(Request.m_Model);
				}

				m_ModelRequests.pop();
			}
			else {
				Request.m_Tries++;
				Native::RequestModel(Request.m_Model);
			}
		}

		if (m_ParticleFX.size()) {
			RequestParticleFXContext& Request = m_ParticleFX.front();
			if (Native::HasNamedPtfxAssetLoaded(Request.m_Anim.first) || Request.m_Tries > 30) {
				if (Request.m_Tries < 30) {
					Request.m_Callback(Request.m_Anim);
				}

				m_ParticleFX.pop();
			}
			else {
				Request.m_Tries++;
				Native::RequestNamedPtfxAsset(Request.m_Anim.first);
			}
		}

		if (m_WeaponAsset.size()) {
			RequestWeaponAssetContext& Request = m_WeaponAsset.front();
			if (Native::HasWeaponAssetLoaded(Request.m_Model) || Request.m_Tries > 30) {
				if (Request.m_Tries < 30) {
					Request.m_Callback(Request.m_Model);
				}

				m_WeaponAsset.pop();
			}
			else {
				Request.m_Tries++;
				Native::RequestWeaponAsset(Request.m_Model, 31, 0);
			}
		}

		if (m_Animations.size()) {
			RequestAnimationContext& Request = m_Animations.front();
			if (Native::HasAnimDictLoaded(Request.m_Animation) || Request.m_Tries > 30) {
				if (Request.m_Tries < 30) {
					Request.m_Callback();
				}

				m_Animations.pop();
			}
			else {
				Request.m_Tries++;
				Native::RequestAnimDict(Request.m_Animation);
			}
		}
	}

	void ControlManager::RequestControl(Entity entity, std::function<void(Entity)> Callback, bool TakeOwner) {
		if (!Native::NetworkIsInSession()) {
			Callback(entity);
		}
		else {
			m_ControlRequests.push({ entity, Callback, nullptr, 0, TakeOwner });
		}
	}

	void ControlManager::RequestControl(Entity entity, std::function<void(Entity, int)> Callback, bool TakeOwner) {
		m_ControlRequests.push({ entity, nullptr, Callback, 0, TakeOwner });
	}

	void ControlManager::RequestModel(uint32_t Model, std::function<void(uint32_t)> Callback) {
		m_ModelRequests.push({ Model, Callback, 0 });
	}

	void ControlManager::RequestAnimation(const char* Animation, std::function<void()> Callback) {
		m_Animations.push({ Animation, Callback, 0 });
	}

	void ControlManager::RequestParticleFX(std::pair<const char*, const char*> Anim, std::function<void(std::pair<const char*, const char*>)> Callback) {
		m_ParticleFX.push({ Anim, Callback, 0 });
	}

	void ControlManager::RequestWeaponAsset(uint32_t Model, std::function<void(uint32_t)> Callback) {
		m_WeaponAsset.push({ Model, Callback, 0 });
	}

	void ControlManager::SimpleRequestModel(uint32_t Model) {
		int Tries = 0;
		while (!Native::HasModelLoaded(Model) && Tries < 25) {
			Native::RequestModel(Model);
			Tries++;
			Utils::GetFiberManager()->GoToMainFiber();
		}
	}

	void ControlManager::SimpleRequestDict(const char* Dict) {
		int Tries = 0;
		while (!Native::HasStreamedTextureDictLoaded(Dict) && Tries < 30) {
			Native::RequestStreamedTextureDict(Dict, false);
			Tries++;
			Utils::GetFiberManager()->GoToMainFiber();
		}
	}

	void ControlManager::SimpleRequestWeaponAsset(uint32_t Model) {
		int Tries = 0;
		while (!Native::HasWeaponAssetLoaded(Model) && Tries < 30) {
			Native::RequestWeaponAsset(Model, 31, 0);
			Tries++;
			Utils::GetFiberManager()->GoToMainFiber();
		}
	}

	void ControlManager::SimpleRequestControl(Entity Ent) {
		int Tries = 0;
		while (Native::DoesEntityExist(Ent) && !Native::NetworkHasControlOfEntity(Ent) && Tries < 30 && Native::NetworkIsSessionActive()) {
			Native::NetworkRequestControlOfEntity(Ent);
			if (Native::NetworkIsSessionStarted()) {
				Native::NetworkRequestControlOfNetworkId(Native::NetworkGetNetworkIdFromEntity(Ent));
				Native::SetNetworkIdCanMigrate(Native::NetworkGetNetworkIdFromEntity(Ent), 1);
			}

			Utils::GetFiberManager()->GoToMainFiber();
			Tries++;
		}
	}

	ControlManager* GetControlManager() {
		static ControlManager Instance;
		return &Instance;
	}
}