#include "stdafx.hpp"
#include "network.hpp"
#include "network/network_players.hpp"
#include "network/network_tunables.hpp"
#include "cheat/util/network_manager.hpp"
#include "cheat/util/timer.hpp"
#include "rage/engine.hpp"
#include "util/fiber_pool.hpp"
#include "rage/classes/network/Network.hpp"
#include "network/network_spoofing.hpp"
#include "cheat/util/global.hpp"
#include "cheat/util/shop_manager.hpp"
#include "network/network_player_history.hpp"
#include "network/network_browser.hpp"
#include "network/network_friends.hpp"
using namespace NetworkMenuVars;

namespace NetworkMenuVars {
	Vars_ m_Vars;

	struct RegionType {
		uint32_t id;
		const char name[22];
	};

	inline const RegionType regions[] = {
		{0, "CIS"},
		{1, "Africa"},
		{2, "East"},
		{3, "Europe"},
		{4, "China"},
		{5, "Australia"},
		{6, "West"},
		{7, "Japan"},
		{8, "Unknown"},
	};

	void TransitionToNewLobby(int ID) {
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

	void RequestAmmoDrop() {
		Menu::Global(2738587).At(891).As<int>() = 1;
	}

	inline void RequestBoatPickup() {
		Menu::Global(2738587).At(892).As<int>() = 1;
	}

	inline void RequestHeliPickup() {
		Menu::Global(2738587).At(893).As<int>() = 1;
	}

	inline void RequestBackupHeli() {
		Menu::Global(2738587).At(4491).As<int>() = 1;
	}

	inline void RequestAirstrike() {
		Menu::Global(2738587).At(4492).As<int>() = 1;
	}

	inline void RequestBst() {
		Menu::Global(2738587).At(3694).As<int>() = 1;
	}

	inline void RequestBallisticArmor() {
		Menu::Global(2738587).At(896).As<int>() = 1;
	}

	inline void RequestAvenger() {
		Menu::Global(2738587).At(938).As<int>() = 1;
	}
	inline void RequestKosatka() {
		Menu::Global(2738587).At(960).As<int>() = 1;
	}
	inline void RequestOperationCenter() {
		Menu::Global(2738587).At(930).As<int>() = 1;
	}
	inline void RequestTerrorbyte() {
		Menu::Global(2738587).At(943).As<int>() = 1;
	}
	inline void RequestAcidLab() {
		Menu::Global(2738587).At(944).As<int>() = 1;
	}
	inline void RequestAcidLabBike() {
		Menu::Global(2738587).At(994).As<int>() = 1;
	}
}

bool m_Spam;
int delay = 300;
int timer = 0;
const char* m_Message = "Big Nigga Dick";
bool m_mocking;

static bool active = false;
inline static void SendChatMessage() {
	Utils::GetFiberPool()->Push([] {
		Native::DisplayOnscreenKeyboard(0, "Input", "", "", "", "", "", 100);
		while (Native::UpdateOnscreenKeyboard() == 0) {
			active = true;
			Framework::GetFrameWork()->ResetKeys();
			Utils::GetFiberManager()->GoToMainFiber();
		}
		active = false;
		if (!Native::GetOnscreenKeyboardResult())
			return;
		m_Message = Native::GetOnscreenKeyboardResult();
	});	
}

std::string m_RidToJoin;
std::string m_NameToJoin;

inline static void JoinByRid() {
	Utils::GetFiberPool()->Push([] {
		Native::DisplayOnscreenKeyboard(0, "Input", "", "", "", "", "", 100);
		while (Native::UpdateOnscreenKeyboard() == 0) {
			active = true;
			Framework::GetFrameWork()->ResetKeys();
			Utils::GetFiberManager()->GoToMainFiber();
		}
		active = false;
		if (!Native::GetOnscreenKeyboardResult())
			return;
		m_RidToJoin = Native::GetOnscreenKeyboardResult();
		Engine::JoinSessionByRid(std::atoi(m_RidToJoin.c_str()));
	});
}

inline static void JoinByGs() {
	Utils::GetFiberPool()->Push([] {
		Native::DisplayOnscreenKeyboard(0, "Input", "", "", "", "", "", 100);
		while (Native::UpdateOnscreenKeyboard() == 0) {
			active = true;
			Framework::GetFrameWork()->ResetKeys();
			Utils::GetFiberManager()->GoToMainFiber();
		}
		active = false;
		if (!Native::GetOnscreenKeyboardResult())
			return;
		m_NameToJoin = Native::GetOnscreenKeyboardResult();
		Engine::JoinSessionByRid(Engine::Api::NameToRid(m_NameToJoin));
	});
}

void NetworkMenu::Run() {
	Framework::addSubmenu("Network", "network-menu", [](Framework::Options::Core* core) {
		g_EnablePlayerInfo = false;
		core->addOption(Framework::Options::SubmenuOption("Players")
			.setTarget("network_players"));

		core->addOption(Framework::Options::SubmenuOption("Session")
			.setTarget("network-session"));

		core->addOption(Framework::Options::SubmenuOption("Spoofing")
			.setTarget("network-spoofing"));

		core->addOption(Framework::Options::SubmenuOption("Global Chat")
			.setTarget("network-chat"));

		core->addOption(Framework::Options::SubmenuOption("Tunables")
			.setTarget("network_tunables"));

		core->addOption(Framework::Options::SubmenuOption("Requests")
			.setTarget("network_requests"));

		core->addOption(Framework::Options::SubmenuOption("Friends")
			.setTarget("network_friends"));

		core->addOption(Framework::Options::SubmenuOption("Rockstar ID Tools")
			.setTarget("network-ridtools"));

		core->addOption(Framework::Options::SubmenuOption("Player History")
			.setTarget("network_player_history"));

		core->addOption(Framework::Options::SubmenuOption("Session Browser")
			.setTarget("network_session_browser"));

		core->addOption(Framework::Options::SubmenuOption("Join Bypass")
			.setTarget("network_join_bypass"));

		core->addOption(Framework::Options::SubmenuOption("New Session")
			.setTarget("network-new-session"));
	});

	Framework::addSubmenu("Join Bypass", "network_join_bypass", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Invite Only")
			.addToggle(&m_Vars.m_InviteOnly));

		core->addOption(Framework::Options::ToggleOption("Friend Only")
			.addToggle(&m_Vars.m_FriendOnly));

		core->addOption(Framework::Options::ToggleOption("Crew Only")
			.addToggle(&m_Vars.m_CrewOnly));
	});

	Framework::addSubmenu("Requests", "network_requests", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ButtonOption("Request Ammo Drop")
			.addClick([] { RequestAmmoDrop(); }));

		core->addOption(Framework::Options::ButtonOption("Request Boat Pickup")
			.addClick([] { RequestBoatPickup(); }));

		core->addOption(Framework::Options::ButtonOption("Request Helicopter Pickup")
			.addClick([] { RequestHeliPickup(); }));

		core->addOption(Framework::Options::ButtonOption("Request Backup Helicopter")
			.addClick([] { RequestBackupHeli(); }));

		core->addOption(Framework::Options::ButtonOption("Request Airstrike")
			.addClick([] { RequestAirstrike(); }));

		core->addOption(Framework::Options::ButtonOption("Request Bullshark Testosterone")
			.addClick([] { RequestBst(); }));

		core->addOption(Framework::Options::ButtonOption("Request Ballistic Armor")
			.addClick([] { RequestBallisticArmor(); }));

		core->addOption(Framework::Options::ButtonOption("Request Avenger")
			.addClick([] { RequestAvenger(); }));

		core->addOption(Framework::Options::ButtonOption("Request Kosatka")
			.addClick([] { RequestKosatka(); }));

		core->addOption(Framework::Options::ButtonOption("Request Moble Operations Center")
			.addClick([] { RequestOperationCenter(); }));

		core->addOption(Framework::Options::ButtonOption("Request Terrorbyte")
			.addClick([] { RequestTerrorbyte(); }));

		core->addOption(Framework::Options::ButtonOption("Request Acid Lab")
			.addClick([] { RequestAcidLab(); }));

		core->addOption(Framework::Options::ButtonOption("Request Acid Lab Bike")
			.addClick([] { RequestAcidLabBike(); }));
	});

	Framework::addSubmenu("Rockstar ID Tools", "network-ridtools", [](Framework::Options::Core* core) {
		std::string to_string2 = std::format("Join Session By Name: {}", m_NameToJoin);

		core->addOption(Framework::Options::ButtonOption(to_string2.c_str())
			.addClick([] {
				JoinByGs();
			}));

		std::string to_string = std::format("Join Session By Rid: {}", m_RidToJoin);

		core->addOption(Framework::Options::ButtonOption(to_string.c_str())
			.addClick([] {
				JoinByRid();
			}));

		core->addOption(Framework::Options::ButtonOption("Invite To Session")
			.addClick([] {
				//Engine::InvitePlayer();
			}));
	});

	Framework::addSubmenu("New Session", "network-new-session", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ButtonOption("Create Public Session")
			.addClick([] { TransitionToNewLobby(1); }));

		core->addOption(Framework::Options::ButtonOption("Create Closed Crew Session")
			.addClick([] { TransitionToNewLobby(2); }));

		core->addOption(Framework::Options::ButtonOption("Create Crew Session")
			.addClick([] { TransitionToNewLobby(3); }));

		core->addOption(Framework::Options::ButtonOption("Create Closed Friend Session")
			.addClick([] { TransitionToNewLobby(6); }));

		core->addOption(Framework::Options::ButtonOption("Create Solo Session")
			.addClick([] { TransitionToNewLobby(10); }));

		core->addOption(Framework::Options::ButtonOption("Create Invite Only Session")
			.addClick([] { TransitionToNewLobby(11); }));
		
		core->addOption(Framework::Options::ButtonOption("Join Public Session")
			.addClick([] { TransitionToNewLobby(0); }));

		core->addOption(Framework::Options::ButtonOption("Join Crew Session")
			.addClick([] { TransitionToNewLobby(12); }));

		core->addOption(Framework::Options::ButtonOption("Join Friend Session")
			.addClick([] { TransitionToNewLobby(9); }));

		core->addOption(Framework::Options::ButtonOption("Leave Online")
			.addClick([] { TransitionToNewLobby(-1); }));
	});

	Framework::addSubmenu("Global Chat", "network-chat", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Commands")
			.setTarget("chat-commands").addTooltip("Process Chat Messages With The Chosen Prefix"));

		core->addOption(Framework::Options::SubmenuOption("Reactions")
			.setTarget("chat-reactions"));

		core->addOption(Framework::Options::SubmenuOption("Log Chat Messages")
			.setTarget("chat-log"));

		core->addOption(Framework::Options::SubmenuOption("Send Message")
			.setTarget("chat-send"));

		core->addOption(Framework::Options::SubmenuOption("Spam")
			.setTarget("chat-spam"));

		core->addOption(Framework::Options::ToggleOption("Mocking")
			.addToggle(&m_mocking).addTooltip("Mock what everyone says in chat.\nExample: \"hey everyone\" becomes \"hEy EvERyOnE\""));

		core->addOption(Framework::Options::ToggleOption("Disable Profanity Filter")
			.addToggle(&m_Vars.m_DisableProfanityFilter));
	});

	Framework::addSubmenu("Send Message", "chat-send", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::KeyboardOption("Enter Message")
			.addRightText(m_Message)
			.addClick([] { SendChatMessage(); }));

		static bool team = false;

		core->addOption(Framework::Options::ToggleOption("Team Chat")
			.addToggle(&team));


		std::string to_string = std::format("Send Message: {}", m_Message);

		core->addOption(Framework::Options::ButtonOption(to_string.c_str())
			.addClick([] {
				Engine::SendChatMessage(m_Message);
			}));
	});

	Framework::addSubmenu("Spam", "chat-spam", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::KeyboardOption("Enter Message")
			.addRightText(m_Message)
			.addClick([] { SendChatMessage(); }));

		static bool team = false;

		core->addOption(Framework::Options::ToggleOption("Team Chat")
			.addToggle(&team));

		core->addOption(Framework::Options::NumberOption<int>("Interval (ms)")
			.addNumber(&delay)
			.addMin(0).addMax(300).addStep(1));

		std::string to_string = std::format("Spam Message: {}", m_Message);

		core->addOption(Framework::Options::ToggleOption(to_string.c_str())
			.addToggle(&m_Spam));
	});

	Framework::addSubmenu("Session", "network-session", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Session Scripts")
			.setTarget("toolkit-script-scripts"));

		core->addOption(Framework::Options::SubmenuOption("Script Migration")
			.setTarget("toolkit-script-migration"));

		core->addOption(Framework::Options::SubmenuOption("Matchmaking")
			.setTarget("toolkit-matchmaking"));

		core->addOption(Framework::Options::SubmenuOption("Speed Up Transition")
			.setTarget("toolkit-transition"));

		core->addOption(Framework::Options::SubmenuOption("Desync Kicks")
			.setTarget("toolkit-desynckicks"));

		core->addOption(Framework::Options::SubmenuOption("Set Session Type")
			.setTarget("toolkit-sessiontype"));

		core->addOption(Framework::Options::ToggleOption("Open Extra Slot")
			.addToggle(&m_Vars.m_OpenExtraSlot));

		core->addOption(Framework::Options::ButtonOption("Rejoin Session")
			.addClick([] { Engine::JoinSession((*Patterns::Vars::g_Network)->m_last_joined_session.m_session_info); }));

		core->addOption(Framework::Options::ButtonOption("Be Alone")
			.addClick([] { 
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					(*Patterns::Vars::g_NetworkPlayerMgr)->RemovePlayer(Player.m_NetGamePlayer);
				});
			}));
	
	});

	Framework::addSubmenu("Session Scripts", "toolkit-script-scripts", [](Framework::Options::Core* core) {

		core->addOption(Framework::Options::ButtonOption("Business Battle")
			.addClick([] { Menu::Network::StartLauncherScript(114); }));

		core->addOption(Framework::Options::ButtonOption("Challenges")
			.addClick([] { Menu::Network::StartLauncherScript(40); }));

		core->addOption(Framework::Options::ButtonOption("Checkpoints")
			.addClick([] { Menu::Network::StartLauncherScript(39); }));

		core->addOption(Framework::Options::ButtonOption("Hunt The Beast")
			.addClick([] { Menu::Network::StartLauncherScript(47); }));

		core->addOption(Framework::Options::ButtonOption("Hot Property")
			.addClick([] { Menu::Network::StartLauncherScript(43); }));

		core->addOption(Framework::Options::ButtonOption("Hot Target")
			.addClick([] { Menu::Network::StartLauncherScript(36); }));

		core->addOption(Framework::Options::ButtonOption("Kill List")
			.addClick([] { Menu::Network::StartLauncherScript(37); }));

		core->addOption(Framework::Options::ButtonOption("King of The Castle")
			.addClick([] { Menu::Network::StartLauncherScript(45); }));

		core->addOption(Framework::Options::ButtonOption("Criminal Damge")
			.addClick([] { Menu::Network::StartLauncherScript(46); }));

		core->addOption(Framework::Options::ButtonOption("Penned In")
			.addClick([] { Menu::Network::StartLauncherScript(41); }));

		core->addOption(Framework::Options::ButtonOption("Space Monkey")
			.addClick([] {Menu::Network::StartLauncherScript(223); }));

		core->addOption(Framework::Options::ButtonOption("Gunslinger")
			.addClick([] { Menu::Network::StartLauncherScript(218); }));

		core->addOption(Framework::Options::ButtonOption("Wizard")
			.addClick([] { Menu::Network::StartLauncherScript(219); }));

		core->addOption(Framework::Options::ButtonOption("Camhedz")
			.addClick([] { Menu::Network::StartLauncherScript(225); }));

		core->addOption(Framework::Options::ButtonOption("QUB3D")
			.addClick([] { Menu::Network::StartLauncherScript(224); }));

		core->addOption(Framework::Options::ButtonOption("Flight School")
			.addClick([] { Menu::Network::StartLauncherScript(203); }));

		core->addOption(Framework::Options::ButtonOption("Darts")
			.addClick([] { Menu::Network::StartLauncherScript(14); }));

		core->addOption(Framework::Options::ButtonOption("Golf")
			.addClick([] { Menu::Network::StartLauncherScript(200); }));

		core->addOption(Framework::Options::ButtonOption("Impromptu Race")
			.addClick([] { Menu::Network::StartLauncherScript(16); }));

		core->addOption(Framework::Options::ButtonOption("Impromptu Deathmatch")
			.addClick([] { Menu::Network::StartLauncherScript(5); }));

		core->addOption(Framework::Options::ButtonOption("One on one Deathmatch")
			.addClick([] {Menu::Network::StartLauncherScript(204); }));

		core->addOption(Framework::Options::ButtonOption("Tutorial")
			.addClick([] { Menu::Network::StartLauncherScript(20); }));
	});

	Framework::addSubmenu("Set Session Type", "toolkit-sessiontype", [](Framework::Options::Core* core) {

		static const char* gstypes[] = {
			"Invite-Only",
			"Friends-Only",
			"Crew-Only",
			"Crew",
			"Solo",
			"Public"
		};

		core->addOption(Framework::Options::scrollOption<const char*, int>("Session Type")
			.addScroll(&gstypes).setPosition(&m_Vars.m_SessionType));

		core->addOption(Framework::Options::ToggleOption("Toggle")
			.addToggle(&m_Vars.m_ToggleSessionType));
	});

	Framework::addSubmenu("Desync Kicks", "toolkit-desynckicks", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Rejoin Desynced Sessions")
			.addToggle(&m_Vars.m_RejoinDesyncedSessions));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_NotifyDesyncKicks));
	});

	Framework::addSubmenu("Speed Up", "toolkit-transition", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Toggle")
			.addToggle(&m_Vars.m_SpeedUp));

		core->addOption(Framework::Options::ToggleOption("Display Current State")
			.addToggle(&m_Vars.m_LogState));
	});

	Framework::addSubmenu("Matchmaking", "toolkit-matchmaking", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Region Override")
			.setTarget("matchmaking-region"));

		core->addOption(Framework::Options::SubmenuOption("Asset Override")
			.setTarget("matchmaking-assets"));

		core->addOption(Framework::Options::SubmenuOption("Pool Override")
			.setTarget("matchmaking-pool"));

		static bool magnet;
		static bool disable;

		core->addOption(Framework::Options::ToggleOption("Player Magnet")
			.addToggle(&magnet));

		core->addOption(Framework::Options::ToggleOption("Disable")
			.addToggle(&disable));
	});

	Framework::addSubmenu("Asset Override", "matchmaking-assets", [](Framework::Options::Core* core) {
		static bool overrideAssets;

		core->addOption(Framework::Options::ToggleOption("Override Game Assets")
			.addToggle(&overrideAssets));

		core->addOption(Framework::Options::ButtonOption("Patch Asset Checksum")
			.addClick([] {}));
	});

	Framework::addSubmenu("Region Override", "matchmaking-region", [](Framework::Options::Core* core) {
		for (const auto& region_type : regions) {
			core->addOption(Framework::Options::ButtonOption(region_type.name)
				.addClick([] {}));
		}
	});

	Framework::addSubmenu("Pool Override", "matchmaking-pool", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ButtonOption("Normal")
			.addClick([] {}));

		core->addOption(Framework::Options::ButtonOption("Cheater")
			.addClick([] {}));

		core->addOption(Framework::Options::ButtonOption("Bad Sport")
			.addClick([] {}));

		core->addOption(Framework::Options::ButtonOption("Custom")
			.addClick([] {}));
	});

	Framework::addSubmenu("Script Migration", "toolkit-script-migration", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Prevent Stealing")
			.addToggle(&Menu::Network::Vars::m_PreventStealing));

		core->addOption(Framework::Options::ButtonOption("Force Script Host")
			.addClick([] { Menu::Network::ForceScriptHost(Joaat("freemode")); }));
	});
}


static float old_percent = NAN;

GtaThread* FindScriptThreadBase(Rage::joaat_t hash) {
	for (auto thread : *Patterns::Vars::g_ScriptThreads) {
		if (thread && thread->m_context.m_thread_id && thread->m_handler && thread->m_script_hash == hash) {
			return thread;
		}
	}

	return nullptr;
}

#include "rage/classes/base/netArrayHandlerBase.hpp"
eTransitionState last_state = eTransitionState::TRANSITION_STATE_EMPTY;
void TransitionState()
{
	const auto state = Menu::Global(1575008).As<eTransitionState>();
	const std::vector<std::string_view> transition_states = { "TRANSITION_STATE_EMPTY", "Singleplayer Swoop Up", "Multiplayer Swoop Up", "Creator Swoop Up", "Pre-HUD Checks", "Wait HUD Exit", "Wait For Summon", "Singleplayer Swoop Down", "Multiplayer Swoop Down", "Cancel Joining", "Retry Loading", "Retry Loading Slot 1", "Retry Loading Slot 2", "Retry Loading Slot 3", "Retry Loading Slot 4", "Wait On Invite", "Prejoining Freemode Session Checks", "Look For Fresh Join Freemode", "Look To Join Another Session Freemode", "Confirm Freemode Session Joining", "Wait Join Freemode Session", "Creation Enter Session", "Pre-Freemode Launch Script", "Freemode Teamfull Check", "Start Freemode Launch Script", "Freemode Transition Create Player", "Is Freemode And Transition Ready", "Freemode Swoop Down", "Post Bink Video Warp", "Freemode Final Setup Player", "Move Freemode To Running State", "Freemode How To Terminate", "Start Creator Pre-Launch Script Check", "Start Creator Launch Script", "Creator Transition Create Player", "Is Creator And Transition Ready", "Creator Swoop Down", "Creator Final Setup Player", "Move Creator To Running State", "Prejoining Testbed Session Checks", "Look For Fresh Join Testbed", "Look For Fresh Host Testbed", "Look To Join Another Session Testbed", "Look To Host Session Testbed", "Confirm Testbed Session Joining", "Wait Join Testbed Session", "Start Testbed Launch Script", "Testbed Transition Create Player", "Is Testbed And Transition Ready", "Testbed Swoop Down", "Testbed Final Setup Player", "Move Testbed To Running State", "Testbed How To Terminate", "Quit Current Session Prompt", "Wait For Transition Session To Setup", "Terminate Singleplayer", "Wait Terminate Singleplayer", "Kick Terminate Session", "Terminate Session", "Wait Terminate Session", "Terminate Session And Hold", "Terminate Session And Move Into Holding State", "Team Swapping Checks", "Return To Singleplayer", "Wait For Singleplayer To Start", "Waiting For External Termination Call", "Terminate Maintransition", "Wait For Dirty Load Confirm", "DLC Intro Bink", "Spawn Into Personal Vehicle" };

	if (Native::_GetNumberOfInstancesOfScriptWithNameHash(RAGE_JOAAT("maintransition")) == 0)
		return;

	// When freemode script loaded remove loading screen.
	if (state == eTransitionState::TRANSITION_STATE_WAIT_JOIN_FM_SESSION && Native::GetIsLoadingScreenActive())
	{
		Native::ShutdownLoadingScreen();
	}

	if ((last_state == state && state != eTransitionState::TRANSITION_STATE_FM_TRANSITION_CREATE_PLAYER) || state == eTransitionState::TRANSITION_STATE_EMPTY || state > eTransitionState::TRANSITION_STATE_SPAWN_INTO_PERSONAL_VEHICLE)
	{
		return;
	}

	if (state != eTransitionState::TRANSITION_STATE_FM_TRANSITION_CREATE_PLAYER && Native::_IsLoadingPromptBeingDisplayed())
	{
		Native::_RemoveLoadingPrompt();
	}

	if ((int)state > 0 && (int)state < transition_states.size())
	{
		Rage::scrThread* freemode{};
		if (state == eTransitionState::TRANSITION_STATE_FM_TRANSITION_CREATE_PLAYER
			&& (freemode = FindScriptThreadBase(RAGE_JOAAT("freemode")), freemode && freemode->m_net_component))
		{
			int num_array_handlers{};
			int received_array_handlers{};

			while (
				auto handler = Caller::Call<Rage::netArrayHandlerBase*>(Patterns::Vars::g_GetHostArrayHandler, (CGameScriptHandlerNetComponent*)freemode->m_net_component, num_array_handlers++))
				if (handler->m_flags & 1)
					received_array_handlers++;

			if (num_array_handlers == 0)
				num_array_handlers = 1;

			float percent = round((static_cast<float>(received_array_handlers) / num_array_handlers) * 100);

			if (percent != old_percent)
			{
				if (m_Vars.m_LogState) {
					Native::_SetLoadingPromptTextEntry("STRING");
					Native::AddTextComponentSubstringPlayerName(std::format("{}: {}%", "Waiting For Host To Broadcast Data", percent).c_str());
					Native::_ShowLoadingPrompt(5);
				}
			}

			old_percent = percent;
		}
		else
		{
			if (m_Vars.m_LogState) {
				old_percent = NAN;
				Native::_SetLoadingPromptTextEntry("STRING");
				Native::AddTextComponentSubstringPlayerName(transition_states[(int)state].data());
				Native::_ShowLoadingPrompt(5);
			}

		}
	}

	last_state = state;
}

void NetworkMenu::Update() {
	Run();
	GetNetworkPlayersMenu()->Update();
	GetNetworkTunablesMenu()->Update();
	GetNetworkSpoofingMenu()->Update();
	GetNetworkBrowserMenu()->Update();
	GetNetworkPlayerHistoryMenu()->Update();
	GetNetworkFriendsMenu()->Update();

	Menu::Network::PreventStealing();

	if (m_Vars.m_SpeedUp) {
		TransitionState();
	}

	if (m_Vars.m_OpenExtraSlot) {
		 Native::_0x49EC8030F5015F8B(4);
	}
	else {
		Native::_0x49EC8030F5015F8B(0);
	}

	if (m_Spam) {
		if (GetTickCount() - timer >= delay)
		{
			Engine::SendChatMessage(m_Message);
			timer = GetTickCount();
		}
	}
}