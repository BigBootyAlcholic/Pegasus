#include "stdafx.hpp"
#include "engine.hpp"
#include "cheat/menu/submenus/network.hpp"
#include "util/fiber.hpp"
namespace Engine {
	CNetGamePlayer*	GetNetworkGamePlayer(u32 Player) {
		return Caller::Call<CNetGamePlayer*>(Patterns::Vars::g_GetNetworkGamePlayer, Player);
	}
	uint64_t GetEntityAddress(::Entity Entity) {
		return Caller::Call<uint64_t>(Patterns::Vars::g_GetEntityAddress, Entity);
	}
	bool ChangeNetworkObjectOwner(int32_t Index, CNetGamePlayer* Owner) {
		u64 NetworkObjectMgrInterface = *(u64*)(Patterns::Vars::g_NetworkObjectMgr);
		if (NetworkObjectMgrInterface == NULL)
			return false;

		if (!Native::DoesEntityExist(Index))
			return false;

		u64 Entity = Caller::Call<u64>(Patterns::Vars::g_GetEntityFromScript, Index);
		if (Entity == NULL)
			return false;

		u64 NetObject = *(u64*)(Entity + 0xD0);
		if (NetObject == NULL)
			return false;

		if (*(std::uint16_t*)(NetObject + 0x8) == 11)
			return false;

		Caller::Call<u64>(Patterns::Vars::g_ChangeNetworkObjectOwner, NetworkObjectMgrInterface, NetObject, Owner, 0ui64);

		return true;
	}

	void JoinSessionByInfo(Network* _Network, Rage::rlSessionInfo* SessionInfo, int Unk, int Flags, Rage::rlGamerHandle* Handle, int HandleCount) {
		Caller::Call<bool>(Patterns::Vars::g_JoinSessionByInfo, _Network, SessionInfo, Unk, Flags, Handle, HandleCount);
	}

	void HandleRotationValuesFromOrder(Math::Matrix* Matrix, Math::Vector3_<float>* Rotation, int Order) {
		Caller::Call<int>(Patterns::Vars::g_HandleRotationValuesFromOrder, Matrix, Rotation, Order);
	}
}