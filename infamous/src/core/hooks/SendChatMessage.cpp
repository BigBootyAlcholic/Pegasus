#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/menu/submenus/network.hpp"
#include "cheat/util/packet.hpp"
namespace Hooks {

	inline void GetNetworkHandle(Rage::rlGamerHandle& Handle, Rage::datBitBuffer& msgBuffer) {
		constexpr int PC_PLATFORM = 3;
		msgBuffer.Write<uint8_t>(PC_PLATFORM, 8);
		msgBuffer.WriteInt64(*(int64_t*)&Handle.m_rockstar_id, 64);
		msgBuffer.Write<uint8_t>(Handle.unk_0009, 8);
	}

	bool SendChatMessageHook(void* _This, Rage::rlGamerInfo* GamerInfo, char* Message, bool isTeam) {
	/*	Menu::Packet::Packet Msg{};
		Msg.WriteMessage(Rage::eNetMessage::MsgTextMessage);
		Msg.m_MsgBuffer.WriteString(Message ? Message : "", 256);
		GetNetworkHandle(Menu::GetLocalPlayer().m_NetGamePlayer->GetGamerInfo()->m_gamer_handle, Msg.m_MsgBuffer);
		Msg.Write<bool>(isTeam, 1);

		if (Native::NetworkIsSessionStarted()) {
			Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
				Msg.Send(Player.m_NetGamePlayer->m_msg_id);
			}, true);
		}*/

		return OgSendChatMessage(_This, GamerInfo, Message, isTeam);
	}
}