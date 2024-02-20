#include "stdafx.hpp"
#include "engine.hpp"
#include "cheat/menu/submenus/network.hpp"
#include "util/fiber.hpp"
#include "core/hooks.hpp"
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

	void DrawChat(const char* msg, const char* player_name, bool is_team)
	{
		int scaleform = Native::RequestScaleformMovie("MULTIPLAYER_CHAT");

		while (!Native::HasScaleformMovieLoaded(scaleform))
			Utils::GetFiberManager()->GoToMainFiber();

		Native::_PushScaleformMovieFunction(scaleform, "ADD_MESSAGE");
		Native::_0xE83A3E3557A56640(player_name); // player name
		Native::_0x77FE3402004CD1B0((Any)msg);             // content
		Native::_PushScaleformMovieFunctionParameterString(Native::_GetLabelText(is_team ? "MP_CHAT_TEAM" : "MP_CHAT_ALL")); // scope
		Native::_PushScaleformMovieFunctionParameterBool(false);                               // teamOnly
		Native::_PushScaleformMovieFunctionParameterInt((int)eHudColors::HUD_COLOUR_PURE_WHITE); // eHudColour
		Native::_PopScaleformMovieFunctionVoid();

		Native::_PushScaleformMovieFunction(scaleform, "SET_FOCUS");
		Native::_PushScaleformMovieFunctionParameterInt(1);                                    // VISIBLE_STATE_DEFAULT
		Native::_PushScaleformMovieFunctionParameterInt(0);                                    // scopeType (unused)
		Native::_PushScaleformMovieFunctionParameterInt(0);                                    // scope (unused)
		Native::_0xE83A3E3557A56640(player_name);           // player
		Native::_PushScaleformMovieFunctionParameterInt((int)eHudColors::HUD_COLOUR_PURE_WHITE); // eHudColour
		Native::_PopScaleformMovieFunctionVoid();

		Native::DrawScaleformMovieFullscreen(scaleform, 255, 255, 255, 255, 0);

		// fix broken scaleforms, when chat alrdy opened
		if (const auto chat_data = *Patterns::Vars::g_ChatData; chat_data && (chat_data->m_chat_open || chat_data->m_timer_two))
			Native::_AbortTextChat();
	}

	void SendChatMessage(const char* Message) {
		if (Hooks::OgSendChatMessage(*Patterns::Vars::g_SendChatMessagePtr, Menu::GetLocalPlayer().m_NetGamePlayer->GetGamerInfo(), (char*)Message, false)) {
			DrawChat(Message, Menu::GetLocalPlayer().m_Name, false);
		}
	}

	void TriggerScriptEvent(int event_group, int64_t* args, int arg_count, int player_bits) {
		Caller::Call<void>(Patterns::Vars::g_TriggerScriptEvent, event_group, args, arg_count, player_bits);
	}

	void SendRagdollEvent(int Player) {
		if (Menu::GetPlayer(Player).m_PedPointer) {
			if (Menu::GetPlayer(Player).m_PedPointer->m_net_object) {
				Caller::Call<int>(Patterns::Vars::g_SendRagdollEvent, Menu::GetPlayer(Player).m_PedPointer->m_net_object->m_object_id);
			}
		}
	}
}