#pragma once
#include "stdafx.hpp"
#include "rage/types/base_types.hpp"
#include <queue>

namespace Menu {
	struct RequestControlContext {
		Entity m_Entity;
		std::function<void(Entity)> m_Callback;
		std::function<void(Entity, int)> m_CallbackWithOwner;
		int m_Tries;
		bool m_TakeOwner;
	};

	struct RequestModelContext {
		uint32_t m_Model;
		std::function<void(uint32_t)> m_Callback;
		int m_Tries;
	};

	struct RequestParticleFXContext {
		std::pair<const char*, const char*> m_Anim;
		std::function<void(std::pair<const char*, const char*>)> m_Callback;
		int m_Tries;
	};

	struct RequestWeaponAssetContext {
		uint32_t m_Model;
		std::function<void(uint32_t)> m_Callback;
		int m_Tries;
	};

	struct RequestAnimationContext {
		const char* m_Animation;
		std::function<void()> m_Callback;
		int m_Tries;
	};

	class ControlManager {
	public:
		void Update();
		void RequestControl(Entity entity, std::function<void(Entity)> Callback, bool TakeOwner = false);
		void RequestControl(Entity entity, std::function<void(Entity, int)> Callback, bool TakeOwner = false);
		void RequestModel(uint32_t Model, std::function<void(uint32_t)> Callback);
		void RequestAnimation(const char* Animation, std::function<void()> Callback);
		void RequestParticleFX(std::pair<const char*, const char*> Anim, std::function<void(std::pair<const char*, const char*>)> Callback);
		void RequestWeaponAsset(uint32_t Model, std::function<void(uint32_t)> Callback);

		void SimpleRequestModel(uint32_t Model);
		void SimpleRequestControl(Entity Ent);
		void SimpleRequestWeaponAsset(uint32_t Model);
		void SimpleRequestDict(const char* Dict);
	private:
		std::queue<RequestControlContext> m_ControlRequests;
		std::queue<RequestModelContext> m_ModelRequests;
		std::queue<RequestParticleFXContext> m_ParticleFX;
		std::queue<RequestWeaponAssetContext> m_WeaponAsset;
		std::queue<RequestAnimationContext> m_Animations;
		// std::vector<uint64_t> m_TakeOwnerTries;
	};

	ControlManager* GetControlManager();
}