#include "stdafx.hpp"
#include "hooks.hpp"
#include "patterns.hpp"
#include "util/hooking/hooking.hpp"
#include "cheat/menu/main.hpp"
namespace Hooks {
	void Hooks::Init() {
		//script vm
		if (IsValidPtr(Patterns::Vars::g_ScriptVm))
			Hooking::GetHooking()->Create("SVM", Patterns::Vars::g_ScriptVm, ScriptVmHook, &OgScriptVmHook);	

		if (IsValidPtr(Patterns::Vars::g_GameSkeleton))
			Hooking::GetHooking()->Create("GSU", Patterns::Vars::g_GameSkeleton, UpdateGameSkeletonHook, &OgUpdateGameSkeletonHook);

		if (IsValidPtr(Patterns::Vars::g_SendMetric))
			Hooking::GetHooking()->Create("SM", Patterns::Vars::g_SendMetric, SendMetricHook, &OgSendMetricHook);

		if (IsValidPtr(Patterns::Vars::g_QueueDependency))
			Hooking::GetHooking()->Create("QD", Patterns::Vars::g_QueueDependency, QueueDependencyHook, &OgQueueDependencyHook);

		if (IsValidPtr(Patterns::Vars::g_SendNetworkEvent))
			Hooking::GetHooking()->Create("SNE", Patterns::Vars::g_SendNetworkEvent, CreateNetworkEventHook, &OgCreateNeverEventHook);

		if (IsValidPtr(Patterns::Vars::g_SendHttpRequest))
			Hooking::GetHooking()->Create("HSR", Patterns::Vars::g_SendHttpRequest, SendHTTPRequestHook, &OgSendHTTPRequestHook);

		if (IsValidPtr(Patterns::Vars::g_FallTaskCreate))
			Hooking::GetHooking()->Create("FTC", Patterns::Vars::g_FallTaskCreate, FallTaskCreateHook, &OgFallTaskCreateHook);

		if (IsValidPtr(Patterns::Vars::g_JumpTaskCreate))
			Hooking::GetHooking()->Create("JTC", Patterns::Vars::g_JumpTaskCreate, JumpTaskCreateHook, &OgJumpTaskCreateHook);

		if (IsValidPtr(Patterns::Vars::g_HandleNetworkEvent))
			Hooking::GetHooking()->Create("HNE", Patterns::Vars::g_HandleNetworkEvent, HandleNetworkEvent, &OgHandleNetworkEvent);

		if (IsValidPtr(Patterns::Vars::g_InitNativeTables))
			Hooking::GetHooking()->Create("INT", Patterns::Vars::g_InitNativeTables, InitNativeTablesHook, &OgInitNativeTablesHook);

		if (IsValidPtr(Patterns::Vars::g_ReceiveCloneRemove))
			Hooking::GetHooking()->Create("PCR", Patterns::Vars::g_ReceiveCloneRemove, ProcessCloneRemoveHook, &OgProcessCloneRemove);

		if (IsValidPtr(Patterns::Vars::g_ReceiveCloneSync))
			Hooking::GetHooking()->Create("PCS", Patterns::Vars::g_ReceiveCloneSync, ProcessCloneSyncHook, &OgProcessCloneSyncHook);

		if (IsValidPtr(Patterns::Vars::g_ReceiveCloneCreate))
			Hooking::GetHooking()->Create("PCC", Patterns::Vars::g_ReceiveCloneCreate, ProcessCloneCreateHook, &OgProcessCloneCreateHook);

		if (IsValidPtr(Patterns::Vars::g_CanApplyData))
			Hooking::GetHooking()->Create("CAD", Patterns::Vars::g_CanApplyData, CanApplyDataHook, &OgCanApplyDataHook);

		if (IsValidPtr(Patterns::Vars::g_GetGameStringLineCount))
			Hooking::GetHooking()->Create("GGSLC", Patterns::Vars::g_GetGameStringLineCount, GetEngineStringLineCountHook, &OgGetEngineStringLineCountHook);

		if (IsValidPtr(Patterns::Vars::g_FormatGameString))
			Hooking::GetHooking()->Create("FGC", Patterns::Vars::g_FormatGameString, FormatEngineStringHook, &OgFormatEngineStringHook);

		if (IsValidPtr(Patterns::Vars::g_ApplyPlayerPhysicalIndexHook))
			Hooking::GetHooking()->Create("APPI", Patterns::Vars::g_ApplyPlayerPhysicalIndexHook, ApplyPlayerPhysicalIndexHook, &OgApplyPlayerPhysicalIndexHook);

		if (IsValidPtr(Patterns::Vars::g_FragmentPatch))
			Hooking::GetHooking()->Create("FCP", Patterns::Vars::g_FragmentPatch, FragmentCrashPatchHook, &OgFragmentCrashPatchHook);

		if (IsValidPtr(Patterns::Vars::g_InvalidDecalPatch))
			Hooking::GetHooking()->Create("IDCP", Patterns::Vars::g_InvalidDecalPatch, InvalidDecalPatchHook, &OgInvalidDecalPatchHook);

		if (IsValidPtr(Patterns::Vars::g_InvalidModsPatch))
			Hooking::GetHooking()->Create("IMCP", Patterns::Vars::g_InvalidModsPatch, InvalidModsPatchHook, &OgInvalidModsPatchHook);

		if (IsValidPtr(Patterns::Vars::g_RenderBigPed))
			Hooking::GetHooking()->Create("RBPH", Patterns::Vars::g_RenderBigPed, RenderBigPedHook, &OgRenderBigPedHook);

		if (IsValidPtr(Patterns::Vars::g_RenderPed))
			Hooking::GetHooking()->Create("RPH", Patterns::Vars::g_RenderPed, RenderPedHook, &OgRenderPedHook);

		if (IsValidPtr(Patterns::Vars::g_RenderEntity))
			Hooking::GetHooking()->Create("REH", Patterns::Vars::g_RenderEntity, RenderEntityHook, &OgRenderEntityHook);

		if (IsValidPtr(Patterns::Vars::g_SeralizeParachuteTask))
			Hooking::GetHooking()->Create("SPPT", Patterns::Vars::g_SeralizeParachuteTask, SerializeParachuteTaskHook, &OgSerializeParachuteTaskHook);

		if (IsValidPtr(Patterns::Vars::g_SeralizePedVariation))
			Hooking::GetHooking()->Create("SPVT", Patterns::Vars::g_SeralizePedVariation, SerializePedVariationHook, &OgSerializePedVariationHook);

		if (IsValidPtr(Patterns::Vars::g_AmbientTaskPatch))
			Hooking::GetHooking()->Create("ABTP", Patterns::Vars::g_AmbientTaskPatch, AmbientTaskPatchHook, &OgAmbientTaskPatchHook);

		if (IsValidPtr(Patterns::Vars::g_ParachuteTaskPatch))
			Hooking::GetHooking()->Create("PTP", Patterns::Vars::g_ParachuteTaskPatch, ParachuteTaskPatchHook, &OgParachuteTaskPatchHook);

		if (IsValidPtr(Patterns::Vars::g_AnimTaskPatch))
			Hooking::GetHooking()->Create("ATP", Patterns::Vars::g_AnimTaskPatch, AnimTaskPatchHook, &OgAnimTaskPatchHook);

		if (IsValidPtr(Patterns::Vars::g_StandPatch))
			Hooking::GetHooking()->Create("SCP", Patterns::Vars::g_StandPatch, StandPatchHook, &OgStandPatchHook);

		if (IsValidPtr(Patterns::Vars::g_KanyePatch))
			Hooking::GetHooking()->Create("KCP", Patterns::Vars::g_KanyePatch, KanyePatchHook, &OgKanyePatchHook);

		if (IsValidPtr(Patterns::Vars::g_ConstructJoinMessage))
			Hooking::GetHooking()->Create("CJM", Patterns::Vars::g_ConstructJoinMessage, ConstructJoinMessageHook, &OgConstructJoinMessageHook);

		if (IsValidPtr(Patterns::Vars::g_HandleNetMessage))
			Hooking::GetHooking()->Create("HNM", Patterns::Vars::g_HandleNetMessage, HandleNetMessageHook, &OgHandleNetMessageHook);

		if (IsValidPtr(Patterns::Vars::g_SyncPedOrientation))
			Hooking::GetHooking()->Create("SPO", Patterns::Vars::g_SyncPedOrientation, SyncPedOrientationHook, &OgSyncPedOrientation);

		if (IsValidPtr(Patterns::Vars::g_SyncEntityMatrix))
			Hooking::GetHooking()->Create("SEM", Patterns::Vars::g_SyncEntityMatrix, SyncEntityMatrixHook, &OgSyncEntityMatrix);

		if (IsValidPtr(Patterns::Vars::g_PedMovement))
			Hooking::GetHooking()->Create("PMDN", Patterns::Vars::g_PedMovement, PedMovementDataNodeHook, &OgPedMovementDataNode);

		if (IsValidPtr(Patterns::Vars::g_SendChatMessage))
			Hooking::GetHooking()->Create("SCM", Patterns::Vars::g_SendChatMessage, SendChatMessageHook, &OgSendChatMessage);

		if (IsValidPtr(Patterns::Vars::g_HandlePickupProcessing))
			Hooking::GetHooking()->Create("HPUP", Patterns::Vars::g_HandlePickupProcessing, HandlePickupProcessingHook, &OgHandlePickupProcessingHook);

	//	if (IsValidPtr(Patterns::Vars::g_PackCloneCreate))
		//	Hooking::GetHooking()->Create("PCC2", Patterns::Vars::g_PackCloneCreate, PackCloneCreateHook, &OgPackCloneCreate);

		//if (IsValidPtr(Patterns::Vars::g_PackCloneSync))
				//	Hooking::GetHooking()->Create("PCS", Patterns::Vars::g_PackCloneSync, PackCloneSyncHook, &OgPackCloneSync);

		//if (IsValidPtr(Patterns::Vars::g_SendCloneSync))
			//Hooking::GetHooking()->Create("SCS", Patterns::Vars::g_SendCloneSync, SendCloneSyncHook, &OgSendCloneSync);

		if (IsValidPtr(Patterns::Vars::g_WritePlayerGameStateDataNode))
			Hooking::GetHooking()->Create("WPGSDN", Patterns::Vars::g_WritePlayerGameStateDataNode, WritePlayerGameStateDataNodeHook, &OgWritePlayerGameStateDataNode);

		if (IsValidPtr(Patterns::Vars::g_ReadNewScriptHostMessage))
			Hooking::GetHooking()->Create("HMNSH", Patterns::Vars::g_ReadNewScriptHostMessage - 0x6D, HandleMsgNewScriptHostHook, &OgHandleMsgNewScriptHost);

		/*

/*		if (IsValidPtr(Patterns::Vars::g_CompressDataNode))
			Hooking::GetHooking()->Create("PDN1", Patterns::Vars::g_CompressDataNode, CompressDataNodeHook, &OgCompressDataNodeHook);

		if (IsValidPtr(Patterns::Vars::g_CompressDataNode2))
			Hooking::GetHooking()->Create("PDN2", Patterns::Vars::g_CompressDataNode2, CompressDataNodeHook2, &OgCompressDataNodeHook2);*/
/*
		if (IsValidPtr(Patterns::Vars::g_VehicleControlOutOfBoundsPatch))
			Hooking::GetHooking()->Create("CVOFB", Patterns::Vars::g_VehicleControlOutOfBoundsPatch, VehicleControlOutOfBoundsHook, &OgVehicleControlOutOfBoundsHook);*/

		//if (IsValidPtr(Patterns::Vars::g_WriteDwordToBitBuffer))
		//	Hooking::GetHooking()->Create("WDTBB", Patterns::Vars::g_WriteDwordToBitBuffer, WriteBitBufferDwordHook, &OgWriteBitBufferDwordHook);

		/*if (IsValidPtr(Patterns::Vars::g_ReadDwordFromBitBuffer))
			Hooking::GetHooking()->Create("RDBB", Patterns::Vars::g_ReadDwordFromBitBuffer, ReadBitBufferDwordHook, &OgReadBitBufferDwordHook);*/

	}
	bool ReadBitBufferDwordHook(__int64 bit_buffer, int value, int bits) {
	//	LOG_CUSTOM("RDBB", std::format("buffer: 0x{} | value: {} | bits: {}", bit_buffer, value, bits).c_str());
	
		//wrong
		if (value == -1) {
			LOG_ERROR("RDBB Tried To Read An Invalid Value");
			return false;
		}

		return OgWriteBitBufferDwordHook(bit_buffer, value, bits);
	}

	static unsigned short g_tampered_buffer_count = { 0 };
	bool WriteBitBufferDwordHook(__int64 bit_buffer, int value, int bits) {
		
		

		/*g_tampered_buffer_count++;
		if (g_tampered_buffer_count >= 6969) {
			MainMenuVars::m_Vars.m_Crash = false;
			LOG_WARN("Disabled Crash");
		}

		for (int i = 0; i < 5; i++) {
			Caller::Call<bool>(Patterns::Vars::g_WriteQWordToBitBuffer, bit_buffer, 0x7fff, rand() % 0xff);
			OgWriteBitBufferDwordHook(bit_buffer, value, bits);
			LOG_SUCCESS("Crash Attempt Finished");
		}*/

		//void* _array = nullptr;
		//Caller::Call<bool>(Patterns::Vars::g_WriteInt64ToBitBuffer, bit_buffer, INT_MAX, INT_MAX);
		//Caller::Call<bool>(Patterns::Vars::g_WriteInt32ToBitBuffer, bit_buffer, INT_MAX, INT_MAX);
		//Caller::Call<bool>(Patterns::Vars::g_WriteArrayToBitBuffer, bit_buffer, _array, INT_MAX, -1);

		LOG_CUSTOM("WDTBB", std::format("buffer: 0x{} | value: {} | bits: {}", bit_buffer, value, bits).c_str());
		return OgWriteBitBufferDwordHook(bit_buffer, value, bits);
		//return true;
	}
}