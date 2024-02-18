#pragma once
#include "stdafx.hpp"
#include "util/math.hpp"
#include "base_types.hpp"
#pragma pack(push)
#pragma pack(1)
namespace Rage::Network {
	struct PlayerInfo;
	struct CameraManagerAngles;
	struct CameraAngles;
	struct PlayerAngles;
	struct NetObj;

	struct NetShopQueue {
		char _0x0000[32];
		void* m_First;
		void* m_Last;
		char _0x0030[9];
		bool m_IsBusy1;
		char _0x003A[14];
		bool m_IsBusy2;
		char _0x0049[7];
		bool m_IsBusy3;
	};

	struct GsSession {
		char _0x0000[0x8];
		uint64_t m_PeerToken;
		char _0x0010[0x38];
		uint64_t m_RockstarID;
	};

	struct JoinRequest {
		char _0x0000[0x24];
		int m_DeveloperFlags;
	};

	struct PlayerGamerDataNode {
		char _0x0000[0xC8];
		int m_CrewID;
		char _0x00cc[0x12];
		char m_CrewName[0x19];
		char m_CrewAbbr[0x5];
		char m_CrewMotto[0x4C];
		char m_CrewRank[0x18];
	};

	struct GlobalMsgContext {
		char m_Message[0x100];
		uint64_t m_SenderChatToken;
		bool m_Flag0IsNeg1;
	};

	struct PlayerPeer {
		int m_PlayerKey;
		int m_SecondaryKey;
		char _0x0008[0x4];
		uint32_t m_ExternalIP;
		char _0x0010[0x20];
		uint8_t m_Flag0;
		char _0x0031[0xF];
		uint8_t m_Bitset;
	};

	struct GlobalChatRaw {
		char _0x0000[0x180];
		int m_Flag0;
		char _0x0184[0x1C24];
		PlayerPeer* m_PlayerPeers[0x20];
		int m_PlayerCount;
		char _0x1eac[0xC];
		uint64_t m_ManagerPtr;
		int m_Flag1;
		int m_Flag2;
		char _0x1ec8[0x30];
		uint8_t m_Bitset;
	};

	struct RsInfo {
		char m_ScsTicket[0x100];
		char _0x0100[0x190];
		uint64_t m_Peer;
		char _0x0298[0xA7];
		char m_Email[0x50];
		char _0x038f[0x20];
		char m_Name[0x20];
		char _0x03cf[0x29];
		uint32_t m_RockstarID;
	};

	struct NetworkCrewHandle {
		/*int m_Handle;
		char _0x0004[0x4];
		char m_CrewName[0x14];
		char _0x0000[0x6C];
		char m_CrewNameAbbr[0x4];
		char _0x0001[0x50];

		NetworkCrewHandle() {
			m_Handle = 0;
		}*/

		int m_Handle; // crew ID
		char _0x0004[0x4];
		char m_CrewName[0x18];
		char _0x0020[0x68];
		char m_CrewAbbr[0x4];
		char _0x008c[0x24];
		char m_CrewRank[0x18];
		char _0x00C8[0x14];
	};

	struct NetworkHandle {
		int m_Handle;
		char _0x0[48];
	};

	struct NetMsgIdentifier {
		uint64_t m_RockstarID;
		uint8_t m_Type;
		char _0x0009[0xB];
	};

	struct RockstarIdentifier {
		uint64_t m_RockstarID;
		uint32_t m_Type;
		uint32_t m_SubType;
	};

	struct TextMessageNetMsg {
		char m_Message[0x100];
		NetMsgIdentifier m_Sender;
	};

	struct NetworkGamerCmd {
		char _0x0020[0x2C];
		uint32_t m_IP;
		char _0x0030[0x10];
		uint64_t m_RockstarID;
		uint64_t m_HostToken;
		uint64_t m_ChatToken;
		NetMsgIdentifier m_Identifier;
		char m_Name[0x14];
		char _0x0080[0x8];
		uint32_t m_MsgIP;
		char _0x008C[0x21C];
		NetMsgIdentifier m_PlayerIdentifiers[0x20];

		void SetIP(int Index, uint8_t IP) {
			uint8_t* Val = (uint8_t*)&m_IP;
			Val[Index] = IP;

			Val = (uint8_t*)&m_MsgIP;
			Val[Index] = IP;
		}
	};

	struct GameCameraAngles {
		CameraManagerAngles* m_CameraManagerAngles;
		char _0x0008[56];
	};

	struct CameraManagerAngles {
		CameraAngles* m_CameraAngles;
	};

	struct CameraAngles {
		char _0x0000[0x3C0];
		PlayerAngles* m_FPSPlayerAngles;
		char _0x03C8[0x10];
		uint64_t m_Unknown;
	};

	struct PlayerAngles {
		char _0x0000[0x40];
		Math::Vector3_<float> m_ViewAngles;
		char _0x0004C[0x14];
		Math::Vector3_<float> m_CrosshairLocation;
	};

	struct Friends {
		char _0x0000[128]; //0x0000
		char m_Name[32]; //0x0080
		char _0x00A0[24]; //0x00A0
		uint64_t m_RockstarID; //0x00B8
		char _0x00C0[4]; //0x00C0
		int m_State; //0x00C4
		char _0x00C8[312]; //0x00C8
	};

	struct EventAddedGamer {
		char _0x0000[0x44];         // 0x0000
		uint32_t m_OldIP;			// 0x0044
		char _0x0048[0x10];         // 0x0048
		uint64_t m_RockstarID;      // 0x0058
		char _0x0060[0x24];         // 0x0060
		char m_Name[0x14];          // 0x0084
		char _0x0098[0x8];			// 0x0098
		uint32_t m_ExternalIP;      // 0x00A0
	};

	struct BlacklistedPlayer {
		NetMsgIdentifier m_Player;
		uint32_t m_Timestamp;
		BlacklistedPlayer* m_Next;
		BlacklistedPlayer* m_Last;
	};

	struct NetworkBaseConfig {
		char _0x0000[0x18498]; // 0x184E8
		NetMsgIdentifier m_LastBlacklisted;
		uint32_t m_LastBlacklistedTimestamp;
		char _0x184b0[0x280];
		BlacklistedPlayer* m_FirstBlacklistedPlayer;
		BlacklistedPlayer* m_LastBlacklistedPlayer;

		void SetNetworkSessionWasInvited(bool Invited) {
			*(bool*)((uint64_t)this + 0x8035) = Invited;
		}
	};

	struct KickPeer {
		uint64_t m_Peer;
		char _0x0008[5];
		bool m_Kicked;
		char _0x000E[2];
	};

	struct ListKickPeers {
		KickPeer m_Peers[0x20];
	};

	struct GameInvite {
		char _0x0000[8]; //0x0000
		GsSession gssession_start; //0x0008
		uint64_t h; //0x0058
		uint64_t h2; //0x0060
		char s[77]; //0x0068
		char n[17]; //0x00B5
		char _0x00C6[2]; //0x00C6
		uint32_t gm; //0x00C8
		char c[23]; //0x00CC
		char _0x00E3[5]; //0x00E3
		uint64_t cc; //0x00E8
		uint64_t cc2;
		char mid[30]; //0x00F8
		char _0x0116[2]; //0x0116
		uint32_t if_; //0x0118
		uint32_t it; //0x011C
		uint32_t l; //0x0120
		uint32_t p; //0x0124
		uint32_t f; //0x0128
		uint32_t cr; //0x012C
		uint32_t u; //0x0130
		uint32_t d; //0x0134
		uint32_t jq; //0x0138
	};
}
#pragma pack(pop)