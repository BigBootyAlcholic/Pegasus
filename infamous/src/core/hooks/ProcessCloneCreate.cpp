#include "stdafx.hpp"
#include "../hooks.hpp"
#include "rage/classes/enums/enums.hpp"
#include "rage/classes/rage/joaat.hpp"
#include "rage/classes/netsync/netSyncTree.hpp"
#include "util/caller.hpp"
#include "rage/classes/enums/enums.hpp"
#include "rage/classes/netsync/data_nodes.hpp"
#include "rage/classes/base/CObject.hpp"
#include "cheat/util/notify.hpp"
#include "SyncData.hpp"
#include "rage/engine.hpp"
#include "util/fiber.hpp"
#include "util/fiber_pool.hpp"
#include "cheat/util/control_manager.hpp"
#include "cheat/menu/submenus/protection.hpp"
namespace Hooks {

	void ParseSyncNodes(Rage::netSyncTree* tree, int type_bit, int flag, Rage::datBitBuffer* buffer, uint64_t log_interface) {
		Caller::Call<void>(Patterns::Vars::g_ParseNodes, tree, type_bit, flag, buffer, log_interface);
	}

	struct CreateCacheType {
		uint32_t m_LastSpawn = 0;
		bool m_SusLastSpawn = false;
		int m_SusCount = 0;
	};

	struct CreateCache {
		std::unordered_map<eNetObjType, CreateCacheType> m_Cache;
	};

	static std::unordered_map<Player, CreateCache> g_CreateCache;

	void ProcessCloneCreateHook(CNetworkObjectMgr* _this, 
								CNetGamePlayer* fromPlayer, 
								CNetGamePlayer* toPlayer,
								eNetObjType objectType,
								int32_t objectID,
								int32_t objetFlag,
								Rage::datBitBuffer* msgBuffer,
								int32_t timeStamp)
	{

		
		Rage::netSyncTree* tree = Caller::Call<Rage::netSyncTree*>(Patterns::Vars::g_GetSyncTree, _this, objectType);
		if (!tree) {
			LOG_ERROR("No Creation Tree Found");
			return;
		}

	/*	Rage::netObject* netObject = Caller::Call<Rage::netObject*>(Patterns::Vars::g_GetNetObject, _this, objectID, true);
		if (netObject) {
			if (netObject->m_owner_id == Menu::GetLocalPlayer().m_ID) {
				Entity Handle = Engine::GetEntityAddress(netObject->m_owner_id);
			}
		}*/

	/*	Rage::netObject* netObject = Caller::Call<Rage::netObject*>(Patterns::Vars::g_GetNetObject, _this, objectID, true);
		if (netObject) {
			if (netObject->m_owner_id == Native::PlayerId()) {
				if (netObject->GetGameObject()) {
					//TODO
				}
			}
		}
*/
		if (objectType < eNetObjType::NET_OBJ_TYPE_AUTOMOBILE || objectType > eNetObjType::NET_OBJ_TYPE_TRAIN) {
			LOG_ERROR("Out of Bounds Crash");
			Menu::Notify::stacked(std::format("Blocked Invalid Object Type From {}", fromPlayer->get_name()).c_str());
			return;
		}

		/*auto& ProtectVars = ProtectionMenuVars::m_Vars;
		if (ProtectVars.m_Limits.size()) {
			auto Vit = std::find_if(ProtectVars.m_Limits.begin(), ProtectVars.m_Limits.end(), [=](ProtectionMenuVars::EntityLimit& Limit) {
			});

			if (Vit != ProtectVars.m_Limits.end()) {
				if (Vit->m_Enabled) {
					if ((GetTickCount() - Vit->m_LastSpawned) > (uint32_t)Vit->m_Delay]) {
						Vit->m_LastSpawned = GetTickCount();
					}
					else {
						LOG_WARN("Preventing Creation Due To Entity Limit")
						return;
					}
				}
			}
		}
*/

		if (objectType == eNetObjType::NET_OBJ_TYPE_PED || objectType == eNetObjType::NET_OBJ_TYPE_OBJECT) {
			if (Native::NetworkIsSessionActive() && Menu::GetLocalPlayer().m_Connected) {
				CreateCacheType& Cache = g_CreateCache[fromPlayer->m_player_id].m_Cache[objectType];
				if (Cache.m_LastSpawn == 0) {
					Cache.m_LastSpawn = GetTickCount();
				}
				else {
					if ((GetTickCount() - Cache.m_LastSpawn < 500)) {
						Cache.m_SusLastSpawn = true;
						Cache.m_SusCount++;
					}
					else {
						Cache.m_SusCount = 0;
						Cache.m_SusLastSpawn = false;
					}
					Cache.m_LastSpawn = GetTickCount();

					if (Cache.m_SusCount >= 20 && Cache.m_SusLastSpawn) {
						LOG(std::format("{} Might Be Spamming, Deleting Entities", fromPlayer->get_name()).c_str());

						Utils::GetFiberPool()->Push([=] {
							int Arr[100];
							memset(Arr, 0, sizeof(Arr));
							Arr[0] = 100;

							if (objectType == eNetObjType::NET_OBJ_TYPE_PED) {
								int Size = Native::GetPedNearbyPeds(Menu::GetLocalPlayer().m_Ped, Arr, Menu::GetLocalPlayer().m_Ped);
								LOG_WARN("Size: %i", Size);

								for (int i = 0; i < Size; i++) {
									if (Arr[i] && Native::DoesEntityExist(Arr[i])) {
										if (Native::IsPedAPlayer(Arr[i])) continue;

										LOG_WARN("Deleting ped %x", Arr[i]);

										Menu::GetControlManager()->SimpleRequestControl(Arr[i]);
										Native::SetEntityAsMissionEntity(Arr[i], true, true);
										Native::DeletePed(&Arr[i]);
									}
								}
							}
						});
					}
				}
			}
		}

		//for can apply data
		g_SyncingPlayer = fromPlayer;
		g_SyncingObjectType = objectType;

		return OgProcessCloneCreateHook(_this, fromPlayer, toPlayer, objectType, objectID, objetFlag, msgBuffer, timeStamp);

	}
}