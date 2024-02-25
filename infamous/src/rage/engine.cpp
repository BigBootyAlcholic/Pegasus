#include "stdafx.hpp"
#include "engine.hpp"
#include "cheat/menu/submenus/network.hpp"
#include "util/fiber.hpp"
#include "core/hooks.hpp"
#include "util/fiber_pool.hpp"
#include "cheat/util/global.hpp"
#include "rage/classes/network/Network.hpp"
#include "util/curl_wrapper.hpp"
#include "cheat/util/notify.hpp"
#include "rage/classes/rage/rlSessionByGamerTaskResult.hpp"
namespace Engine {
	CNetGamePlayer*	GetNetworkGamePlayer(u32 Player) {
		return Caller::Call<CNetGamePlayer*>(Patterns::Vars::g_GetNetworkGamePlayer, Player);
	}
	uint64_t GetEntityAddress(::Entity Entity) {
		return Caller::Call<uint64_t>(Patterns::Vars::g_GetEntityAddress, Entity);
	}

	Entity GetEntityHandleFromAddress(uint64_t Address) {
		return Caller::Call<Entity>(Patterns::Vars::g_GetEntityHandleFromAddress, Address, 0);
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

	void JoinSessionTmp(int ID) {
		Utils::GetFiberPool()->Push([=] {
			if (ID == -1) {
				Menu::Global(1574589 + 2).As<int>() = ID;
			}
			else {
				Menu::Global(1575032).As<int>() = ID;
			}

			Menu::Global(1574589).As<int>() = 1;
			Utils::WaitFiber(200);
			Menu::Global(1574589).As<int>() = 0;
			});
	}


	void JoinSession(const Rage::rlSessionInfo& Info) {
		if (Native::_GetNumberOfInstancesOfScriptWithNameHash(RAGE_JOAAT("maintransition")) != 0 || Native::IsPlayerSwitchInProgress()) {
			LOG_ERROR("ERR SWITCH");
			return;
		}

		NetworkMenuVars::m_Vars.m_JoinQueue = true;
		NetworkMenuVars::m_Vars.m_SessionInfo = Info;
		JoinSessionTmp(1);

		if (Native::_GetNumberOfInstancesOfScriptWithNameHash(RAGE_JOAAT("maintransition")) == 0)
		{
			NetworkMenuVars::m_Vars.m_JoinQueue = true;
			LOG_ERROR("ERR SWITCH2");
		}
		return;
	}

	std::string gsTypeToString(int gstype) {
		switch (gstype) {
		case 0: {
			return "Invite-Only";
		} break;
		case 1: {
			return "Friends-Only";
		} break;
		case 2: {
			return "Crew-Only";
		} break;
		case 3: {
			return "Crew";
		} break;
		case 4: {
			return "Solo";
		} break;
		case 5: {
			return "Public";
		} break;
		}
		return "Offline";
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

	void JoinSessionByRid(u64 rid) {
		Utils::GetFiberPool()->Push([=] {
			if (Native::_GetNumberOfInstancesOfScriptWithNameHash(RAGE_JOAAT("maintransition")) != 0 || Native::IsPlayerSwitchInProgress()) {
				LOG_ERROR("ERR SWITCH");
				return;
			}

			Rage::rlGamerHandle player_handle(rid);
			Rage::rlSessionByGamerTaskResult result;
			bool success = false;
			Rage::rlTaskStatus state{};

			if (Caller::Call<bool>(Patterns::Vars::g_StartSessionByGamer, 0, &player_handle, 1, &result, 1, &success, &state))
			{
				while (state.status == 1)
					Utils::GetFiberManager()->GoToMainFiber();

				if (state.status == 3 && success)
				{
					JoinSession(result.m_session_info);
					return;
				}
			}

			Menu::Notify::stacked("Player Is Offline");
		});
		

		/*Utils::GetFiberPool()->Push([=] {
			if (Native::_GetCurrentFrontendMenu() != 0xFFFFFFFF) {
				Native::ActivateFrontendMenu(Joaat("FE_MENU_VERSION_SP_PAUSE"), false, 2);
				Utils::GetFiberManager()->Sleep(200);
			}
			Native::ActivateFrontendMenu(Joaat("FE_MENU_VERSION_SP_PAUSE"), false, 2);
			Utils::GetFiberManager()->Sleep(200);

			CPlayerListMenu* Menu = new CPlayerListMenu();
			u32 Hash{ 0xDA4858C1 };
			u64 Info{ (u64)Caller::Call<u64>(Patterns::Vars::g_GetFriendsMenu, 0) };
			u8* Data{ reinterpret_cast<u8*>(Info + 0x8) };

			if (Data) {
				u8 Idx{};
				while (*Data <= 3u) {
					if (*Data == 3) {
						break;
					}
					++Idx;
					Data += 0x10;
				}
				if (Idx < 20ui8) {
					u64 OriginalRID{ *(u64*)(Info + 16ui64 * Idx) };
					*(u64*)(Info + 16ui64 * Idx) = rid;
					Caller::Call<bool>(Patterns::Vars::g_ConstructPlayerMenu, Menu, &Hash);
					Utils::GetFiberManager()->Sleep(400);
					*(u64*)(Info + 16ui64 * Idx) = OriginalRID;
				}
			}
		});*/


	}
	
	namespace Api {
		nlohmann::json Request(nlohmann::json body, std::string endpoint) {
			std::string ticket = (*Patterns::Vars::g_ScInfo).m_ticket;
			CurlWrapper curl{};
			std::string jsonBody{ body.dump() };
			std::string authorizationHeader{ "Authorization: SCAUTH val=\"" + ticket + "\"" };
			std::vector<std::string> headers{
				"X-Requested-With: XMLHttpRequest",
				"Content-Type: application/json; charset=utf-8",
				authorizationHeader
			};
			std::string response{ curl.Post(endpoint, jsonBody, headers) };
			if (response != "bad_res") {
				return nlohmann::json::parse(response);
			}
			return {};
		}
		bool IsOnline(u64 rid) {
			if (nlohmann::json json{ Request({ { "RockstarId", std::to_string(rid) } }, "https://scui.rockstargames.com/api/friend/getprofile") }; !json.is_null()) {
				for (auto& acc : json["Accounts"]) {
					if (auto& r_acc = acc["RockstarAccount"]; !r_acc.is_null()) {
						if (rid == r_acc["RockstarId"].get<u64>()) {
							return r_acc["IsOnline"].get<bool>();
						}
					}
				}
			}
			return false;
		}
		u64 NameToRid(std::string name) {
			std::string endpoint = "https://scui.rockstargames.com/api/friend/accountsearch";
			nlohmann::json body = { { "searchNickname", name } };
			nlohmann::json json = Request(body, endpoint);
			if (!json.is_null()) {
				if (name.size() <= 20) {
					int numAccs = json["Total"].get<int>();
					if (numAccs > 0) {
						return json["Accounts"][0]["RockstarId"].get<u64>();
					}
					else {
						Menu::Notify::stacked(std::format("Cant Find {}", name).c_str());
					}
				}
				else {
					Menu::Notify::stacked("The character count cannot exceed 20. Please shorten the value.");
				}
			}
			return 0;
		}
		std::string RidToName(u64 rid) {
			if (nlohmann::json json{ Request({ { "RockstarId", std::to_string(rid) } }, "https://scui.rockstargames.com/api/friend/getprofile") }; !json.is_null()) {
				for (auto& acc : json["Accounts"]) {
					if (auto& r_acc = acc["RockstarAccount"]; !r_acc.is_null()) {
						if (rid == r_acc["RockstarId"].get<u64>()) {
							return r_acc["Name"].get<std::string>();
						}
					}
				}
			}
			return {};
		}
	}

	void InvitePlayer(uint64_t rid) {
		NetMsgIdentifier identifier;
		memset(&identifier, 0, sizeof(identifier));

		identifier.m_RockstarID = rid;
		identifier.m_Type = 3;

		Caller::Call<void>(Patterns::Vars::g_InvitePlayer, Patterns::Vars::g_NetworkBaseConfig, &identifier, 1, "Invite", "join me daddy", 0);
	}
}