#pragma once
#include "stdafx.hpp"
#include "rage/classes/script/scrNativeRegistration.hpp"
#include "rage/classes/texture/grcTextureFactory.hpp"
#include "rage/classes/texture/grcTextureStore.hpp"
#include "rage/classes/ped/CPedFactory.hpp"
#include "rage/classes/script/scrProgramTable.hpp"
#include "util/math.hpp"
#include "rage/types/pool_types.hpp"
#include "rage/classes/base/HashTable.hpp"
#include "rage/classes/vehicle/CTrainConfig.hpp"
#include "rage/classes/network/netShopping.hpp"
#include "rage/classes/script/GtaThread.hpp"

#include "rage/types/network_types.hpp"
//#include "rage/classes/network/Network.hpp"
struct CWaypointData {
	Math::Vector4<float>* m_points;
	char _0x0008[0x3C];
	uint32_t m_count;
};
class CNetworkPlayerMgr;
class Network;
class CNetworkObjectMgr;

namespace Patterns {
	class Patterns {
	public:
		void Init();
	private:
		void PreGame();
		void AntiCheat();
		void InGame();
	};

	inline Patterns* GetPatterns() {
		static Patterns instance;
		return &instance;
	}

	//pattern variables
	namespace Vars {
		inline u32* g_GameState;
		inline u64 g_ScriptVm;
		inline u64 g_SendMetric;
		inline u64 g_SendHttpRequest;
		inline u64 g_AddEventToList;
		inline u64 g_SendNetworkEvent;
		inline u64 g_GameSkeleton;
		inline u64 g_QueueDependency;

		inline u64 g_FallTaskCreate;
		inline u64 g_JumpTaskCreate;

		inline CPedFactory** g_PedFactory;

		inline u64 g_InitNativeTables;
		inline Rage::scrProgramTable* g_ProgramTable;

		inline u64** g_GlobalCache;

		inline CWaypointData* g_WaypointData;
		inline u64 g_GetEventData;
		inline u64 g_HandleNetworkEvent;

		inline IDXGISwapChain** g_SwapChain;
		inline Rage::grcTextureStore* g_TextureStore;
		inline Rage::grcTextureFactory** g_TextureFactory;
		inline u64 g_RegisterStreamingFile;
		inline u64 g_RequestStreamingFile;

		using setup_dictionary_t = Rage::pgDictionary* (*)(Rage::pgDictionary* dict, int size);
		using add_texture_to_dictionary_t = void(*)(Rage::pgDictionary* dict, uint32_t hash, Rage::grcTexture* texture);
		using setup_store_item_t = uint32_t * (*)(Rage::grcTextureStore* store, uint32_t* out, uint32_t* dictHash);
		using add_dictionary_to_pool_t = void(*)(Rage::grcTextureStore* store, uint32_t slot, Rage::pgDictionary* dict);

		inline setup_dictionary_t g_SetupDictionary;
		inline add_texture_to_dictionary_t g_AddTextureToDictionary;
		inline setup_store_item_t g_SetupStoreItem;
		inline add_dictionary_to_pool_t g_AddDictionaryToPool;

		inline u64 g_ReadDwordFromBitBuffer;
		inline u64 g_ReadArrayFromBitBuffer;
		inline u64 g_ReadStringFromBitBuffer;
		inline u64 g_ReadBoolFromBitBuffer;

		inline u64 g_WriteDwordToBitBuffer;
		inline u64 g_WriteQWordToBitBuffer;
		inline u64 g_WriteInt64ToBitBuffer;
		inline u64 g_WriteInt32ToBitBuffer;
		inline u64 g_WriteBoolToBitBuffer;
		inline u64 g_WriteArrayToBitBuffer;

		inline u64 g_SendEventAck;

		inline Rage::NativeRegistration** g_NativeRegistration;

		inline u64 g_CanApplyData;

		inline u64 g_ParseNodes;
		inline u64 g_ReceiveCloneCreate;
		inline u64 g_ReceiveCloneSync;
		inline u64 g_GetSyncTree;
		inline u64 g_GetNetObject;
		inline CNetworkObjectMgr** g_NetworkObjectMgr;

		inline u64 g_GetNetworkObjectFromNetworkID;

		inline HashTable<CBaseModelInfo*>* g_ModelTable;
		inline Rage::atArray<CTrainConfig>* g_TrainConfigArray;

		inline GenericPool** g_InteriorPool;

		inline Rage::netShopping::CNetworkShoppingMgr* g_NetShopMgr;
		inline u64 g_ConstructBasket;
		inline u64 g_AddItemToBasket;

		inline Rage::atArray<GtaThread*>* g_ScriptThreads;

		inline u64 g_FormatGameString;
		inline u64 g_GetGameStringLineCount;
		inline u64 g_GetEntityAddress;

		inline u64 g_ApplyPlayerPhysicalIndexHook;
		inline u64 g_TriggerScriptEvent;
		
		inline uint64_t g_GetModelInfo;
		inline u64 g_SetNetworkedThread;
		inline Rage::HashList* g_GetHashList;
		inline u64 g_GetWeaponHastList;
		inline u64 g_GetWeaponHastListCount;

		inline TimeCycle* g_TimeCycleMgr;

		inline u64 g_RemoveRefrence;

		inline u64 g_FragmentPatch;
		inline u64 g_InvalidDecalPatch;
		inline u64 g_InvalidModsPatch;
		inline u64 g_RenderBigPed;
		inline u64 g_RenderEntity;
		inline u64 g_RenderPed;
		inline u64 g_AmbientTaskPatch;
		inline u64 g_ParachuteTaskPatch;
		inline u64 g_SeralizeParachuteTask;
		inline u64 g_SeralizePedVariation;
		inline u64 g_KanyePatch;
		inline u64 g_AnimTaskPatch;
		inline u64 g_StandPatch;

		inline u64* g_DrawHandlerMgr;

		inline u64 g_VehicleControlOutOfBoundsPatch;
		inline u64 g_GetNetworkGamePlayer;

		inline u64 g_ChangeNetworkObjectOwner;
		inline u64 g_GetEntityFromScript;

		inline CNetworkPlayerMgr** g_NetworkPlayerMgr;
		inline Rage::Network::NetworkBaseConfig* g_NetworkBaseConfig;
		inline u64 g_JoinSessionByInfo;
		inline Network** g_Network;
		inline u64 g_ConstructJoinMessage;

		inline u64 g_GetFriendsMenu;
		inline u64 g_ConstructPlayerMenu;

		inline u64 g_HandleNetMessage;
		//inline u64 g_WriteGsItem;

		inline u64 g_GetConnectionPeer;
		inline u64 g_QueuePacket;
		inline u64 g_SendPacket;
		inline u64 g_QueuePacketWithPlayer;

		inline u64 g_CompressDataNode;
		inline u64 g_CompressDataNode2;


		//for spoofing
		inline u64 g_SyncPedOrientation;
		inline u64 g_SyncEntityMatrix;
		inline u64 g_PedMovement;
		inline u64 g_HandleRotationValuesFromOrder;
	}
}