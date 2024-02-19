#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "core/core.hpp"


class ProtectionMenu {
public:
	void Run();
	void Reports();
	void Events();
	void Update();
};

static ProtectionMenu* GetProtectionMenu() {
	static ProtectionMenu instance;
	return &instance;
}

namespace ProtectionMenuVars {

	struct EntityLimit {
		uint32_t m_Model;
		int m_Delay = 500;
		char m_Comment[0x100];
		bool m_Enabled = true;
		uint32_t m_LastSpawned = 0;
	};
	struct Ghost {
		bool m_Enabled = false;
		bool m_DisableForSession = false;
		eNetObjectTypes m_Type;
		std::string m_Name;
	};
	struct Vars_ {

		bool m_Godmode = true;
		bool m_SuperJump = true;
		bool m_Health = true;
		bool m_Armor = true;
		bool m_Spectate = true;

		bool m_BlockAllScriptEvents = false;

		Ghost m_Ghosts[14];
		bool m_BlockIncomingSyncs;
		bool m_BlockOutGoingSyncs;

		bool m_EntityLimits[14];

		bool m_BlockModderSync;
		bool m_BlockModderCreate;

		bool m_MetricLog = true;
		bool m_MetricNotify = true;

		bool m_ArxanLog = true;
		bool m_ArxanNotify = true;

		bool m_BonusLog = true;
		bool m_BonusNotify = true;

		bool m_RacLog = true;
		bool m_RacNotify = true;

		ProtectionEventContext m_GiveWeapon = { true, false, true, true };
		ProtectionEventContext m_RemoveWeapon = { true, false, true, true };
		ProtectionEventContext m_RemoveAllWeapons = { true, false, true, true };
		ProtectionEventContext m_Freeze = { true, false, true, true };
		ProtectionEventContext m_Explosion = { true, false, true, true };
		ProtectionEventContext m_Fire = { true, false, true, true };
		ProtectionEventContext m_Ragdoll = { true, false, true, true };
		ProtectionEventContext m_PTFX = { true, false, true , true };
		ProtectionEventContext m_Sound = { true, false, true, true };
		ProtectionEventContext m_Control = { true, false, true, false };

		ProtectionEventContext m_CrashEvent = { true, false, true, false };//
		ProtectionEventContext m_KickEvent = { true, false, true, false };//

		//script events
		ProtectionEventContext m_Bounty = { true, false, true, false };//
		ProtectionEventContext m_CeoKick = { true, false, true, false };//
		ProtectionEventContext m_CeoMoney = { true, false, true, false };//
		ProtectionEventContext m_ClearWantedLevel = { true, false, true, false };//
		ProtectionEventContext m_Notification = { true, false, true, false };//
		ProtectionEventContext m_ForceToMission = { true, false, true, false };//
		ProtectionEventContext m_GiveCollectible = { true, false, true, false };//
		ProtectionEventContext m_Banner = { true, false, true, false };//
		ProtectionEventContext m_Teleport = { true, false, true, false };//
		ProtectionEventContext m_SendToClubHouse = { true, false, true, false };//
		ProtectionEventContext m_TeleportToWarehouse = { true, false, true, false };//
		ProtectionEventContext m_InsuranceMessage = { true, false, true, false };//
		ProtectionEventContext m_RemoteOffRadar = { true, false, true, false };//
		ProtectionEventContext m_RotateCam = { true, false, true, false };//
		ProtectionEventContext m_SendToIsland = { true, false, true, false };//
		ProtectionEventContext m_ForceToCutscene = { true, false, true, false };//
		ProtectionEventContext m_SpectateMessage = { true, false, true, false };
		ProtectionEventContext m_VehicleKick = { true, false, true, false };//
		ProtectionEventContext m_ForceToActivity = { true, false, true, false };//
		ProtectionEventContext m_DestroyPersonalVehicle = { true, false, true, false };//
		ProtectionEventContext m_KickFromInterior = { true, false, true, false };//
		ProtectionEventContext m_TransactionError = { true, false, true, false };//

		std::vector<EntityLimit> m_Limits;
		bool m_AutoDetect;
	};

	extern Vars_ m_Vars;
}
