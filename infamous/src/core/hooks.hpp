#pragma once
#include "stdafx.hpp"
#include "rage/classes/script/scrProgram.hpp"
#include "rage/classes/script/scrThreadContext.hpp"
#include "rage/classes/rage/rlMetric.hpp"
#include "rage/classes/network/CNetGamePlayer.hpp"
#include "rage/classes/network/netEvent.hpp"
#include "rage/classes/network/RemoteGamerInfoMsg.hpp"
#include "rage/classes/netsync/netSyncDataNode.hpp"
#include "rage/classes/netsync/data_nodes.hpp"
//:DDD

class CHttpRequest;
class ClonedTakeOffPedVariationInfo;
namespace Hooks {
	class Hooks {
	public:
		void Init();
	private:
		
	};

	inline Hooks* GetHooks() {
		static Hooks instance;
		return &instance;
	}

	void SetWarningMessageWithHeader(Rage::Invoker::NativeContext* src);
	void StatGetIntHook(Rage::Invoker::NativeContext* Context);
	void NetworkSessionHost(Rage::Invoker::NativeContext* Context);

	void _0x6BFB12CE158E3DD4(Rage::Invoker::NativeContext* Context);
	void _0xFE4C1D0D3B9CC17E(Rage::Invoker::NativeContext* Context);
	void IsSwitchToMultiFirstPartFinished(Rage::Invoker::NativeContext* Context);
	void SetFocusEntity(Rage::Invoker::NativeContext* Context);
	void HideHudAndRadar(Rage::Invoker::NativeContext* Context);
	void ActivateFrontendMenu(Rage::Invoker::NativeContext* Context);
	void RestartFrontendMenu(Rage::Invoker::NativeContext* Context);
	void TogglePauseRenderPhase(Rage::Invoker::NativeContext* Context);
	void SetEntityVisible(Rage::Invoker::NativeContext* Context);
	void SetEntityCoords(Rage::Invoker::NativeContext* Context);
	void SetEntityCollision(Rage::Invoker::NativeContext* Context);
	void SetPlayerControl(Rage::Invoker::NativeContext* Context);
	void FreezeEntityPosition(Rage::Invoker::NativeContext* Context);
	void ReviveLocalPlayer(Rage::Invoker::NativeContext* Context);
	//hooks
	inline uint64_t(*OgScriptVmHook)(Rage::scrValue* Stack, uint64_t** Globals, Rage::scrProgram* Program, Rage::scrThreadContext* Context);
	uint64_t ScriptVmHook(Rage::scrValue* Stack, uint64_t** Globals, Rage::scrProgram* Program, Rage::scrThreadContext* Context);

	inline bool(*OgSendMetricHook)(Rage::rlMetric* metric, bool unk);
	bool SendMetricHook(Rage::rlMetric* metric, bool unk);

	inline void(*OgCreateNeverEventHook)(uint64_t net_table, uint64_t event);
	void CreateNetworkEventHook(uint64_t net_table, uint64_t event);

	inline bool(*OgSendHTTPRequestHook)(void* request, const char* uri);
	bool SendHTTPRequestHook(void* request, const char* uri);

	inline void(*OgUpdateGameSkeletonHook)(__int64 skeleton, int type);
	void UpdateGameSkeletonHook(__int64 skeleton, int type);

	inline int(*OgQueueDependencyHook)(void* a1, int a2, int64_t dependency);
	int QueueDependencyHook(void* a1, int a2, int64_t dependency);

	inline u64* (*OgFallTaskCreateHook)(u64* _this, u32 flags);
	u64* FallTaskCreateHook(u64* _this, u32 flags);

	inline u64 (*OgJumpTaskCreateHook)(u64 _this, u32 flags);
	u64 JumpTaskCreateHook(u64 _this, u32 flags);

	inline void (*OgHandleNetworkEvent)(void* _this, CNetGamePlayer* fromPlayer, CNetGamePlayer* toPlayer, u16 eventID, int eventIndex, int eventBitset, int bufferSize, Rage::datBitBuffer* buffer);
	void HandleNetworkEvent(void* _this, CNetGamePlayer* fromPlayer, CNetGamePlayer* toPlayer, u16 eventID, int eventIndex, int eventBitset, int bufferSize, Rage::datBitBuffer* buffer);

	inline bool(*OgInitNativeTablesHook)(Rage::scrProgram* _this);
	bool InitNativeTablesHook(Rage::scrProgram* _this);

	inline eAckCode(*OgProcessCloneSyncHook)(CNetworkObjectMgr* _this, CNetGamePlayer* fromPlayer, CNetGamePlayer* toPlayer, eNetObjType objectType, u16 objectID, Rage::datBitBuffer* msgBuffer, u16 seqNum, u32 timeStamp);
	eAckCode ProcessCloneSyncHook(CNetworkObjectMgr* _this, CNetGamePlayer* fromPlayer, CNetGamePlayer* toPlayer, eNetObjType objectType, u16 objectID, Rage::datBitBuffer* msgBuffer, u16 seqNum, u32 timeStamp);

	inline void(*OgProcessCloneCreateHook)(CNetworkObjectMgr* _this, CNetGamePlayer* fromPlayer, CNetGamePlayer* toPlayer, eNetObjType objectType, int32_t objectID, int32_t objetFlag, Rage::datBitBuffer* msgBuffer, int32_t timeStamp);
	void ProcessCloneCreateHook(CNetworkObjectMgr* _this, CNetGamePlayer* fromPlayer, CNetGamePlayer* toPlayer, eNetObjType objectType, int32_t objectID, int32_t objetFlag, Rage::datBitBuffer* msgBuffer, int32_t timeStamp);

	inline bool(*OgCanApplyDataHook)(Rage::netSyncTree* tree, Rage::netObject* object);
	bool CanApplyDataHook(Rage::netSyncTree* tree, Rage::netObject* object);
	
	inline int(*OgFormatEngineStringHook)(uint64_t rcx, uint64_t rdx, uint32_t r8d, const char* r9, uint32_t stack);
	uint64_t FormatEngineStringHook(uint64_t rcx, uint64_t rdx, uint32_t r8d, const char* r9, uint32_t stack);

	inline int(*OgGetEngineStringLineCountHook)(uint64_t rcx, const char* rdx, float xmm2, uint64_t r9, bool stack);
	int GetEngineStringLineCountHook(uint64_t rcx, const char* rdx, float xmm2, uint64_t r9, bool stack);

	inline void(*OgApplyPlayerPhysicalIndexHook)(uint64_t, CNetGamePlayer*, uint8_t);
	void ApplyPlayerPhysicalIndexHook(uint64_t Rcx, CNetGamePlayer* NetGamePlayer, uint8_t R8b);	

	//idk
	inline bool(*OgWriteBitBufferDwordHook)(__int64 bit_buffer, int value, int bits);
	bool WriteBitBufferDwordHook(__int64 bit_buffer, int value, int bits);

	inline bool(*OgReadBitBufferDwordHook)(__int64 bit_buffer, int value, int bits);
	bool ReadBitBufferDwordHook(__int64 bit_buffer, int value, int bits);
	

	//crashes
	inline bool(*OgFragmentCrashPatchHook)(float* a1, float* a2);
	bool FragmentCrashPatchHook(float* a1, float* a2);

	inline void(*OgInvalidDecalPatchHook)(uintptr_t a1, int a2);
	void InvalidDecalPatchHook(uintptr_t a1, int a2);

	inline void(*OgInvalidModsPatchHook)(int64_t a1, int64_t a2, int a3, char a4);
	void InvalidModsPatchHook(int64_t a1, int64_t a2, int a3, char a4);

	inline __int64(*OgRenderBigPedHook)(__int64 renderer, CPed* ped, __int64 a3, __int64 a4);
	__int64 RenderBigPedHook(__int64 renderer, CPed* ped, __int64 a3, __int64 a4);

	inline void(*OgRenderEntityHook)(__int64 renderer, Rage::fwEntity* entity, int unk, bool a4);
	void RenderEntityHook(__int64 renderer, Rage::fwEntity* entity, int unk, bool a4);

	inline void*(*OgRenderPedHook)(__int64 renderer, CPed* ped, __int64 a3, __int64 a4);
	void* RenderPedHook(__int64 renderer, CPed* ped, __int64 a3, __int64 a4);

	inline void(*OgSerializeParachuteTaskHook)(__int64 info, Rage::CSyncDataBase* serializer);
	void SerializeParachuteTaskHook(__int64 info, Rage::CSyncDataBase* serializer);

	inline void(*OgSerializePedVariationHook)(ClonedTakeOffPedVariationInfo* info, Rage::CSyncDataBase* serializer);
	void SerializePedVariationHook(ClonedTakeOffPedVariationInfo* info, Rage::CSyncDataBase* serializer);

	inline int(*OgAmbientTaskPatchHook)(uint64_t _this, int a2, int a3);
	int AmbientTaskPatchHook(uint64_t _this, int a2, int a3);

	inline int(*OgParachuteTaskPatchHook)(uint64_t _this, int a2, int a3);
	int ParachuteTaskPatchHook(uint64_t _this, int a2, int a3);

	inline bool(*OgAnimTaskPatchHook)(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5);
	bool AnimTaskPatchHook(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5);

	inline void(*OgStandPatchHook)(__int64 a1, __int64 a2, unsigned int a3, int a4, __int64 a5);
	void StandPatchHook(__int64 a1, __int64 a2, unsigned int a3, int a4, __int64 a5);

	inline uint64_t(*OgKanyePatchHook)(uintptr_t a1, bool a2);
	uint64_t KanyePatchHook(uintptr_t a1, bool a2);


	//48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 4C 89 60 20 55 41 56 41 57 48 8B EC 48 81 EC 80 00 00 00 4C 8B 75 48

	inline bool(*OgVehicleControlOutOfBoundsHook)(__int64 a1, __m128* a2, __int64 a3, int a4, __int64 a5, __int64 a6, char a7);
	bool VehicleControlOutOfBoundsHook(__int64 a1, __m128* a2, __int64 a3, int a4, __int64 a5, __int64 a6, char a7);

	inline bool(*OgConstructJoinMessageHook)(RemoteGamerInfoMsg* info, void* data, int size, int* bits);
	bool ConstructJoinMessageHook(RemoteGamerInfoMsg* info, void* data, int size, int* bits);

	inline bool(*OgHandleNetMessageHook)(void* _This, void* Rcx, Rage::netConnection::InFrame* Frame);
	bool HandleNetMessageHook(void* _This, void* Rcx, Rage::netConnection::InFrame* Frame);

	inline bool(*OgCompressDataNodeHook)(DataNode* node, uint32_t size, CNetGamePlayer* sender, CNetGamePlayer* receiver, Rage::datBitBuffer* buffer, int bufferSize, uint64_t a7, bool a8, uint32_t* a9, uint32_t* a10);
	bool CompressDataNodeHook(DataNode* node, uint32_t size, CNetGamePlayer* sender, CNetGamePlayer* receiver, Rage::datBitBuffer* buffer, int bufferSize, uint64_t a7, bool a8, uint32_t* a9, uint32_t* a10);

	inline bool(*OgCompressDataNodeHook2)(DataNode* Node, NetObjectBase* Object);
	bool CompressDataNodeHook2(DataNode* Node, NetObjectBase* Object);

	inline void(*OgSyncPedOrientation)(uint64_t, uint64_t);
	void SyncPedOrientationHook(uint64_t Rcx, uint64_t Rdx);

	inline void(*OgSyncEntityMatrix)(uint64_t, uint64_t);
	void SyncEntityMatrixHook(uint64_t Rcx, uint64_t Rdx);

	inline void(*OgPedMovementDataNode)(uint64_t, uint64_t);
	void PedMovementDataNodeHook(uint64_t Rcx, uint64_t Rdx);

	inline bool(*OgSendChatMessage)(void* _This, Rage::rlGamerInfo* GamerInfo, char* Message, bool isTeam);
	bool SendChatMessageHook(void* _This, Rage::rlGamerInfo* GamerInfo, char* Message, bool isTeam);

	inline bool(*OgHandlePickupProcessingHook)(Rage::netObject* object, void* unk, CPed* ped);
	bool HandlePickupProcessingHook(Rage::netObject* object, void* unk, CPed* ped);

	inline bool(*OgProcessCloneRemove)(uint64_t, CNetGamePlayer*, uint64_t, uint16_t, uint64_t);
	bool ProcessCloneRemoveHook(uint64_t This, CNetGamePlayer* NetGamePlayer, uint64_t R8, uint16_t NetworkID, uint64_t Stack);

	inline void(*OgPackCloneCreate)(uint64_t, Rage::netObject*, CNetGamePlayer*, uint64_t);
	void PackCloneCreateHook(uint64_t This, Rage::netObject* Object, CNetGamePlayer* Player, uint64_t SyncData);

	inline bool(*OgPackCloneSync)(uint64_t, Rage::netObject*, CNetGamePlayer*);
	bool PackCloneSyncHook(uint64_t This, Rage::netObject* NetObj, CNetGamePlayer* NetGamePlayer);

	inline bool(*OgSendCloneSync)(uint64_t, Rage::netObject*, CNetGamePlayer*);
	bool SendCloneSyncHook(uint64_t This, Rage::netObject* NetObj, CNetGamePlayer* NetGamePlayer);

	inline void(*OgWritePlayerGameStateDataNode)(Rage::netObject*, CPlayerGameStateDataNode*);
	void WritePlayerGameStateDataNodeHook(Rage::netObject* Player, CPlayerGameStateDataNode* Node);

	inline bool(*OgHandleMsgNewScriptHost)(uint64_t Rcx, uint64_t Rdx);
	bool HandleMsgNewScriptHostHook(uint64_t Rcx, uint64_t Rdx);

	//hooks variables
	namespace Vars {
	}
}