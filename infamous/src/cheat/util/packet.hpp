#pragma once
#include "stdafx.hpp"
#include "rage/classes/network/netEvent.hpp"
#include "player_manager.hpp"
namespace Menu::Packet {
	class Packet {
	public:
		char m_Data[0x400]{};
		Rage::datBitBuffer m_MsgBuffer;

		Packet();
		void Send(uint32_t msgId);
		void Send(CNetGamePlayer* Player, int connId);
		void Send(int peerId, int connId);

		inline operator Rage::datBitBuffer& () {
			return m_MsgBuffer;
		}

		template<typename T>
		inline void Write(T data, int length) {
			m_MsgBuffer.Write<T>(data, length);
		}

		inline void WriteMessage(Rage::eNetMessage message) {
			Write<int>(0x3246, 14);
			if ((int)message > 0xFF) {
				Write<bool>(true, 1);
				Write<Rage::eNetMessage>(message, 16);
			}
			else {
				Write<bool>(false, 1);
				Write<Rage::eNetMessage>(message, 8);
			}
		}

		inline void WritePeerId(uint64_t peer_id) {
			char b[8];
			Rage::datBitBuffer buf(b, 8);
			buf.WriteQWord(peer_id, 64);
			m_MsgBuffer.WriteArray(b, 8 * buf.GetDataLength());
		}
	};
}