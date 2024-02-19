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

		core->addOption(Framework::Options::SubmenuOption("New Session")
			.setTarget("network-new-session"));

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
		core->addOption(Framework::Options::SubmenuOption("Script Migration")
			.setTarget("toolkit-script-migration"));

		core->addOption(Framework::Options::SubmenuOption("Matchmaking")
			.setTarget("toolkit-matchmaking"));

		core->addOption(Framework::Options::SubmenuOption("Speed Up Transition")
			.setTarget("toolkit-transition"));
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
	Menu::Network::PreventStealing();

	if (m_Vars.m_SpeedUp) {
		TransitionState();
	}

	if (m_Spam) {
		if (GetTickCount() - timer >= delay)
		{
			Engine::SendChatMessage(m_Message);
			timer = GetTickCount();
		}
	}
}