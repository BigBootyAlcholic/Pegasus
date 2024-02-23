#pragma once
#include "util/caller.hpp"
#include "core/patterns.hpp"
#include "rage/classes/network/CNetGamePlayer.hpp"
#include "rage/classes/rage/rlSessionInfo.hpp"
namespace Engine {
	CNetGamePlayer* GetNetworkGamePlayer(u32 Player);
	uint64_t GetEntityAddress(::Entity Entity);
	bool ChangeNetworkObjectOwner(int32_t Index, CNetGamePlayer* Owner);
	void JoinSessionByInfo(Network* _Network, Rage::rlSessionInfo* SessionInfo, int Unk, int Flags, Rage::rlGamerHandle* Handle, int HandleCount);
	void JoinSession(const Rage::rlSessionInfo& Info);
	void HandleRotationValuesFromOrder(Math::Matrix* Matrix, Math::Vector3_<float>* Rotation, int Order);
	void SendChatMessage(const char* Message);
	void TriggerScriptEvent(int event_group, int64_t* args, int arg_count, int player_bits);
	void SendRagdollEvent(int Player);


	void JoinSessionByRid(u64 rid);
}