#pragma once
#include "stdafx.hpp"
#include "rage/classes/network/CNetGamePlayer.hpp"
#include "rage/types/base_types.hpp"
#include "util/math.hpp"
namespace Menu {

	enum ePlayerReportReasons {
		REASON_INVALID_MODEL,
		REASON_SPOOFED_NAME,
		REASON_SPOOFED_PEER,
		REASON_SPOOFED_RID,
		REASON_SPOOFED_IP,
		REASON_CRC_FLOOD,
		REASON_CRC_COMPROMISED,
		REASON_CRC_NOT_REPLIED,
		REASON_INFO_CHANGE_1,
		REASON_INFO_CHANGE_2,
		REASON_INFO_CHANGE_4,
		REASON_INFO_CHANGE_5,
		REASON_INFO_CHANGE_6,
		REASON_INFO_CHANGE_7,
		REASON_ARXAN,
		REASON_SPOOFED_CHAT,
		REASON_SPOOFED_TEXT,
		REASON_CRASH_ATTEMPT,
		REASON_WEATHER_CHANGE,
		REASON_SPECTATING,
		REASON_EXPLOSIVE_AMMO,
		REASON_EXPLOSIVE_MELEE,
		REASON_FIRE_AMMO,
		REASON_SUPER_JUMP,
		REASON_MONEY_DROP,
		REASON_RP_DROP,
		REASON_QUANTUM_KICK,
		REASON_NET_GAME_PLAYER_CHEATER,
		REASON_SCRIPT_MIGRATION,
		REASON_PROFANITY,
		REASON_JOIN_TIMEOUT,
		REASON_INFO_MISMATCH,
		REASON_MENU_CHERAX,
		REASON_MENU_IMPULSE,
		REASON_MENU_IMPULSE_VIP,
		REASON_MENU_IMPULSE_STAFF,
		REASON_MENU_IMPULSE_BLOCK,
	};

#pragma pack(push)
#pragma pack(1)
	struct PlayerVars {
		const char* m_Name;
		Player m_ID;
		Ped m_Ped;
		Entity m_Entity;
		Entity m_WeaponEntity;
		Entity m_Vehicle;
		uint32_t m_WeaponHash;
		uint32_t m_VehicleWeaponHash;
		uint32_t m_Model;
		uint32_t m_BlacklistRequests = 0;
		uint32_t m_NetworkModelHash = 0;

		Rage::rlGamerHandle m_NetworkHandle;

		bool m_InVehicle;
		bool m_Exists;
		bool m_Connected;
		bool m_IsShooting;
		bool m_IsAiming;
		bool m_IsFriend;
		bool m_IsOzarkUser;
		bool m_IsImpulseUser;
		bool m_IsModder;
		bool m_UsingGhost;
		bool m_IsInside;
		bool m_IsGodmode;
		bool m_IsScriptHost;
		bool m_HasWeaponInHand;
		bool m_Alive;
		bool m_SyncIntervalToggle;
		bool m_GiftVehicle;

		float m_Heading;
		float m_Health;
		int m_PedHeadshot;
		int m_SyncInterval;

		Math::Vector3<float> m_Coords;
		Math::Vector3<float> m_Rotation;
		CNetGamePlayer* m_NetGamePlayer;
		CPed* m_PedPointer;

		// Network
		bool m_DemiGodmode;
		bool m_OffTheRadar;
		bool m_DisablePolice;
		bool m_Revenge;
		bool m_DropToggle;
		bool m_AlwaysWanted;
		bool m_Ragdoll;
		bool m_DisableTasks;
		bool m_Taze;
		bool m_GlitchPhysics;
		bool m_KickFromVehicle;
		bool m_AttackerInvincibility;
		bool m_AttackerInvisibility;
		bool m_ExplosionSound = true;
		bool m_ExplosionLoop;
		bool m_Rape;
		bool m_HostilePeds;
		bool m_HostileTraffic;
		bool m_FakeMoneyDrop;
		bool m_RainRockets;
		bool m_Forcefield;
		bool m_BlockPassive;
		bool m_ChatCommands;
		bool m_ChatCommand[11];
		bool m_Spectate;
		bool m_ForceVisible;
		bool m_TargetedGlobalChat;
		bool m_DropStealth;

		int m_DropType;
		int m_DropPickup;
		int m_DropWeapon;
		int m_DropMoneyModel;
		int m_DropMoneyStyle;
		int m_DropMoneyDelay = 3;
		int m_Explode;
		int m_CEO;
		int m_SendToID;
		int m_AttackerType;
		int m_AttackerCount = 1;
		int m_ExplosionType;
		int m_ExplosionVisual;
		int m_RapeType;
		int m_CageType;
		int m_Messages;
		int m_CloneType;
		int m_RemoveWeaponGroupType;
		int m_JetType;
		int m_RamType;
		int m_TextMessageColor;
		int m_TextMessageFont;
		int m_TextMessageSize;
		int m_TextMessageCount = 1;
		int m_Crash;

		int m_AttachmentIndex[3];
		int m_BoneIndex;
		std::vector<std::pair<std::string, Object>> m_AttachedObjects;

		float m_ExplosionRadius = 5.f;
		float m_ExplosionCameraShake;

		int m_DemiGodmodeTimer;
		int m_OffTheRadarTimer;
		int m_DisablePoliceTimer;
		int m_DropMoneyTimer;
		int m_TazeTimer;
		int m_KickFromVehicleTimer;
		int m_GlitchPhysicsTimer;
		int m_RagdollTimer;
		int m_HostilePedsTimer;
		int m_HostileTrafficTimer;
		int m_FakeMoneyDropTimer;
		int m_RainRocketsTimer;

		std::string m_TextMessageMsg = "";
		char m_TextMessageFinal[100];
		bool m_ModderFlags[37];
		bool m_GhostDisable[14];
		bool m_BlockEntities[14];

		bool IsValidName() {
			return m_Name != nullptr && strlen(m_Name) > 0;
		}

		void Reset() {
			memset(this, 0, sizeof(PlayerVars));
		}
	};
#pragma pack(pop)

	class PlayerManager {
	public:
		void Update();
		void Foreach(std::function<void(PlayerVars&)> Callback, bool BypassExcludes = false, bool BypassGhost = false);

		PlayerVars& GetPlayer(int Index);
		PlayerVars& GetLocalPlayer();

		PlayerVars m_Players[34];
	};

	PlayerManager* GetPlayerManager();
	PlayerVars& GetPlayer(int Index);
	PlayerVars& GetLocalPlayer();
	PlayerVars& GetSelectedPlayer();
}