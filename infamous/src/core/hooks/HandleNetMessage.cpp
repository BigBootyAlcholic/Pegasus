#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/util/notify.hpp"
#include "util/caller.hpp"
#include "cheat/menu/submenus/network.hpp"
#include "util/fiber_pool.hpp"
#include "rage/classes/network/CNetGamePlayer.hpp"
#include "rage/classes/network/CNetworkPlayerMgr.hpp"
#include "rage/classes/network/Network.hpp"

inline void GetGamerHandle(Rage::rlGamerHandle& hnd, Rage::datBitBuffer& buf) {
	constexpr int PC_PLATFORM = 3;
	if ((hnd.m_platform = buf.Read<uint8_t>(8)) != PC_PLATFORM)
		return;

	buf.ReadInt64((int64_t*)&hnd.m_rockstar_id, 64);
	hnd.unk_0009 = buf.Read<uint8_t>(8);
}

inline bool IsKickMessage(Rage::datBitBuffer& buffer) {
	Rage::rlGamerHandle sender, receiver;
	char name[18];
	GetGamerHandle(sender, buffer);
	GetGamerHandle(receiver, buffer);
	buffer.ReadString(name, 17);
	int instruction = buffer.Read<int>(32);
	return instruction == 8;
}

namespace Hooks {

	CNetGamePlayer* GetPlayerFromPeer(uint64_t peerAddress) {
		if (auto cNetworkPlayerMgr = *Patterns::Vars::g_NetworkPlayerMgr; cNetworkPlayerMgr) {
			for (uint16_t i{ cNetworkPlayerMgr->m_player_count }; i; --i) {
				if (auto cNetGamePlayer = cNetworkPlayerMgr->m_player_list[i]; cNetGamePlayer && cNetGamePlayer->is_valid()) {
					if (auto rlGamerInfo = cNetGamePlayer->GetGamerInfo()) {
						if (rlGamerInfo->m_host_token == peerAddress) {
							return cNetGamePlayer;
						}
					}
				}
			}
		}
		return nullptr;
	}

	bool GetMsgType(Rage::eNetMessage& msgType, Rage::datBitBuffer& buffer) {
		uint32_t pos;
		uint32_t magic;
		uint32_t length;
		uint32_t extended{};
		if ((buffer.m_flagBits & 2) != 0 || (buffer.m_flagBits & 1) == 0 ? (pos = buffer.m_curBit) : (pos = buffer.m_maxBit),
			buffer.m_bitsRead + 15 > pos || !buffer.ReadDword(&magic, 14) || magic != 0x3246 || !buffer.ReadDword(&extended, 1))
		{
			msgType = Rage::eNetMessage::MsgInvalid;
			return false;
		}
		length = extended ? 16 : 8;
		if ((buffer.m_flagBits & 1) == 0 ? (pos = buffer.m_curBit) : (pos = buffer.m_maxBit),
			length + buffer.m_bitsRead <= pos && buffer.ReadDword((uint32_t*)&msgType, length))
			return true;
		else
			return false;
	}

	static void GetScriptId(CGameScriptId& id, Rage::datBitBuffer& buffer) {
		id.m_hash = buffer.Read<uint32_t>(32);
		id.m_timestamp = buffer.Read<uint32_t>(32);

		if (buffer.Read<bool>(1))
			id.m_position_hash = buffer.Read<uint32_t>(32);

		if (buffer.Read<bool>(1))
			id.m_instance_id = buffer.Read<int32_t>(8);
	}

	bool HandleNetMessageHook(void* _This, void* Rcx, Rage::netConnection::InFrame* Frame) {

		if (Frame->GetEventType() != Rage::netConnection::InFrame::EventType::FrameReceived)
			return OgHandleNetMessageHook(_This, Rcx, Frame);

		if (Frame->m_Data == nullptr || Frame->m_Length == 0)
			return OgHandleNetMessageHook(_This, Rcx, Frame);

		Rage::datBitBuffer MsgBuffer(Frame->m_Data, Frame->m_Length);
		MsgBuffer.m_flagBits = 1;

		CNetGamePlayer* Player{};

		if (Rage::snPlayer * snPlayer{ (*Patterns::Vars::g_Network)->m_game_session_ptr->GetPlayerByPlatformData(Frame->m_PeerId)}) {
			Player = GetPlayerFromPeer(snPlayer->m_player_data.m_host_token);
		}

		Rage::eNetMessage MsgType;

		if (!GetMsgType(MsgType, MsgBuffer))
			return OgHandleNetMessageHook(_This, Rcx, Frame);

		if (Player) {
			switch (MsgType) {
			case Rage::eNetMessage::MsgReassignNegotiate: {
				u32 Mask = MsgBuffer.Read<u32>(8);

				u32 PeerCount = MsgBuffer.Read<uint32_t>(5);

				//if there is only 1 target
				if (PeerCount == 1) {
					u64* Peers = new u64[PeerCount];

					for (u32 i = 0; i < PeerCount; i++) {
						MsgBuffer.ReadQWord(&Peers[i], 64);
						LOG_ERROR("Peer[%i]: %llx", i, Peers[i]);
					}

					
				}
				break;
			}
			case Rage::eNetMessage::MsgNetComplaint: {
				//No need to check if we're host, it'll backfire regardless. It'll only false complain.
				if (!(*Patterns::Vars::g_NetworkPlayerMgr)->m_local_net_player->is_host()) {
					if (auto net = (*Patterns::Vars::g_Network); net) {
						auto complaintMgr = net->m_game_complaint_mgr;
						uint64_t peerAddress;
						MsgBuffer.ReadQWord(&peerAddress, 64);
						uint32_t numberOfpeerAddresses{};
						MsgBuffer.ReadDword(&numberOfpeerAddresses, 32);
						if (numberOfpeerAddresses <= 64) {
							//We only need the first element, so we only need to grab one
							uint64_t arrayElement{};
							MsgBuffer.ReadQWord(&arrayElement, 64);
							if (arrayElement != NULL) {
							
								LOG_ERROR("Desync Kick");
								complaintMgr.remove_complaint(arrayElement);
								MsgBuffer.Seek(0);
								return false;
												
							}
						}
						int kickType{};
						if (complaintMgr.m_num_tokens_complained < 1) {
							kickType = 1;
							LOG_ERROR("Complaint Kick");
							MsgBuffer.Seek(0);
							return false;
						}
					}
				}
				break;
			} 
			case Rage::eNetMessage::MsgScriptMigrateHost: {
				if (Player->m_player_id != Native::PlayerId()) {
					Menu::Notify::stacked(std::format("Blocked Kick (N0) From {}", Player->get_name()).c_str());
					return true;
				}
				break;
			}
			case Rage::eNetMessage::MsgLostConnectionToHost: {
				Menu::Notify::stacked(std::format("Blocked Kick (H0) From {}", Player->get_name()).c_str());
				return true;
				break;
			}
			case Rage::eNetMessage::MsgRadioStationSyncRequest: {
				Menu::Notify::stacked(std::format("Blocked Kick (N0) From {}", Player->get_name()).c_str());
				return true;
				break;
			}
			}
		}
		else {
			switch (MsgType) {
			case Rage::eNetMessage::MsgScriptMigrateHost: return true;
			case Rage::eNetMessage::MsgRadioStationSyncRequest: return true;
			}
		}

		return OgHandleNetMessageHook(_This, Rcx, Frame);
	}

}
