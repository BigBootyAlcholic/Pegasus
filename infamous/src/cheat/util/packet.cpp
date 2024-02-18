#include "stdafx.hpp"
#include "packet.hpp"
#include "core/patterns.hpp"
#include "util/caller.hpp"
#include "rage/classes/network/netConnection.hpp"
#include "rage/classes/network/Network.hpp"
namespace Menu::Packet {
	Packet::Packet() :
		m_MsgBuffer(m_Data, sizeof(m_Data))
	{
	}

	void Packet::Send(uint32_t msgId) {
		Caller::Call<bool>(Patterns::Vars::g_QueuePacket, (*Patterns::Vars::g_Network)->m_game_session_ptr->m_net_connection_mgr, msgId, m_Data, (m_MsgBuffer.m_curBit + 7) >> 3, 1, nullptr);
	}

	void Packet::Send(CNetGamePlayer* Player, int connId) {
		Send(Player->GetGamerInfo()->m_peer_id_2, connId);
		//Caller::Call<bool>(Patterns::Vars::g_QueuePacketWithPlayer);
	}

	void Packet::Send(int peerId, int connId) {
		auto mgr = (*Patterns::Vars::g_Network)->m_game_session_ptr->m_net_connection_mgr;
		auto peer = Caller::Call<Rage::netConnectionPeer*>(Patterns::Vars::g_GetConnectionPeer, mgr, peerId);
		Caller::Call<bool>(Patterns::Vars::g_SendPacket, mgr, &peer->m_peer_address, connId, m_Data, (m_MsgBuffer.m_curBit + 7) >> 3, 0x1000000);
	}
}