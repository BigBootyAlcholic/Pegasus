#pragma once
#include <cstdint>
#include "../rage/rlGamerInfoBase.hpp"
#include "netPeerAddress.hpp"

#pragma pack(push, 1)
namespace Rage
{
	class netConnectionManager;
	class netConnectionPeer;

	class netQueuedMessage
	{
	public:
		void* m_data_buffer;
		void* qword8;
		void* qword10;
		void* qword18;
		Rage::netQueuedMessage* m_next;
		void* qword28;
		char gap30[8];
		int m_creation_time;
		int m_last_send_time;
		int m_resend_count;
		char gap44[4];
		int dword48;
		uint16_t word4C;
		char byte4E;
	};
	static_assert(sizeof(netQueuedMessage) == 0x4F);

	class netMessageQueue
	{
	public:
		Rage::netQueuedMessage* m_first;
		Rage::netQueuedMessage* m_last;
		uint64_t m_count;
	};

	class netPackedMessage
	{
	public:
		void* m_data_buffer;
		void* m_allocator;
		void* qword10;
		void* qword18;
	};

	class netPackedMessageQueue
	{
	public:
		Rage::netPackedMessage* m_first;
		Rage::netPackedMessage* m_last;
		uint64_t m_count;
	};

	class netConnection
	{
	public:
		class InFrame
		{
		public:
			enum class EventType
			{
				ConnectionClosed = 3,
				FrameReceived = 4,
				BandwidthExceeded = 6,
				OutOfMemory = 7
			};

			virtual ~InFrame() = default;

			virtual void Destroy() = 0;
			virtual EventType GetEventType() = 0;
			virtual uint32_t _0x18() = 0;

			uint32_t m_TimeStamp;             //0x0008
			char pad_0008[52];                //0x000C
			uint32_t m_MsgId;                //0x0040
			uint32_t m_ConnectionId; //0x0044
			InFrame* m_This;                  //0x0048
			uint32_t m_PeerId;               //0x0050
			char pad_0050[44];                //0x0058
			uint32_t m_Length;                //0x0080
			char pad_007C[4];                 //0x0084
			void* m_Data;                     //0x0088
		};
		static_assert(sizeof(Rage::netConnection::InFrame) == 0x90);

		char gap0[8];
		Rage::netConnectionPeer* m_ConnectionPeer;
		int m_MsgId;
		uint32_t m_ConnectionId;
		void* m_Alllocator;
		uint32_t m_ConnectionState;
		uint32_t m_LastSendTime;
		uint32_t m_LastRecieveTime;
		uint32_t m_FailedMessages;
		char gap30[8];
		uint32_t m_TimeoutReason;
		uint32_t dword3C;
		uint32_t m_Timeout;
		uint32_t dword44;
		uint32_t m_ResendMax;
		char gap4C[4];
		Rage::netMessageQueue m_ReliableResendMessageQueue;
		Rage::netMessageQueue m_NormalMessageQueue;
		Rage::netQueuedMessage* m_UnackedReliableMsgList;
		int m_UnackedReliableMsgCount;
		char gap8C[36];
		netConnectionManager* m_NetConnectionMgr;
		char gapB8[8];
		uint32_t dwordC0;
		int16_t m_MsgCounter;
		int16_t wordC6;
		char gapC8[2];
		int16_t m_LastReliableMsgCounter;
		char m_Flags;
		char gapCD[3];
		int m_FailedAllocationSize;
		int32_t m_FailedAllocation;
		Rage::netConnection* m_Next;
		char gapE0[208];
		int m_Flags2;
		char gap1B4[69];
	};
	static_assert(sizeof(netConnection) == 0x1F9);

	class netConnectionQueue
	{
	public:
		Rage::netConnection* m_first;
		Rage::netConnection* m_last;
		uint64_t m_count;
	};

	class netConnectionPeer
	{
	public:
		Rage::netConnection* m_connections_by_id[16];
		Rage::netConnectionQueue m_net_connection_queue;
		Rage::netPackedMessageQueue m_packed_message_queue;
		void* qwordB0;
		char byteB8;
		char gapB9[3];
		int intBC;
		uint32_t dwordC0;
		char gapC4[4];
		void* qwordC8;
		Rage::netPeerAddress m_relay_address;
		Rage::rlGamerInfoBase m_gamer_info;
		char gap1B0[24];
		uint32_t dword1C8;
		char gap1CC[28];
		uint32_t m_security_id;
		char gap1EC[28];
		void* qword208;
		char gap210[24];
		Rage::netPeerAddress m_peer_address;
		Rage::netConnectionPeer* m_next;
		char gap250[8];
		int m_time_until_next_batch;
		int m_empty_batch_interval;
		uint32_t m_time_until_timeout;
		int m_last_msg_process_time;
		int gap268;
		void* qword26C;
		char gap274[4];
		void* qword278;
		char gap280[24];
		void* qword298;
		char gap2A0[64];
		uint32_t m_peer_id;
		char byte2E4;
		char more[51];
		int gap318;
		char gap31C[24];
		int m_num_encryption_attempts;
		char gap338[60];
		int m_num_messages_batched;
		int m_num_reliable_messages_batched;
		int m_num_resent_reliable_messages_batched;
		char gap380[145];
	};
	static_assert(sizeof(netConnectionPeer) == 0x411);
}
#pragma pack(pop)