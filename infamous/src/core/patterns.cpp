#include "stdafx.hpp"
#include "patterns.hpp"
#include "util/memory/pattern.hpp"
#include "util/memory/memory.hpp"
#include "cheat/util/pools/pool_manager.hpp"
namespace Patterns {

	u64 GetAddressInMemory(std::string ptr) {
		return (u64)Base::Memory::pattern(ptr).get(0).get<char>(0);
	}

	void Patterns::PreGame() {
		Memory::Batch Batch;
		bool out = true;

		//game state
		Batch.Add({ "GS", "83 3D ? ? ? ? ? 75 17 8B 43 20" }, [](Memory::Ptr ptr) {
			Vars::g_GameState = ptr.Lea().As<decltype(Vars::g_GameState)>();
			return IsValidPtr(Vars::g_GameState);
		}, out);

		//script vm
		Batch.Add({ "SVM", "E8 ? ? ? ? 48 85 FF 48 89 1D" }, [](Memory::Ptr ptr) {
			Vars::g_ScriptVm = ptr.Call().As<decltype(Vars::g_ScriptVm)>();
			return IsValidPtr(Vars::g_ScriptVm);
		}, out);

		//native registeration
		Batch.Add({ "NRT", "48 8D 0D ? ? ? ? 48 8B 14 FA E8" }, [](Memory::Ptr ptr) {
			Vars::g_NativeRegistration = ptr.FromInstruction().As<Rage::NativeRegistration**>();
			return IsValidPtr(Vars::g_NativeRegistration);
		}, out);

		//swapchain
		Batch.Add({ "SC", "48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8" }, [](Memory::Ptr ptr) {
			Vars::g_SwapChain = ptr.FromInstruction().As<decltype(Vars::g_SwapChain)>();
			//return IsValidPtr(Vars::g_SwapChain);
		}, out);

		Batch.Add({ ("SD"), ("E8 ? ? ? ? EB 03 48 8B C3 48 89 47 18") }, [](Memory::Ptr ptr) {
			Vars::g_SetupDictionary = ptr.Call().As<decltype(Vars::g_SetupDictionary)>();
		}, out);

		Batch.Add({ ("ATTD"), ("E8 ? ? ? ? 84 C0 74 1F 48 8D 4F 30") }, [](Memory::Ptr ptr) {
			Vars::g_AddTextureToDictionary = ptr.Call().As<decltype(Vars::g_AddTextureToDictionary)>();
		}, out);

		Batch.Add({ ("SSI"), ("E8 ? ? ? ? 8B 18 48 8B 45 07") }, [](Memory::Ptr ptr) {
			Vars::g_SetupStoreItem = ptr.Call().As<decltype(Vars::g_SetupStoreItem)>();
		}, out);

		Batch.Add({ ("ADTP"), ("89 54 24 10 48 83 EC 28 48 8B 41 40 4C 63 CA 46 0F B6 14 08 8B 41 4C 41 81 E2 ? ? ? ? 45 8B CA 0F AF C2") }, [](Memory::Ptr ptr) {
			Vars::g_AddDictionaryToPool = ptr.As<decltype(Vars::g_AddDictionaryToPool)>();
		}, out);

		Batch.Add({ ("TS"), ("48 8D 0D ? ? ? ? E8 ? ? ? ? 8B 45 EC 4C 8D 45 F0 48 8D 55 EC 48 8D 0D ? ? ? ? 89 45 F0 E8") }, [](Memory::Ptr ptr) {
			Vars::g_TextureStore = ptr.FromInstruction().As<decltype(Vars::g_TextureStore)>();
		}, out);

		Batch.Add({ ("TF"), ("48 8B 0D ? ? ? ? 45 33 C0 48 8B 01 33 D2 FF 90 ? ? ? ? 48 8B 0D ? ? ? ? 83 64 24") }, [](Memory::Ptr ptr) {
			Vars::g_TextureFactory = ptr.FromInstruction().As<decltype(Vars::g_TextureFactory)>();
		}, out);

		Batch.Add({ ("RSF"), ("48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50 48 8B EA 4C 8B FA 48 8B D9 4D 85 C9 B2") }, [](Memory::Ptr ptr) {
			Vars::g_RegisterStreamingFile = ptr.As<u64>();
		}, out);

		Batch.Add({ ("RSF2"), ("E8 ? ? ? ? 83 4F 48 10") }, [](Memory::Ptr ptr) {
			Vars::g_RequestStreamingFile = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("JTC"), ("48 89 5C 24 ? 89 54 24 10 57 48 83 EC 30 0F 29 74 24") }, [](Memory::Ptr ptr) {
			Vars::g_JumpTaskCreate = ptr.As<u64>();
		}, out);

		Batch.Add({ ("FTC"), ("E8 ? ? ? ? B3 04 08 98 A0") }, [](Memory::Ptr ptr) {
			Vars::g_FallTaskCreate = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("WP"), ("48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81") }, [](Memory::Ptr ptr) {
			Vars::g_PedFactory = ptr.FromInstruction().As<CPedFactory**>();
		}, out);

		Batch.Add({ ("GC"), ("4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11") }, [](Memory::Ptr ptr) {
			Vars::g_GlobalCache = ptr.FromInstruction().As<u64**>();
		}, out);

		Batch.Add({ ("WD"), ("48 8D 0D ? ? ? ? 48 98 4C 8B C6 41 8B 44 85 ? 0D ? ? ? ? 89 44 24 30 83 64 24 ? ? C6 44 24") }, [](Memory::Ptr ptr) {
			Vars::g_WaypointData = ptr.FromInstruction().As<CWaypointData*>();
		}, out);

		Batch.Add({ ("GED"), ("53 43 52 49 50 54 5F 4E 45 54 57 4F 52 4B") }, [](Memory::Ptr ptr) {
			Vars::g_GetEventData = *ptr.Sub(0x38).As<u64*>();
		}, out);

		Batch.Add({ ("HNE"), ("66 41 83 F9 ? 0F 83") }, [](Memory::Ptr ptr) {
			Vars::g_HandleNetworkEvent = ptr.As<u64>();
		}, out);

		Batch.Add({ ("RBWD"), ("48 89 74 24 ? 57 48 83 EC 20 48 8B D9 33 C9 41 8B F0 8A") }, [](Memory::Ptr ptr) {
			Vars::g_ReadDwordFromBitBuffer = ptr.Sub(5).As<u64>();
		}, out);

		Batch.Add({ ("RBA"), ("48 89 5C 24 ? 57 48 83 EC 30 41 8B F8 4C") }, [](Memory::Ptr ptr) {
			Vars::g_ReadArrayFromBitBuffer = ptr.As<u64>();
		}, out);

		Batch.Add({ ("RBS"), ("48 89 5C 24 08 48 89 6C 24 18 56 57 41 56 48 83 EC 20 48 8B F2 45") }, [](Memory::Ptr ptr) {
			Vars::g_ReadStringFromBitBuffer = ptr.As<u64>();
		}, out);

		Batch.Add({ ("RBB"), ("48 8B C4 48 89 58 08 55 56 57 48 83 EC 20 48 83 60") }, [](Memory::Ptr ptr) {
			Vars::g_ReadBoolFromBitBuffer = ptr.As<u64>();
		}, out);

		Batch.Add({ ("WBD"), ("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 20 8B EA BF 01") }, [](Memory::Ptr ptr) {
			Vars::g_WriteDwordToBitBuffer = ptr.As<u64>();
		}, out);

		Batch.Add({ ("WBQ"), ("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 41 8B F0 48 8B EA 48 8B D9 41 83 F8 20") }, [](Memory::Ptr ptr) {
			Vars::g_WriteQWordToBitBuffer = ptr.As<u64>();
		}, out);

		Batch.Add({ ("WBUL"), ("E8 ? ? ? ? 8A 53 39 48 8B CF") }, [](Memory::Ptr ptr) {
			Vars::g_WriteInt64ToBitBuffer = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("WBSL"), ("E8 ? ? ? ? 8A 53 74") }, [](Memory::Ptr ptr) {
			Vars::g_WriteInt32ToBitBuffer = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("WBB"), ("E8 ? ? ? ? 8A 57 39") }, [](Memory::Ptr ptr) {
			Vars::g_WriteBoolToBitBuffer = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("WBA"), ("E8 ? ? ? ? 01 7E 08") }, [](Memory::Ptr ptr) {
			Vars::g_WriteArrayToBitBuffer = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("SEA"), ("E8 ? ? ? ? 66 83 7B 08 5B") }, [](Memory::Ptr ptr) {
			Vars::g_SendEventAck = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("INT"), ("8B CB E8 ? ? ? ? 8B 43 70 ? 03 C4 A9 00 C0 FF FF") }, [](Memory::Ptr ptr) {
			Vars::g_InitNativeTables = ptr.Add(3).Rip().As<u64>();
		}, out);

		Batch.Add({ ("PT"), ("48 8B 1D ? ? ? ? 41 83 F8 FF") }, [](Memory::Ptr ptr) {
			Vars::g_ProgramTable = ptr.FromInstruction().As<decltype(Vars::g_ProgramTable)>();
		}, out);

		Batch.Add({ ("CNOM"), ("48 8B 0D ? ? ? ? 45 33 C0 E8 ? ? ? ? 33 FF 4C 8B F0") }, [](Memory::Ptr ptr) {
			Vars::g_NetworkObjectMgr = ptr.FromInstruction().As<decltype(Vars::g_NetworkObjectMgr)>();
		}, out);

		Batch.Add({ ("IPG"), ("4C 8B 05 ? ? ? ? 4C 0F BF 0B") }, [](Memory::Ptr ptr) {
			Vars::g_InteriorPool = ptr.FromInstruction().As<GenericPool**>();
		}, out);

		Batch.Add({ ("NPM"), ("48 8B 0D ? ? ? ? 8A D3 48 8B 01 FF 50 ? 4C 8B 07 48 8B CF") }, [](Memory::Ptr ptr) {
			Vars::g_NetworkPlayerMgr = ptr.FromInstruction().As<CNetworkPlayerMgr**>();
		}, out);

		Batch.Add({ ("GNOFNI"), ("48 89 5C 24 ? 48 89 6C 24 ? 66 89 54 24 ? 56 57 41 56 48 83 EC 20 45") }, [](Memory::Ptr ptr) {
			Vars::g_GetNetworkObjectFromNetworkID = ptr.As<u64>();
		}, out);

		Batch.Add({ ("MT"), ("4C 03 05 ? ? ? ? EB 03") }, [](Memory::Ptr ptr) {
			Vars::g_ModelTable = ptr.FromInstruction().As<HashTable<CBaseModelInfo*>*>();
		}, out);

		Batch.Add({ ("TCA"), ("48 8D 0D ? ? ? ? E8 ? ? ? ? 44 88 64 24 30 4C 8D 8C 24 60 02 00 00") }, [](Memory::Ptr ptr) {
			Vars::g_TrainConfigArray = ptr.FromInstruction().As<Rage::atArray<CTrainConfig>*>();
		}, out);

		Batch.Add({ ("PSN"), ("E8 ? ? ? ? 48 8B BC 24 B0 00 00 00") }, [](Memory::Ptr ptr) {
			Vars::g_ParseNodes = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("RCC"), ("48 8B C4 66 44 89 48") }, [](Memory::Ptr ptr) {
			Vars::g_ReceiveCloneCreate = ptr.As<u64>();
		}, out);

		Batch.Add({ ("RCR"), ("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 54 41 56 41 57 48 83 EC 50 4C 8B F2 4D 8B E0 48") }, [](Memory::Ptr ptr) {
			Vars::g_ReceiveCloneRemove = ptr.As<u64>();
		}, out);

		Batch.Add({ ("RCS"), ("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 83 EC 40 4C 8B EA") }, [](Memory::Ptr ptr) {
			Vars::g_ReceiveCloneSync = ptr.As<decltype(Vars::g_ReceiveCloneSync)>();
			Vars::g_GetSyncTree = ptr.Add(0x2F).Call().As<decltype(Vars::g_GetSyncTree)>();
			Vars::g_GetNetObject = ptr.Add(0x109).Call().As<decltype(Vars::g_GetNetObject)>();
		}, out);

		Batch.Add({ ("CAD"), ("E8 ? ? ? ? 84 C0 0F 84 AF 01 00 00 48 8B 03") }, [](Memory::Ptr ptr) {
			Vars::g_CanApplyData = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("PCC2"), ("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B F9 48 8B CA 49 8B E9") }, [](Memory::Ptr ptr) {
			Vars::g_PackCloneCreate = ptr.As<u64>();
		}, out);

		Batch.Add({ ("PCS2"), ("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 02") }, [](Memory::Ptr ptr) {
			Vars::g_PackCloneSync = ptr.As<u64>();
		}, out);

		Batch.Add({ ("SCS"), ("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F B7 42") }, [](Memory::Ptr ptr) {
			Vars::g_SendCloneSync = ptr.As<u64>();
		}, out);

		Batch.Add({ ("NS_NSM"), ("48 8B 0D ? ? ? ? 41 8A D9") }, [](Memory::Ptr ptr) {
			Vars::g_NetShopMgr = *ptr.Add(3).Rip().As<Rage::netShopping::CNetworkShoppingMgr**>();		
		}, out);

		Batch.Add({ ("NS_CB"), ("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 54 41 56 41 57 48 83 EC 30 48 8B F1") }, [](Memory::Ptr ptr) {
			Vars::g_ConstructBasket = ptr.As<decltype(Vars::g_ConstructBasket)>();
		}, out);

		Batch.Add({ ("NS_AITB"), ("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B F2 48 8D 54 24") }, [](Memory::Ptr ptr) {
			Vars::g_AddItemToBasket = ptr.As<decltype(Vars::g_AddItemToBasket)>();
			//return hooking::detour("NS_AITB", patterns::add_item_to_basket, &addItemToBasketHook, &ogAddItemToBasketHook);
		}, out);

		Batch.Add({ ("GT"), ("45 33 F6 8B E9 85 C9 B8") }, [](Memory::Ptr ptr) {
			Vars::g_ScriptThreads = ptr.Sub(4).Rip().Sub(8).As<decltype(Vars::g_ScriptThreads)>();
		}, out);

		Batch.Add({ ("FGS"), ("E8 ? ? ? ? 8B 44 24 78 4C 8B 4C 24 ? 44 8B C7") }, [](Memory::Ptr ptr) {
			Vars::g_FormatGameString = ptr.Call().As<decltype(Vars::g_FormatGameString)>();
		}, out);

		Batch.Add({ ("GGSLC"), ("E8 ? ? ? ? 8B F8 48 8B 55 B8") }, [](Memory::Ptr ptr) {
			Vars::g_GetGameStringLineCount = ptr.Call().As<decltype(Vars::g_GetGameStringLineCount)>();
		}, out);

		Batch.Add({ ("APPI"), ("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC 20 41 8A E8 4C 8B F2 48 8B F1") }, [](Memory::Ptr ptr) {
			Vars::g_ApplyPlayerPhysicalIndexHook = ptr.As<decltype(Vars::g_ApplyPlayerPhysicalIndexHook)>();
		}, out);

		Batch.Add({ ("TSE"), ("48 8B C4 48 89 58 08 44 89 48 20 55 56 57 48 83 EC 30") }, [](Memory::Ptr ptr) {
			Vars::g_TriggerScriptEvent = ptr.As<decltype(Vars::g_TriggerScriptEvent)>();
		}, out);

		Batch.Add({ ("GMI"), ("41 3B 0A 74 54") }, [](Memory::Ptr ptr) {
			Vars::g_GetModelInfo = ptr.Sub(46).As<decltype(Vars::g_GetModelInfo)>();
		}, out);

		Batch.Add({ ("SNT"), ("48 8B C8 FF 52 30 84 C0 74 05 48") }, [](Memory::Ptr ptr) {
			Vars::g_SetNetworkedThread = ptr.Add(8).As<decltype(Vars::g_SetNetworkedThread)>();
			Memory::Nop(Vars::g_SetNetworkedThread, 2);
		}, out);

		Batch.Add({ ("GHL"), ("4C 03 05 ? ? ? ? EB 09 49 83 C2 04 EB 8B") }, [](Memory::Ptr ptr) {
			Vars::g_GetHashList = ptr.FromInstruction().As<decltype(Vars::g_GetHashList)>();
		}, out);

		Batch.Add({ ("GWH"), ("8B 05 ? ? ? ? 44 8B D3 8D 48 FF 85 C9 78 35 46") }, [](Memory::Ptr ptr) {
			Vars::g_GetWeaponHastListCount = ptr.Lea().As<decltype(Vars::g_GetWeaponHastListCount)>();
			Vars::g_GetWeaponHastList = ptr.FromInstruction().Add(0x14).As<decltype(Vars::g_GetWeaponHastList)>();
		}, out);

		Batch.Add({ ("DHM"), ("48 89 05 ? ? ? ? EB 07 48 89 1D ? ? ? ? 48 8B CB") }, [](Memory::Ptr ptr) {
			Vars::g_DrawHandlerMgr = ptr.FromInstruction().As<u64*>();
		}, out);

		Batch.Add({ ("RR"), ("48 89 5C 24 08 57 48 83 EC 20 80 3D ? ? ? ? ? 48 8B FA 48 8B D9 74 13") }, [](Memory::Ptr ptr) {
			Vars::g_RemoveRefrence = ptr.As<u64>();
		}, out);

		Batch.Add({ ("GNGP"), ("48 83 EC ? 33 C0 38 05 ? ? ? ? 74 ? 83 F9") }, [](Memory::Ptr ptr) {
			Vars::g_GetNetworkGamePlayer = ptr.As<u64>();
		}, out);

		Batch.Add({ ("GEA"), ("E8 ? ? ? ? 48 8B D8 48 85 C0 0F 84 ? ? ? ? 48 8B 0D ? ? ? ? 8B D7 E8 ? ? ? ? 48 8B F8 48 85 C0") }, [](Memory::Ptr ptr) {
			Vars::g_GetEntityAddress = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("CNOW"), ("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 54 41 56 41 57 48 81 EC ? ? ? ? 44 8A 62 4B") }, [](Memory::Ptr ptr) {
			Vars::g_ChangeNetworkObjectOwner = ptr.As<u64>();
		}, out);

		Batch.Add({ ("GEFS"), ("E8 ? ? ? ? 8D 53 01 33 DB") }, [](Memory::Ptr ptr) {
			Vars::g_GetEntityFromScript = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("NBC"), ("48 8B 0D ? ? ? ? 33 D2 E8 ? ? ? ? 84 C0 75 ? 48 8B 05 ? ? ? ? BA") }, [](Memory::Ptr ptr) {
			Vars::g_NetworkBaseConfig = ptr.FromInstruction().As<Rage::Network::NetworkBaseConfig*>();
		}, out);

		Batch.Add({ ("NBC2"), ("48 8B 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 84 C0 75 17 48 8B 0D ? ? ? ? 48 8B D7") }, [](Memory::Ptr ptr) {
			Vars::g_Network = ptr.FromInstruction().As<Network**>();
		}, out);

		Batch.Add({ ("JSBI"), ("E8 ? ? ? ? 0F B6 CB 84 C0 41 0F 44 CD") }, [](Memory::Ptr ptr) {
			Vars::g_JoinSessionByInfo = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("CJM"), ("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 49 8D 8F ? ? ? ?") }, [](Memory::Ptr ptr) {
			Vars::g_ConstructJoinMessage = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("GFM"), ("75 1C E8 ? ? ? ? 48 85 C0") }, [](Memory::Ptr ptr) {
			Vars::g_GetFriendsMenu = ptr.Sub(0xB).As<u64>();
		}, out);

		Batch.Add({ ("CPM"), ("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B D9") }, [](Memory::Ptr ptr) {
			Vars::g_ConstructPlayerMenu = ptr.As<u64>();
		}, out);

		Batch.Add({ ("HNM"), ("48 83 EC 20 4C 8B 71 50 33 ED") }, [](Memory::Ptr ptr) {
			Vars::g_HandleNetMessage = ptr.Sub(0x19).As<u64>();
		}, out);

		Batch.Add({ ("SSBG"), ("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8B 05 ? ? ? ? 48 8D 4C 24") }, [](Memory::Ptr ptr) {
			Vars::g_StartSessionByGamer = ptr.Call().As<u64>();
			}, out);


		Batch.Add({ ("GCP"), ("48 89 5C 24 08 48 89 74 24 18 89 54 24 10 57 48 83 EC 40 48") }, [](Memory::Ptr ptr) {
			Vars::g_GetConnectionPeer = ptr.As<u64>();
		}, out);

		Batch.Add({ ("QP"), ("E8 ? ? ? ? 84 C0 74 4D B3 01") }, [](Memory::Ptr ptr) {
			Vars::g_QueuePacket = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("SP"), ("48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 4C 89 48 20 55 41 54 41 55 41 56 41 57 48 8D A8 98") }, [](Memory::Ptr ptr) {
			Vars::g_SendPacket = ptr.As<u64>();
		}, out);

		Batch.Add({ ("QPWP"), ("48 83 EC 48 48 8B 44 24 78 48 89 44 24 30") }, [](Memory::Ptr ptr) {
			Vars::g_QueuePacketWithPlayer = ptr.As<u64>();
		}, out);

		Batch.Add({ ("PDN1"), ("E8 ? ? ? ? 84 C0 0F 95 C0 48 83 C4 58 C3 4C 8B DC") }, [](Memory::Ptr ptr) {
			Vars::g_CompressDataNode = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("PDN2"), ("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC 30 48 8B 01 4C 8B F2 33 D2 48 8B F1 FF 90 ? ? ? ? 84 C0 0F 84") }, [](Memory::Ptr ptr) {
			Vars::g_CompressDataNode2 = ptr.As<u64>();
		}, out);

		Batch.Add({ ("HROFV"), ("45 33 C9 45 85 C0 74 56 41 FF C8 74 4C 41 FF C8 74 42 41 FF C8 74 38 41 FF C8 74 2E 41 FF") }, [](Memory::Ptr ptr) {
			Vars::g_HandleRotationValuesFromOrder = ptr.As<u64>();
			}, out);

		Batch.Add({ ("SPO"), ("4C 8B 81 ? ? ? ? 4D 85 C0 0F 84 ? ? ? ? 41 8B 80 ? ? ? ? F3") }, [](Memory::Ptr ptr) {
			Vars::g_SyncPedOrientation = ptr.As<u64>();
		}, out);

		Batch.Add({ ("SEM"), ("40 53 48 83 EC 20 48 81 C1 ? ? ? ? 48 8B DA 48 8B 01 FF 90 ? ? ? ? 33 C9 48 85 C0 0F 84 ? ? ? ? 0F 28 48 60 0F") }, [](Memory::Ptr ptr) {
			Vars::g_SyncEntityMatrix = ptr.As<u64>();
		}, out);

		Batch.Add({ ("PMDN"), ("40 53 48 83 EC 20 48 8B 02 4C 8B C2 48 8B D9 48 8B D1 49 8B C8 FF 90 88 00 00 00 B8") }, [](Memory::Ptr ptr) {
			Vars::g_PedMovement = ptr.As<u64>();
		}, out);

		Batch.Add({ ("SCMP"), ("83 7E 1C 01 48 8B 3D") }, [](Memory::Ptr ptr) {
			Vars::g_SendChatMessagePtr = ptr.Add(7).Rip().As<int64_t**>();
		}, out);

		Batch.Add({ ("SCM"), ("48 83 EC 20 48 8B F1 48 8B CA 41 8A E9") }, [](Memory::Ptr ptr) {
			Vars::g_SendChatMessage = ptr.Sub(21).As<u64>();
		}, out);

		Batch.Add({ ("CD"), ("48 8B 05 ? ? ? ? 0F 45 DF") }, [](Memory::Ptr ptr) {
			Vars::g_ChatData = ptr.FromInstruction().As<ChatData**>();
		}, out);

		Batch.Add({ ("HPUP"), ("49 8B 80 ? ? ? ? 48 85 C0 74 0C F6 80 ? ? ? ? ? 75 03 32 C0 C3") }, [](Memory::Ptr ptr) {
			Vars::g_HandlePickupProcessing = ptr.As<u64>();
		}, out);

		Batch.Add({ ("3DPED"), ("4C 8B 15 ? ? ? ? F3 0F 11 45 ? F3 0F 10 05 ? ? ? ? BF ? ? ? ? BB ? ? ? ? 41 BE ? ? ? ? F3 0F 11") }, [](Memory::Ptr ptr) {
			Vars::g_UiDrawManager = ptr.FromInstruction().As<u64>();
		}, out);

		Batch.Add({ ("PSPTM"), ("40 53 48 83 EC 20 8B 02 48 8D 54 24 38 48 8B D9 89 44 24 38 E8 ? ? ? ? 48 8B C8") }, [](Memory::Ptr ptr) {
			Vars::g_PushScenePresetManager = ptr.As<u64>();
		}, out);

		Batch.Add({ ("AETS"), ("4C 8B DC 48 83 EC 58 8B") }, [](Memory::Ptr ptr) {
			Vars::g_AddElementToScene = ptr.As<u64>();
		}, out);

		Batch.Add({ ("SCEL"), ("48 8B C4 48 89 58 10 48 89 70 18 57 48 83 EC 30 48 83 B9") }, [](Memory::Ptr ptr) {
			Vars::g_SetSceneElementLighting = ptr.As<u64>();
		}, out);

		Batch.Add({ ("GCP"), ("0F B7 81 10 08 00 00") }, [](Memory::Ptr ptr) {
			Vars::g_GetScenePreset = ptr.As<u64>();
		}, out);

		Batch.Add({ ("WPGSN"), ("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC 30 0F B7 81") }, [](Memory::Ptr ptr) {
			Vars::g_WritePlayerGameStateDataNode = ptr.As<u64>();
		}, out);

		Batch.Add({ ("HMNSH"), ("E8 ? ? ? ? 84 C0 74 60 4C 39 77 40 74 29 48 8B 4C 24 ? 48 8B 01 FF 50 28 48 8B 4F 40 44 0F B7") }, [](Memory::Ptr ptr) {
			Vars::g_ReadNewScriptHostMessage = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("GHAH"), ("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 8A 81 8F") }, [](Memory::Ptr ptr) {
			Vars::g_GetHostArrayHandler = ptr.As<u64>();
		}, out);

		Batch.Add({ ("WQ"), ("74 41 4C 8B 05 ? ? ?") }, [](Memory::Ptr ptr) {
			Vars::g_WaterQuads.m_oceanQuads = (QuadInfo*)Memory::GetAddressFromInstruction(ptr.As<uint64_t>(), 2);
		}, out);

		Batch.Add({ ("NRQ"), ("E8 ? ? ? ? EB 3E 48 8B D3") }, [](Memory::Ptr ptr) {
			Vars::g_RequestControl = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("HTPR"), ("83 F9 FF 74 31 4C 8B 0D") }, [](Memory::Ptr ptr) {
			Vars::g_HandleToPtr = ptr.As<u64>();
		}, out);

		Batch.Add({ ("SRE"), ("40 53 48 83 EC 20 0F B7 D9 48 8B 0D ? ? ? ? B2 01 E8 ? ? ? ? 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B C8 33 C0 48 85 C9 74 08 0F B7 D3 E8 ? ? ? ? 48 8B 0D") }, [](Memory::Ptr ptr) {
			Vars::g_SendRagdollEvent = ptr.As<u64>();
		}, out);

		Batch.Add({ ("WIM"), ("0F B7 15 ? ? ? ? ? 33 D2 2B D3 78 ? ? 8B 1D") }, [](Memory::Ptr ptr) {
			Vars::g_WeaponInfoManager = ptr.FromInstruction().Sub(72).As<CWeaponInfoManager*>();
		}, out);

		Batch.Add({ ("FCP"), ("E8 ? ? ? ? 84 C0 75 0B 41 FF CF") }, [](Memory::Ptr ptr) {
			Vars::g_FragmentPatch = ptr.Call().As<decltype(Vars::g_FragmentPatch)>();
		}, out);

		Batch.Add({ ("IDCP"), ("E8 ? ? ? ? 8B 9C 24 B8 00 00 00 4C 8B AC 24 A8 00 00 00") }, [](Memory::Ptr ptr) {
			Vars::g_InvalidDecalPatch = ptr.Call().As<decltype(Vars::g_InvalidDecalPatch)>();
		}, out);

		Batch.Add({ ("IMCP"), ("E8 ? ? ? ? 40 88 7C 24 ? 49 89 9C 24") }, [](Memory::Ptr ptr) {
			Vars::g_InvalidModsPatch = ptr.Call().As<decltype(Vars::g_InvalidModsPatch)>();
		}, out);

		Batch.Add({ ("RBP"), ("48 89 5C 24 08 4C 89 44 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 81 EC 80 00 00 00 48") }, [](Memory::Ptr ptr) {
			Vars::g_RenderBigPed = ptr.As<u64>();
		}, out);

		Batch.Add({ ("RP"), ("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 81 EC 80 00 00 00 48 8B FA") }, [](Memory::Ptr ptr) {
			Vars::g_RenderPed = ptr.As<u64>();
		}, out);

		Batch.Add({ ("RE"), ("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 41 54 41 55 41 56 41 57 48 83 EC 70 0F BA") }, [](Memory::Ptr ptr) {
			Vars::g_RenderEntity = ptr.As<u64>();
		}, out);

		Batch.Add({ ("SPPT"), ("40 55 53 56 57 41 54 48 8B") }, [](Memory::Ptr ptr) {
			Vars::g_SeralizeParachuteTask = ptr.As<u64>();
		}, out);

		Batch.Add({ ("SPVT"), ("40 55 53 57 41 56 48 8B EC 48 83 EC 68") }, [](Memory::Ptr ptr) {
			Vars::g_SeralizePedVariation = ptr.As<u64>();
		}, out);

		Batch.Add({ ("ABTP"), ("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 80 3D ? ? ? ? ? 41 8B D8 8B F2") }, [](Memory::Ptr ptr) {
			Vars::g_AmbientTaskPatch = ptr.As<u64>();
		}, out);

		Batch.Add({ ("PTP"), ("0F 88 ? ? ? ? 75 34") }, [](Memory::Ptr ptr) {
			Vars::g_ParachuteTaskPatch = ptr.Sub(6).As<u64>();
		}, out);

		Batch.Add({ ("ATP"), ("E8 ? ? ? ? 84 C0 74 8D") }, [](Memory::Ptr ptr) {
			Vars::g_AnimTaskPatch = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("SCP"), ("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 48 8D 05") }, [](Memory::Ptr ptr) {
			Vars::g_StandPatch = ptr.As<u64>();
		}, out);

		Batch.Add({ ("KCP"), ("E8 ? ? ? ? FE 8F 3B 02 00 00") }, [](Memory::Ptr ptr) {
			Vars::g_KanyePatch = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("CVOFB"), ("48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 4C 89 60 20 55 41 56 41 57 48 8B EC 48 81 EC 80 00 00 00 4C 8B 75 48") }, [](Memory::Ptr ptr) {
			Vars::g_VehicleControlOutOfBoundsPatch = ptr.As<u64>();
		}, out);

		Batch.Add({ ("HGBA"), ("40 53 48 83 EC 30 48 8B 1D ? ? ? ? BA") }, [](Memory::Ptr ptr) {
			Vars::g_HasGameBeenAltered = ptr.As<u64>();
		}, out);

		Batch.Add({ ("HRP"), ("E8 ? ? ? ? EB 0B 8B CB") }, [](Memory::Ptr ptr) {
			Vars::g_HasRosPrivilege = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("PRM"), ("E8 ? ? ? ? 84 C0 75 02 33 FF 4C 8D") }, [](Memory::Ptr ptr) {
			Vars::g_PostRawMessage = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("MDP"), ("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 80 3D ? ? ? ? ? 41 8B D8 8B F2 48 8B F9 74 2F E8") }, [](Memory::Ptr ptr) {
			Vars::g_MeltdownPatch = ptr.As<u64>();
		}, out);

		Batch.Add({ ("SCPCSC"), ("48 89 5C 24 ? 48 89 4C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 83 EC 30 4C 8B FA 4D 8B E8 48 8B D1 4D 85 FF 74 10") }, [](Memory::Ptr ptr) {
			Vars::g_CheckStringStatus = ptr.As<u64>();
		}, out);

		Batch.Add({ ("APM"), ("40 53 48 83 EC 30 0F 29 74 24 ? 48 8B D9 E8 ? ? ? ? F3 0F 10 83 ? ? ? ? F3 0F 10 35") }, [](Memory::Ptr ptr) {
			Vars::g_ApplyWeatherMeta = ptr.As<u64>();
		}, out);

		Batch.Add({ ("AMM"), ("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 20 48 8B D9 48 8B 49 18") }, [](Memory::Ptr ptr) {
			Vars::g_AllocateMemoryMessage = ptr.As<u64>();
			Vars::g_ConnMgrFreeMemory = ptr.Add(0x52).Rip().As<u64>();
		}, out);

		Batch.Add({ ("RMFQ"), ("E8 ? ? ? ? 0F B7 43 4C 48 8D 55 20") }, [](Memory::Ptr ptr) {
			Vars::g_RemoveMessageFromQueue = ptr.Call().As<u64>();
			Vars::g_RemoveUnackedMessage = ptr.Add(0x19).Rip().As<u64>();
		}, out);

		Batch.Add({ ("MBT"), ("E8 ? ? ? ? CC FF 15") }, [](Memory::Ptr ptr) {
			Vars::g_MessageBoxTermination = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("RBA2"), ("48 89 5C 24 10 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 30 48 8B 05") }, [](Memory::Ptr ptr) {
			Vars::g_ReceiveBroadcastArray = ptr.As<u64>();
		}, out);

		Batch.Add({ ("SCI"), ("48 8B D3 48 8D 4C 24 ? 48 69 D2") }, [](Memory::Ptr ptr) {
			Vars::g_ScInfo = ptr.Sub(7).FromInstruction().As<ScInfo*>();
		}, out);

		Batch.Add({ ("PMR"), ("48 89 5C 24 08 48 89 74 24 10 57 48 81 EC F0 00 00 00 41 83") }, [](Memory::Ptr ptr) {
			Vars::g_ProcessMatchmakingResponse = ptr.As<u64>();
		}, out);

		Batch.Add({ ("SMF"), ("E8 ? ? ? ? 84 C0 0F 84 F6 FE FF FF") }, [](Memory::Ptr ptr) {
			Vars::g_StartMatchmakingFind = ptr.Call().As<u64>();
		}, out);

		Batch.Add({ ("FL"), ("48 03 0D ? ? ? ? E8 ? ? ? ? 48 8D 4C 24 ? E8 ? ? ? ? 84 C0 74 10 48 8D 4C 24 ? 44 8B") }, [](Memory::Ptr ptr) {
			Vars::g_Friends = ptr.FromInstruction().As<Friends*>();
		}, out);

		Batch.Add({ ("GEH"), ("48 F7 F9 49 8B 48 08 48 63 D0 C1 E0 08 0F B6 1C 11 03 D8") }, [](Memory::Ptr ptr) {
			Vars::g_GetEntityHandleFromAddress = ptr.Sub(104).As<u64>();
		}, out);

		//patches :DDDDDDD

		//should be patched by the game already
		Batch.Add({ ("NARRC"), ("48 8B 0D ? ? ? ? 48 85 C9 74 08 48 8B 01 B2 01 FF 50 18 0F B7 56 0A 41 B0 01 48 8B CD E8 ? ? ? ? 33 C9") }, [](Memory::Ptr ptr) {
			Memory::WriteVector(ptr.As<u64>(), { 0xE9, 0x7C, 0x01, 0x00, 0x00 });
		}, out);

		Batch.Add({ ("CKL"), ("E8 ? ? ? ? 48 8B 56 08 84 C0 74 59 48 FF C3 44 38 3C 1A 75 F7 48 8D 4D 00 4C 8B C3 E8 ? ? ? ? 48 8D 4D 48 E8") }, [](Memory::Ptr ptr) {
			Memory::WriteVector(ptr.Call().As<u64>(), { 0xB0, 0x00, 0xC3 });
			}, out);

		Batch.Add({ ("CKK"), ("E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 48 8B 56 08 4C 8B C3 49 FF C0 46 38 3C 02 75 F7 48 8D 4D 00 E8 ? ? ? ? 48 8D 4D 00 E8 ? ? ? ? 48 8B 56 08 84 C0 0F 84") }, [](Memory::Ptr ptr) {
			Memory::WriteVector(ptr.Call().As<u64>(), { 0xB0, 0x00, 0xC3 });
		}, out);

		Batch.Add({ ("FMEE"), ("48 8B 5C 24 40 48 8B 6C 24 48 48 8B 74 24 50 48 8B 7C 24 58 48 83 C4 30 41 5E C3 48 8B 0D") }, [](Memory::Ptr ptr) {
			Memory::Nop(ptr.Add(0x31).As<u64>(), 5);
		}, out);

		Batch.Add({ ("TCRASH"), ("44 38 3D ? ? ? ? 74 0E B1 01 E8 ? ? ? ? 33 C9 E8") }, [](Memory::Ptr ptr) {
			Memory::Nop(ptr.Add(0x12).As<u64>(), 5);
		}, out);

		Batch.Add({ ("NAARC2"), ("E8 ? ? ? ? EB 61 E8 ? ? ? ? 48 8B 0D ? ? ? ? 8B 51 20 8B 41 10 C1 E2 02 C1 FA 02 2B C2 85 C0 7E 30 E8") }, [](Memory::Ptr ptr) {
			Memory::Nop(ptr.As<u64>(), 5);
		}, out);

		Batch.Add({ ("CT"), ("48 3B F8 74 ? 8B 1D") }, [](Memory::Ptr ptr) {
			Memory::WriteVector(ptr.Add(4).As<u64>(), { 0x00 });
		}, out);

		//should be done from the script vm but cant get the reimplementaion working :/
		/*Batch.Add({ ("SVMP1"), ("3b 0a 0f 83 ? ? ? ? 48 ff c7") }, [](Memory::Ptr ptr) {
			Memory::WriteVector(ptr.Add(2).As<u64>(), { 0xc9310272 });
			Memory::WriteVector(ptr.Add(6).As<u64>(), { 0x9090 });
		}, out);

		Batch.Add({ ("SVMP2"), ("3b 0a 0f 83 ? ? ? ? 49 03 fa") }, [](Memory::Ptr ptr) {
			Memory::WriteVector(ptr.Add(2).As<u64>(), { 0xc9310272 });
			Memory::WriteVector(ptr.Add(6).As<u64>(), { 0x9090 });
		}, out);

		Batch.Add({ ("SVMP3"), ("3b 11 0f 83 ? ? ? ? 48 ff c7") }, [](Memory::Ptr ptr) {
			Memory::WriteVector(ptr.Add(2).As<u64>(), { 0xd2310272 });
			Memory::WriteVector(ptr.Add(6).As<u64>(), { 0x9090 });

			Memory::WriteVector(ptr.Add(0x1C).Add(2).As<u64>(), { 0xd2310272 });
			Memory::WriteVector(ptr.Add(0x1C).Add(6).As<u64>(), { 0x9090 });
		}, out);

		Batch.Add({ ("SVMP4"), ("3b 11 0f 83 ? ? ? ? 49 03 fa") }, [](Memory::Ptr ptr) {
			Memory::WriteVector(ptr.Add(2).As<u64>(), { 0xd2310272 });
			Memory::WriteVector(ptr.Add(6).As<u64>(), { 0x9090 });

			Memory::WriteVector(ptr.Add(0x26).Add(2).As<u64>(), { 0xd2310272 });
			Memory::WriteVector(ptr.Add(0x26).Add(6).As<u64>(), { 0x9090 });
		}, out);*/

		auto Region = Memory::Module("GTA5.exe");
		Batch.Run(Region);

		auto patches = Region.ScanAll("48 8D 45 ? 48 89 45 ? 48 8D 05 ? ? ? ? 48 89 45");
		for (auto patch : patches) {
			memset(patch.Add(8).As<PVOID>(), 0x90, 7);
		}


		//bandaid for now
		Vars::g_TimeCycleMgr = (TimeCycle*)Memory::GetAddressFromInstruction(GetAddressInMemory("f3 0f 10 72 78 ? 8d 0d ? ? ? ? 33 d2 e8"), 8, 12);
		//LOG_CUSTOM_SUCCESS("TC", "Init");
		//LOG_CUSTOM_WARN("Bypass", "Patched All Integrity Handlers");

		Memory::Batch ScBatch;


		ScBatch.Add({ ("PD"), ("48 8D 05 ? ? ? ? 48 8B F9 48 89 01 48 83 C1 08 E8 ? ? ? ? 33 C0") }, [](Memory::Ptr ptr) {
			auto Tmp = ptr.FromInstruction().As<u64*>();
			Vars::g_UpdatePresenceAttributeInt = Tmp[1];
			Vars::g_UpdatePresenceAttributeString = Tmp[3];
		}, out);

		ScBatch.Add({ ("SCGI"), ("48 8D 05 ? ? ? ? 48 03 F8 44 8B 47 14 48 8D 57 20 E8 ? ? ? ? 85") }, [](Memory::Ptr ptr) {
			Vars::g_ScGameInfo = ptr.FromInstruction().As<ScGameInfo*>();
		}, out);

		ScBatch.Add({ ("RAP"), ("75 70 EB 23") }, [](Memory::Ptr ptr) {
			Memory::Nop(ptr.As<u64>(), 2);
			Memory::WriteVector(ptr.Add(0x72).As<u64>(), { 0xB0, 0x01 });
		}, out);

		auto ScRegion = Memory::Module("socialclub.dll");
		ScBatch.Run(ScRegion);


		Menu::Pools::GetPoolManager()->Initialize();
	}

	void Patterns::AntiCheat() {
		Memory::Batch Batch;
		bool out = true;

		Batch.Add({ "GSU", "40 53 48 83 EC 20 48 8B 81 40 01" }, [](Memory::Ptr ptr) {
			Vars::g_GameSkeleton = ptr.As<u64>();
			return IsValidPtr(Vars::g_GameSkeleton);
		}, out);

		Batch.Add({ "QD", "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 8B F2 49 8B F8" }, [](Memory::Ptr ptr) {
			Vars::g_QueueDependency = ptr.As<u64>();
			return IsValidPtr(Vars::g_QueueDependency);
		}, out);

		Batch.Add({ "SM", "E8 ? ? ? ? EB 69 41 8B D6" }, [](Memory::Ptr ptr) {
			Vars::g_SendMetric = ptr.Call().As<u64>();
			return IsValidPtr(Vars::g_SendMetric);
		}, out);

		Batch.Add({ "SNE", "48 8B 5F 08 48 8B 7F 10 49 8B D6 48 8B 03 48 8B CB FF 90 ? ? ? ? 84 C0 0F 85" }, [](Memory::Ptr ptr) {
			Vars::g_SendNetworkEvent = ptr.Sub(0x3A).As<u64>();
			return IsValidPtr(Vars::g_SendNetworkEvent);
		}, out);

		Batch.Add({ "HSR", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B D9 48 81 C1 ? ? ? ? 48 8B F2 33 FF E8" }, [](Memory::Ptr ptr) {
			Vars::g_SendHttpRequest = ptr.As<u64>();
			return IsValidPtr(Vars::g_SendHttpRequest);
		}, out);

		auto Region = Memory::Module("GTA5.exe");
		Batch.Run(Region);
	}

	void Patterns::InGame() {
		Memory::Batch Batch;
		bool out = true;


		Batch.Add({ ("INVPLY"), ("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 30 49 8B D9 41 8B F8 48 8B F2 48 8B E9 4D 85 C9 75 16 48 8D 15 ? ? ? ? 48") }, [](Memory::Ptr ptr) {
			Vars::g_InvitePlayer = ptr.As<u64>();
		}, out);

		auto Region = Memory::Module("GTA5.exe");
		Batch.Run(Region);
	}

	void Patterns::Init() {
		PreGame();
		AntiCheat();

		//wait for the game to load
		while (*Vars::g_GameState != 0) {
			std::this_thread::sleep_for(3s);
			std::this_thread::yield();
		}
		
		//when game has finished loading scan for in game patterns
		InGame();

		//LOG_SUCCESS("patterns init");
	}

}