#include "stdafx.hpp"
#include "../hooks.hpp"
#include "rage/classes/enums/enums.hpp"
#include "cheat/util/notify.hpp"
#include "util/caller.hpp"
#include "rage/classes/ped/CPed.hpp"
#include "rage/classes/entities/CEntity.hpp"
#include "rage/classes/network/netObject.hpp"
#include "rage/classes/script/CGameScriptObjInfo.hpp"
#include "rage/classes/base/CObject.hpp"
#include "cheat/menu/submenus/protection.hpp"
#include "util/fiber_pool.hpp"
#include "cheat/util/event.hpp"
/*CRespawnPlayerPedEvent::CRespawnPlayerPedEvent() :
netGameEvent( RESPAWN_PLAYER_PED_EVENT, true ),
m_bShouldReply(false),
m_spawnPoint(Vector3(0.0f, 0.0f, 0.0f)),
m_timeStamp(0),
m_RespawnNetObjId(NETWORK_INVALID_OBJECT_ID),
m_timeRespawnNetObj(0),
m_LastFrameScopeFlagsUpdated(0),
m_RespawnScopeFlags(0),
m_ResurrectPlayer(false),
m_EnteringMPCutscene(false),
m_RespawnFailed(false),
m_HasMoney(true),
m_HasScarData(false)
{
}*/

/*CRespawnPlayerPedEvent::CRespawnPlayerPedEvent(const Vector3 &spawnPoint, u32 timeStamp, const ObjectId respawnNetObjId, u32 timeRespawnNetObj, bool resurrectPlayer, bool enteringMPCutscene, bool hasMoney, bool hasScarData, float u, float v, float scale) :
netGameEvent( RESPAWN_PLAYER_PED_EVENT, true ),
m_bShouldReply(false),
m_spawnPoint(spawnPoint),
m_timeStamp(timeStamp),
m_RespawnNetObjId(respawnNetObjId),
m_timeRespawnNetObj(timeRespawnNetObj),
m_LastFrameScopeFlagsUpdated(0),
m_RespawnScopeFlags(0),
m_ResurrectPlayer(resurrectPlayer),
m_EnteringMPCutscene(enteringMPCutscene),
m_RespawnFailed(false),
m_HasMoney(hasMoney),
m_HasScarData(hasScarData),
m_scarU(u),
m_scarV(v),
m_scarScale(scale)
{
}*/

static void GetScriptID(CGameScriptId& id, Rage::datBitBuffer& buffer)
{
	id.m_hash = buffer.Read<uint32_t>(32);
	id.m_timestamp = buffer.Read<uint32_t>(32);

	if (buffer.Read<bool>(1))
		id.m_position_hash = buffer.Read<uint32_t>(32);

	if (buffer.Read<bool>(1))
		id.m_instance_id = buffer.Read<int32_t>(8);
}

static bool IsValidWeapon(Rage::joaat_t hash)
{
	for (const auto& info : Patterns::Vars::g_WeaponInfoManager->m_item_infos)
	{
		if (info && info->m_name == hash && info->GetClassId() == RAGE_JOAAT("cweaponinfo"))
		{
			return true;
		}
	}

	return false;
}

void CheckWeaponDamageEvent(CNetGamePlayer* player, Rage::datBitBuffer* buffer)
{
	uint8_t damageType;
	uint32_t weaponType; // weaponHash

	bool overrideDefaultDamage;
	bool hitEntityWeapon;
	bool hitWeaponAmmoAttachment;
	bool silenced;

	uint32_t damageFlags;
	bool hasActionResult;

	uint32_t actionResultName;
	uint16_t actionResultId;
	uint32_t f104;

	uint32_t weaponDamage;
	bool isNetTargetPos;

	Rage::fvector3 localPos;

	bool f112;

	uint32_t damageTime;
	bool willKill;
	uint32_t f120;
	bool hasVehicleData;

	uint16_t f112_1;

	uint16_t parentGlobalId; // Source entity?
	uint16_t hitGlobalId;    // Target entity?

	uint8_t tyreIndex;
	uint8_t suspensionIndex;
	uint8_t hitComponent;

	bool f133;
	bool hasImpactDir;

	Rage::fvector3 impactDir;

	damageType = buffer->Read<uint8_t>(2);
	weaponType = buffer->Read<uint32_t>(32);

	overrideDefaultDamage = buffer->Read<uint8_t>(1);
	hitEntityWeapon = buffer->Read<uint8_t>(1);
	hitWeaponAmmoAttachment = buffer->Read<uint8_t>(1);
	silenced = buffer->Read<uint8_t>(1);

	damageFlags = buffer->Read<uint32_t>(24);
	// (damageFlags >> 1) & 1
	hasActionResult = buffer->Read<uint8_t>(1);

	if (hasActionResult)
	{
		actionResultName = buffer->Read<uint32_t>(32);
		actionResultId = buffer->Read<uint16_t>(16);
		f104
			= buffer->Read<uint32_t>(32);
	}

	if (overrideDefaultDamage)
	{
		weaponDamage = buffer->Read<uint32_t>(17);
	}
	else
	{
		weaponDamage = 0;
	}

	bool _f92 = buffer->Read<uint8_t>(1);

	if (_f92)
	{
		buffer->Read<uint8_t>(4);
	}

	isNetTargetPos = buffer->Read<uint8_t>(1);

	if (isNetTargetPos)
	{
		localPos.x = buffer->ReadSignedFloat(16, 55.f); // divisor: 0x425C0000
		localPos.y = buffer->ReadSignedFloat(16, 55.f);
		localPos.z = buffer->ReadSignedFloat(16, 55.f);
	}

	if (damageType == 3)
	{
		damageTime = buffer->Read<uint32_t>(32);
		willKill = buffer->Read<uint8_t>(1);

		if (hasActionResult)
		{
			hitGlobalId = buffer->Read<uint16_t>(13);
		}
		else
		{
			hitGlobalId = 0;
		}

		f112 = buffer->Read<uint8_t>(1);

		if (!f112)
		{
			f112_1 = buffer->Read<uint16_t>(11);
		}
		else
		{
			f112_1 = buffer->Read<uint16_t>(16);
		}
	}
	else
	{
		parentGlobalId = buffer->Read<uint16_t>(13); // +118
		hitGlobalId = buffer->Read<uint16_t>(13); // +120
	}

	if (damageType < 2)
	{
		localPos.x = buffer->ReadSignedFloat(16, 55.f); // divisor: 0x425C0000
		localPos.y = buffer->ReadSignedFloat(16, 55.f);
		localPos.z = buffer->ReadSignedFloat(16, 55.f);

		if (damageType == 1)
		{
			hasVehicleData = buffer->Read<uint8_t>(1);

			if (hasVehicleData)
			{
				tyreIndex = buffer->Read<uint8_t>(4); // +122
				suspensionIndex = buffer->Read<uint8_t>(4); // +123
			}
		}
	}
	else
	{
		hitComponent = buffer->Read<uint8_t>(5); // +108
	}

	f133 = buffer->Read<uint8_t>(1);
	hasImpactDir = buffer->Read<uint8_t>(1);

	if (hasImpactDir)
	{
		impactDir.x = buffer->ReadSignedFloat(16, 6.2831854820251f); // divisor: 0x40C90FDB
		impactDir.y = buffer->ReadSignedFloat(16, 6.2831854820251f);
		impactDir.z = buffer->ReadSignedFloat(16, 6.2831854820251f);
	}

	buffer->Seek(0);

	// LOG(INFO) << localPos.x << " " << localPos.y << " " << localPos.z << ". " << math::distance_between_vectors(localPos, *g_local_player->m_navigation->get_position()) << " " << (g_local_player->m_net_object->m_object_id == hitGlobalId ? "TRUE" : "FALSE") << " " << damageType << " " << damageFlags;

	if (damageType == 3 && (damageFlags & (1 << 1)) == 0)
		hitGlobalId = (*Patterns::Vars::g_PedFactory)->m_local_ped ? (*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object->m_object_id : 0;

}

void CheckExplosionEvent(CNetGamePlayer* player, Rage::datBitBuffer* buffer)
{
	uint16_t f186;
	uint16_t f208;
	int ownerNetId;
	uint16_t f214;
	eExplosionTag explosionType;
	float damageScale;

	float posX;
	float posY;
	float posZ;

	bool f242;
	uint16_t f104;
	float cameraShake;

	bool isAudible;
	bool f189;
	bool isInvisible;
	bool f126;
	bool f241;
	bool f243;

	uint16_t f210;

	float unkX;
	float unkY;
	float unkZ;

	bool f190;
	bool f191;

	uint32_t f164;

	float posX224;
	float posY224;
	float posZ224;

	bool f240;
	uint16_t f218;
	bool f216;

	// clang-format off

	f186 = buffer->Read<uint16_t>(16);
	f208 = buffer->Read<uint16_t>(13);
	ownerNetId = buffer->Read<uint16_t>(13);
	f214 = buffer->Read<uint16_t>(13);               // 1604+
	explosionType = (eExplosionTag)buffer->ReadSigned<int>(8);// 1604+ bit size
	damageScale = buffer->Read<int>(8) / 255.0f;

	posX = buffer->ReadSignedFloat(22, 27648.0f);
	posY = buffer->ReadSignedFloat(22, 27648.0f);
	posZ = buffer->ReadFloat(22, 4416.0f) - 1700.0f;

	f242 = buffer->Read<uint8_t>(1);
	f104 = buffer->Read<uint16_t>(16);
	cameraShake = buffer->Read<int>(8) / 127.0f;

	isAudible = buffer->Read<uint8_t>(1);
	f189 = buffer->Read<uint8_t>(1);
	isInvisible = buffer->Read<uint8_t>(1);
	f126 = buffer->Read<uint8_t>(1);
	f241 = buffer->Read<uint8_t>(1);
	f243 = buffer->Read<uint8_t>(1);// 1604+

	f210 = buffer->Read<uint16_t>(13);

	unkX = buffer->ReadSignedFloat(16, 1.1f);
	unkY = buffer->ReadSignedFloat(16, 1.1f);
	unkZ = buffer->ReadSignedFloat(16, 1.1f);

	f190 = buffer->Read<uint8_t>(1);
	f191 = buffer->Read<uint8_t>(1);
	f164 = buffer->Read<uint32_t>(32);

	if (f242)
	{
		posX224 = buffer->ReadSignedFloat(31, 27648.0f);
		posY224 = buffer->ReadSignedFloat(31, 27648.0f);
		posZ224 = buffer->ReadFloat(31, 4416.0f) - 1700.0f;
	}
	else
	{
		posX224 = 0;
		posY224 = 0;
		posZ224 = 0;
	}

	auto f168 = buffer->Read<uint32_t>(32);// >= 1868: f_168


	f240 = buffer->Read<uint8_t>(1);
	if (f240)
	{
		f218 = buffer->Read<uint16_t>(16);

		if (f191)
		{
			f216 = buffer->Read<uint8_t>(8);
		}
	}

	buffer->Seek(0);

	auto object = Caller::Call<Rage::netObject*>(Patterns::Vars::g_GetNetObject, *Patterns::Vars::g_NetworkObjectMgr, ownerNetId, true);
	auto entity = object ? object->GetGameObject() : nullptr;

	auto offset_object = Caller::Call<Rage::netObject*>(Patterns::Vars::g_GetNetObject, *Patterns::Vars::g_NetworkObjectMgr, f210, true);

	if (f208 == 0 && entity && entity->m_entity_type == 4 && reinterpret_cast<CPed*>(entity)->m_player_info
		&& player->m_player_info->m_ped && player->m_player_info->m_ped->m_net_object
		&& ownerNetId != player->m_player_info->m_ped->m_net_object->m_object_id && !offset_object)
	{


		Menu::Notify::stacked(std::format("{} Blamed you For Explosion", player->get_name()).c_str());
		// too many false positives, disabling it
		//session::add_infraction(g_player_service->get_by_id(player->m_player_id), Infraction::BLAME_EXPLOSION_DETECTED);
		return;
	}

	// clang-format on
}


namespace Hooks {


	GtaThread* FindScriptThread(Rage::joaat_t hash) {
		for (auto thread : *Patterns::Vars::g_ScriptThreads) {
			if (thread && thread->m_context.m_thread_id && thread->m_handler && thread->m_script_hash == hash) {
				return thread;
			}
		}

		return nullptr;
	}


	void SendEventAck(void* _this, CNetGamePlayer* fromPlayer, CNetGamePlayer* toPlayer, int eventIndex, int eventBitset) {
		Caller::Call<u64>(Patterns::Vars::g_SendEventAck, _this, fromPlayer, toPlayer, eventIndex, eventBitset);
		//LOG_WARN("Acknowledged Event %d", eventIndex);
	}


	bool ScriptedEventHandler(CScriptedGameEvent* Event, CNetGamePlayer* fromPlayer) {
		const auto Arguments = Event->m_args;
		const auto Hash = static_cast<eRemoteEvent>(Arguments[0]);
		const auto playerName = fromPlayer->get_name();

		switch (Hash)
		{
		case eRemoteEvent::Bounty:
			if (ProtectionMenuVars::m_Vars.m_Bounty.m_Block && Arguments[3] == Native::PlayerId()) {
				Menu::Notify::stacked(std::format("Blocked Bounty From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::CeoKick:
			if (ProtectionMenuVars::m_Vars.m_CeoKick.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Ceo Kick From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::CeoMoney:
			if (ProtectionMenuVars::m_Vars.m_CeoMoney.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Ceo Money From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::ClearWantedLevel:
			if (ProtectionMenuVars::m_Vars.m_ClearWantedLevel.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Clear Wanted Level From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::Crash: Menu::Notify::stacked(std::format("Blocked S0 Crash From {}", playerName).c_str()); return true;
		case eRemoteEvent::Crash2:
			//if (Arguments[3] > 32) // actual crash condition is if args[2] is above 255
			//{
			Menu::Notify::stacked(std::format("Blocked S2 Crash From {}", playerName).c_str());
			return true;
			//}
			break;
		case eRemoteEvent::Crash3:
		{
			if (isnan(*(float*)&Arguments[4]) || isnan(*(float*)&Arguments[5])) {
				Menu::Notify::stacked(std::format("Blocked S3 Crash From {}", playerName).c_str());
				return true;
			}
			break;
		}
		case eRemoteEvent::Notification:
		{
			switch (static_cast<eRemoteEvent>(Arguments[3]))
			{
			case eRemoteEvent::NotificationMoneyBanked: // never used
			case eRemoteEvent::NotificationMoneyRemoved:
			case eRemoteEvent::NotificationMoneyStolen: return true;
			case eRemoteEvent::NotificationCrash1:                             // this isn't used by the game			
				return true;
			case eRemoteEvent::NotificationCrash2:
				if (!FindScriptThread(RAGE_JOAAT("gb_salvage"))) {
					return true;
				}
				break;
			}

			break;
		}
		case eRemoteEvent::ForceMission:
			if (ProtectionMenuVars::m_Vars.m_ForceToMission.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Force to Mission From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::GiveCollectible:
			if (ProtectionMenuVars::m_Vars.m_GiveCollectible.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Give Collectible From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::GtaBanner:
			if (ProtectionMenuVars::m_Vars.m_Banner.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Game Banner From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::MCTeleport:
			if (ProtectionMenuVars::m_Vars.m_SendToClubHouse.m_Block && Arguments[4] <= 32) {
				for (int i = 0; i < 32; i++) {
					if (Arguments[5 + i] == Native::NetworkHashFromPlayerHandle(Native::PlayerId())) {
						Menu::Notify::stacked(std::format("Blocked Send to Clubhouse From {}", playerName).c_str());
						return true;
					}
				}
			}
			else if (Arguments[4] > 32)
			{
				Menu::Notify::stacked(std::format("Blocked Gang Crash From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::PersonalVehicleDestroyed:
			if (ProtectionMenuVars::m_Vars.m_DestroyPersonalVehicle.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Destroy Personal Vehicle From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::RemoteOffradar:
			if (ProtectionMenuVars::m_Vars.m_RemoteOffRadar.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Remote off Radar From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::TSECommand:
			if (ProtectionMenuVars::m_Vars.m_RotateCam.m_Block && static_cast<eRemoteEvent>(Arguments[3]) == eRemoteEvent::TSECommandRotateCam && !Native::NetworkIsActivitySession()) {
				Menu::Notify::stacked(std::format("Blocked Rotate Camera From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::SendToCayoPerico:
			if (ProtectionMenuVars::m_Vars.m_SendToIsland.m_Block && Arguments[4] == 0) {
				Menu::Notify::stacked(std::format("Blocked Send to Island From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::SendToCutscene:
			if (ProtectionMenuVars::m_Vars.m_ForceToCutscene.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Force to Cutscene From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::SendToLocation:
		{

			bool known_location = false;

			if (Arguments[3] == 0 && Arguments[4] == 0) {
				if (Arguments[5] == 4 && Arguments[6] == 0) {
					known_location = true;

					if (ProtectionMenuVars::m_Vars.m_Teleport.m_Block) {
						Menu::Notify::stacked(std::format("Blocked Force to Random Location From {}", playerName).c_str());
						return true;
					}
				}
				else if ((Arguments[5] == 3 || Arguments[5] == 4) && Arguments[6] == 1)	{
					known_location = true;

					if (ProtectionMenuVars::m_Vars.m_Teleport.m_Block) {
						Menu::Notify::stacked(std::format("Blocked Force to Random Location From {}", playerName).c_str());
						return true;
					}
				}
			}

			if (!known_location) {
				return true;
			}
			break;
		}
		case eRemoteEvent::SoundSpam:
		{
			if (ProtectionMenuVars::m_Vars.m_Sound.m_Block) {
				return true;
			}
			break;
		}
		case eRemoteEvent::Spectate:
			if (ProtectionMenuVars::m_Vars.m_SpectateMessage.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Spectate Message From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::Teleport:
			if (ProtectionMenuVars::m_Vars.m_Teleport.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Teleport From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::TransactionError: return true;
		case eRemoteEvent::VehicleKick:
			if (ProtectionMenuVars::m_Vars.m_VehicleKick.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Vehicle Kick From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::NetworkBail:
			return true;
		case eRemoteEvent::TeleportToWarehouse:
			if (ProtectionMenuVars::m_Vars.m_TeleportToWarehouse.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Teleport to Warehouse From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::StartActivity:
		{
			eActivityType activity = static_cast<eActivityType>(Arguments[3]);
			if (ProtectionMenuVars::m_Vars.m_ForceToActivity.m_Block) {
				if (activity == eActivityType::Survival || activity == eActivityType::Mission || activity == eActivityType::Deathmatch || activity == eActivityType::BaseJump || activity == eActivityType::Race) {
					Menu::Notify::stacked(std::format("Blocked Freeze From {}", playerName).c_str());
					return true;
				}
				else if (activity == eActivityType::Darts) {
					Menu::Notify::stacked(std::format("Blocked Force to Darts From {}", playerName).c_str());
					return true;
				}
				else if (activity == eActivityType::PilotSchool) {
					Menu::Notify::stacked(std::format("Blocked Force to Pilot School From {}", playerName).c_str());
					return true;
				}
				else if (activity == eActivityType::ImpromptuDeathmatch)
				{
					Menu::Notify::stacked(std::format("Blocked Force to Deathmath From {}", playerName).c_str());
					return true;
				}
				else if (activity == eActivityType::DefendSpecialCargo || activity == eActivityType::GunrunningDefend || activity == eActivityType::BikerDefend || Arguments[3] == 238)
				{
					Menu::Notify::stacked(std::format("Blocked Force to Gang Attack From {}", playerName).c_str());
					return true;
				}
			}
			else if (activity == eActivityType::Tennis) {
				Menu::Notify::stacked(std::format("Blocked S4 Crash From {}", playerName).c_str());
				return true;
			}

			if (ProtectionMenuVars::m_Vars.m_ForceToActivity.m_Block) {
				return true;
			}

			break;
		}
		case eRemoteEvent::InteriorControl:
		{
			int interior = (int)Arguments[3];
			if (interior < 0 || interior > 166) // the upper bound will change after an update
			{
				Menu::Notify::stacked(std::format("Blocked S0 Kick From {}", playerName).c_str());
				return true;
			}

			if (Native::NetworkIsActivitySession())
				break;

			return true; 
			
			break;
		}
		case eRemoteEvent::DestroyPersonalVehicle:
			return true;
		case eRemoteEvent::KickFromInterior:
			if (ProtectionMenuVars::m_Vars.m_KickFromInterior.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Interior Kick From {}", playerName).c_str());
				return true;
			}
			break;
		case eRemoteEvent::TriggerCEORaid:
		{
			return true;
			break;
		}
		}

		// detect pasted menus setting args[1] to something other than PLAYER_ID()
		if (*(int*)&Arguments[1] != fromPlayer->m_player_id && fromPlayer->m_player_id != -1)
		{
			return true;
		}

		std::string script_args = "{ ";
		for (std::size_t i = 0; i < Event->m_args_size; i++)
		{
			if (i)
				script_args += ", ";

			script_args += std::to_string((int)Arguments[i]);
		}
		script_args += " };";

		//LOG(std::format("SE | Player: {} Args{}", fromPlayer->get_name(), script_args).c_str());

		if (ProtectionMenuVars::m_Vars.m_BlockAllScriptEvents) {
			return true;
		}

		return false;
	}

	void HandleNetworkEvent(void* _this, CNetGamePlayer* fromPlayer, CNetGamePlayer* toPlayer, u16 eventID, int eventIndex, int eventBitset, int bufferSize, Rage::datBitBuffer* buffer) {

		if (eventID > 91u) {
			SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
			return;
		}

		const auto eventName = *(char**)((DWORD64)_this + 8i64 * eventID + 243376);
		if (eventName == nullptr || fromPlayer == nullptr || fromPlayer->m_player_id < 0 || fromPlayer->m_player_id >= 32) {
			SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
			return;
		}


		switch (static_cast<eNetworkEvents>(eventID)) {
		case eNetworkEvents::SCRIPTED_GAME_EVENT: {
			const auto ScriptGameEvent = std::make_unique<CScriptedGameEvent>();
			buffer->ReadDword(&ScriptGameEvent->m_args_size, 32);
			if (ScriptGameEvent->m_args_size - 1 <= 0x1AF)
				buffer->ReadArray(&ScriptGameEvent->m_args, 8 * ScriptGameEvent->m_args_size);

			if (ScriptedEventHandler(ScriptGameEvent.get(), fromPlayer)) {
				SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
				return;
			}
			buffer->Seek(0);

			break;
		}
		case eNetworkEvents::REQUEST_CONTROL_EVENT: {
			int16_t NetworkID = 0;
			NetworkID = buffer->Read<int16_t>(13);

			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_vehicle) {
					if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_vehicle->m_net_object) {
						if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_vehicle->m_net_object->m_object_id == NetworkID) {
							if (ProtectionMenuVars::m_Vars.m_Control.m_Block) {
								LOG_WARN("HNE: REQUEST_CONTROL_EVENT");
								SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
								return;
							}
						}
					}
				}
			}
			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::NETWORK_CLEAR_PED_TASKS_EVENT: {
			int16_t NetworkID = 0;
			NetworkID = buffer->Read<int16_t>(13);

			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object) {
					if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object->m_object_id == NetworkID) {
						if (ProtectionMenuVars::m_Vars.m_Freeze.m_Redirect) {
							Utils::GetFiberPool()->Push([=] {
								Native::ClearPedTasksImmediately(Native::GetPlayerPedScriptIndex(fromPlayer->m_player_id));
							});
						}
						if (ProtectionMenuVars::m_Vars.m_Freeze.m_Block) {
							Menu::Notify::stacked(std::format("Blocked Freeze From {}", fromPlayer->get_name()).c_str());
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
						if (ProtectionMenuVars::m_Vars.m_Freeze.m_Notify) {
							Menu::Notify::stacked(std::format("Blocked Freeze From {}", fromPlayer->get_name()).c_str());
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
					}
				}
			}

			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::RAGDOLL_REQUEST_EVENT: {
			int16_t NetworkID = 0;
			NetworkID = buffer->Read<int16_t>(13);

			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object) {
					if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object->m_object_id == NetworkID) {
						if (ProtectionMenuVars::m_Vars.m_Ragdoll.m_Block) {
							Menu::Notify::stacked(std::format("Blocked Ragdoll From {}", fromPlayer->get_name()).c_str());
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
						if (ProtectionMenuVars::m_Vars.m_Ragdoll.m_Notify) {
							Menu::Notify::stacked(std::format("Blocked Ragdoll From {}", fromPlayer->get_name()).c_str());
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
					}
				}
			}

			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::REMOVE_WEAPON_EVENT: {
			int16_t NetworkID = 0;
			NetworkID = buffer->Read<int16_t>(13);

			Hash WeaponHash = 0;
			WeaponHash = buffer->Read<Hash>(32);

			if (WeaponHash == RAGE_JOAAT("WEAPON_UNARMED")) {
				Menu::Notify::stacked(std::format("Blocked Invalid Weapon Id From {}", fromPlayer->get_name()).c_str());
				SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
				return;
			}

			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object) {
					if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object->m_object_id == NetworkID) {
						if (ProtectionMenuVars::m_Vars.m_RemoveWeapon.m_Redirect) {
							Utils::GetFiberPool()->Push([=] {
								Native::RemoveWeaponFromPed(Native::GetPlayerPedScriptIndex(fromPlayer->m_player_id),  WeaponHash);
							});
						}
						if (ProtectionMenuVars::m_Vars.m_RemoveWeapon.m_Block) {
							Menu::Notify::stacked(std::format("Blocked Remove Weapon From {}", fromPlayer->get_name()).c_str());
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
						if (ProtectionMenuVars::m_Vars.m_RemoveWeapon.m_Notify) {
							Menu::Notify::stacked(std::format("Blocked Remove Weapon From {}", fromPlayer->get_name()).c_str());
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
					}
				}
			}

			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::REMOVE_ALL_WEAPONS_EVENT: {
			int16_t NetworkID = 0;
			NetworkID = buffer->Read<int16_t>(13);

			Hash WeaponHash = 0;
			WeaponHash = buffer->Read<Hash>(32);

			if (WeaponHash == RAGE_JOAAT("WEAPON_UNARMED")) {
				Menu::Notify::stacked(std::format("Blocked Invalid Weapon Id From {}", fromPlayer->get_name()).c_str());
				SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
				return;
			}

			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object) {
					if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object->m_object_id == NetworkID) {
						if (ProtectionMenuVars::m_Vars.m_RemoveAllWeapons.m_Redirect) {
							Utils::GetFiberPool()->Push([=] {
								Native::RemoveAllPedWeapons(Native::GetPlayerPedScriptIndex(fromPlayer->m_player_id), true);
								});
						}
						if (ProtectionMenuVars::m_Vars.m_RemoveAllWeapons.m_Block) {
							Menu::Notify::stacked(std::format("Blocked Remove All Weapon From {}", fromPlayer->get_name()).c_str());
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
						if (ProtectionMenuVars::m_Vars.m_RemoveAllWeapons.m_Notify) {
							Menu::Notify::stacked(std::format("Blocked Remove All Weapon From {}", fromPlayer->get_name()).c_str());
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
					}
				}
			}

			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::GIVE_WEAPON_EVENT: {
			int16_t NetworkID = 0;
			NetworkID = buffer->Read<int16_t>(13);

			Hash WeaponHash = 0;
			WeaponHash = buffer->Read<Hash>(32);

			int AmmoCount = -1;
			AmmoCount = buffer->Read<int>(32);

			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object) {
					if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object->m_object_id == NetworkID) {
						if (ProtectionMenuVars::m_Vars.m_GiveWeapon.m_Redirect) {
							Utils::GetFiberPool()->Push([=] {					
								Native::GiveDelayedWeaponToPed(Native::GetPlayerPedScriptIndex(fromPlayer->m_player_id), WeaponHash, 0, true);
								Native::SetPedAmmo(Native::GetPlayerPedScriptIndex(fromPlayer->m_player_id), WeaponHash, AmmoCount, 0);
								});
						}
						if (ProtectionMenuVars::m_Vars.m_GiveWeapon.m_Block) {
							Menu::Notify::stacked(std::format("Blocked Give Weapon From {}", fromPlayer->get_name()).c_str());
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
						if (ProtectionMenuVars::m_Vars.m_GiveWeapon.m_Notify) {
							Menu::Notify::stacked(std::format("Blocked Give Weapon From {}", fromPlayer->get_name()).c_str());
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
					}
				}
			}

			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::FIRE_EVENT: {
			int16_t NetworkID = 0;
			NetworkID = buffer->Read<int16_t>(13);

			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object) {
					if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object->m_object_id == NetworkID) {
						if (ProtectionMenuVars::m_Vars.m_Fire.m_Block) {
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
					}
				}
			}

			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::NETWORK_PLAY_SOUND_EVENT: {
			int16_t NetworkID = 0;
			NetworkID = buffer->Read<int16_t>(13);

			Rage::netObject* Object = Caller::Call<Rage::netObject*>(Patterns::Vars::g_GetNetworkObjectFromNetworkID, *(uint64_t*)Patterns::Vars::g_NetworkObjectMgr, NetworkID, true);
			if (!Object) {
				SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
				return;
			}

			if (ProtectionMenuVars::m_Vars.m_Sound.m_Block) {
				SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
				return;
			}

			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::EXPLOSION_EVENT: {
			int16_t NetworkID = 0;
			NetworkID = buffer->Read<int16_t>(13);

			if (ProtectionMenuVars::m_Vars.m_Explosion.m_Block) {
				Menu::Notify::stacked(std::format("Blocked Explosion From {}", fromPlayer->get_name()).c_str());
				SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
				return;
			}
			if (ProtectionMenuVars::m_Vars.m_Explosion.m_Notify) {
				Menu::Notify::stacked(std::format("Blocked Explosion From {}", fromPlayer->get_name()).c_str());
				SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
				return;
			}

			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::NETWORK_PTFX_EVENT: {
			uint32_t Name = 0;
			uint32_t Dict = 0;
			uint32_t Scale = 0;
			Math::Vector3_<float> XYZ;
			Math::Vector3_<float> ROT;

			//auto Instance = buffer->CreateSyncReaderInstance();
			Name = buffer->Read<uint32_t>(32);
			Dict = buffer->Read<uint32_t>(32);
			Scale = buffer->Read<uint32_t>(10);

			float FixedScale = ((((float)Scale) * 0.00097751711f) * 10.0f);
			
			int16_t NetworkID = 0;
			NetworkID = buffer->Read<int16_t>(13);


			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object) {
					if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object->m_object_id == NetworkID) {
						if (ProtectionMenuVars::m_Vars.m_PTFX.m_Block) {
							Menu::Notify::stacked(std::format("Blocked PTFX From {}", fromPlayer->get_name()).c_str());
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
						if (ProtectionMenuVars::m_Vars.m_PTFX.m_Notify) {
							Menu::Notify::stacked(std::format("Blocked PTFX From {}", fromPlayer->get_name()).c_str());
							SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
							return;
						}
					}
				}
			}

			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::SCRIPT_ENTITY_STATE_CHANGE_EVENT:
		{
			uint16_t entity = buffer->Read<uint16_t>(13);
			auto type = buffer->Read<ScriptEntityChangeType>(4);
			uint32_t unk = buffer->Read<uint32_t>(32);
			if (type == ScriptEntityChangeType::SettingOfTaskVehicleTempAction)
			{
				uint16_t ped_id = buffer->Read<uint16_t>(13);
				uint32_t action = buffer->Read<uint32_t>(8);

				if ((action >= 15 && action <= 18) || action == 33)
				{
					SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
					Menu::Notify::stacked(std::format("Blocked Invalid Action From {}", fromPlayer->get_name()).c_str());
					return;
				}
			}
			else if (type > ScriptEntityChangeType::SetVehicleExclusiveDriver || type < ScriptEntityChangeType::BlockingOfNonTemporaryEvents)
			{
				Menu::Notify::stacked(std::format("Blocked Invalid Entity Change From {}", fromPlayer->get_name()).c_str());
				SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
				return;
			}
			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::SCRIPT_WORLD_STATE_EVENT:
		{
			auto type = buffer->Read<WorldStateDataType>(4);
			buffer->Read<bool>(1);
			CGameScriptId id;
			GetScriptID(id, *buffer);

			if (type == WorldStateDataType::Rope)
			{
				buffer->Read<int>(9);    // network rope id
				buffer->Read<float>(19); // pos x
				buffer->Read<float>(19); // pos y
				buffer->Read<float>(19); // pos z
				buffer->Read<float>(19); // rot x
				buffer->Read<float>(19); // rot y
				buffer->Read<float>(19); // rot z
				buffer->Read<float>(16); // length
				int type = buffer->Read<int>(4);
				float initial_length = buffer->Read<float>(16);
				float min_length = buffer->Read<float>(16);

				if (type == 0 || initial_length < min_length) // https://docs.fivem.net/natives/?_0xE832D760399EB220
				{
					Menu::Notify::stacked(std::format("Blocked Invalid Rope Length From {}", fromPlayer->get_name()).c_str());
					SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
					return;
				}
			}
			else if (type == WorldStateDataType::PopGroupOverride)
			{
				int pop_schedule = buffer->ReadSigned<int>(8); // Pop Schedule
				int pop_group = buffer->Read<int>(32);      // Pop Group
				int percentage = buffer->Read<int>(7);       // Percentage

				if (pop_group == 0 && (percentage == 0 || percentage == 103))
				{
					Menu::Notify::stacked(std::format("Blocked Invalid Group Change From {}", fromPlayer->get_name()).c_str());
					SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
					return;
				}
			}
			else if (type > WorldStateDataType::VehiclePlayerLocking || type < WorldStateDataType::CarGen)
			{
				Menu::Notify::stacked(std::format("Blocked Invalid State Change From {}", fromPlayer->get_name()).c_str());
				SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
				return;
			}
			else if (type == WorldStateDataType::PopMultiplierArea && !Native::NetworkIsActivitySession()) {
				//Menu::Notify::stacked(std::format("Blocked Invalid State Change From {}", fromPlayer->get_name()).c_str());
				LOG_WARN("Bad Traffic Id");
				SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
				return;
			}

			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::GIVE_CONTROL_EVENT:
		{
			uint32_t timestamp = buffer->Read<uint32_t>(32);
			int count = buffer->Read<int>(2);
			bool all_objects_migrate_together = buffer->Read<bool>(1);
			eNetObjType sync_type;

			if (count > 3)
			{
				count = 3;
			}

			for (int i = 0; i < count; i++)
			{
				int net_id = buffer->Read<int>(13);
				eNetObjType object_type = buffer->Read<eNetObjType>(4);
				int migration_type = buffer->Read<int>(3);

				if (object_type < eNetObjType::NET_OBJ_TYPE_AUTOMOBILE || object_type > eNetObjType::NET_OBJ_TYPE_TRAIN)
				{
					Menu::Notify::stacked(std::format("Blocked Invalid Sync Type From {}", fromPlayer->get_name()).c_str());
					SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
					return;
				}

				sync_type = object_type;
			}

			buffer->Seek(0);

			if (count)
			{
				g_SyncingPlayer = fromPlayer;
				g_SyncingObjectType = sync_type;
			}
			break;
		}
		case eNetworkEvents::WEAPON_DAMAGE_EVENT: {
			u32 weaponType = buffer->Read<uint32_t>(32);
			if (!IsValidWeapon(weaponType)) {
				Menu::Notify::stacked(std::format("Blocked Crash S7 From {}", fromPlayer->get_name()).c_str());
				SendEventAck(_this, fromPlayer, toPlayer, eventIndex, eventBitset);
				return;
			}

			buffer->Seek(0);
			break;
		}
		case eNetworkEvents::RESPAWN_PLAYER_PED_EVENT: {
			Menu::Notify::stacked(std::format("{} just spawned", fromPlayer->get_name()).c_str());
			break;
		}
		}

		return OgHandleNetworkEvent(_this, fromPlayer, toPlayer, eventID, eventIndex, eventBitset, bufferSize, buffer);
	}
}