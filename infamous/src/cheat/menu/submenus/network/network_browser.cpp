#include "stdafx.hpp"
#include "network_browser.hpp"
#include "cheat/util/player_manager.hpp"
#include "cheat/util/timer.hpp"
#include "cheat/util/global.hpp"
#include "util/json.hpp"
#include "util/config.hpp"
#include "rage/engine.hpp"
#include "cheat/util/notify.hpp"
#include "cheat/util/matchmaking_manager.hpp"
using namespace NetworkBrowserMenuVars;
using namespace Framework::Options;
namespace NetworkBrowserMenuVars {
	Vars_ m_Vars;


	struct RegionType
	{
		uint32_t id;
		const char name[22];
	};

	const RegionType Regions[] = {
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

	std::string RegionTypeToString(int type) {
		switch (type) {
		case 0:
			return "CIS";
			break;
		case 1:
			return "Africa";
			break;
		case 2:
			return "Asia";
			break;
		case 3:
			return "Europe";
			break;
		case 4:
			return "China";
			break;
		case 5:
			return "Australia";
			break;
		case 6:
			return "West";
			break;
		case 7:
			return "Japan";
			break;
		default:
			return "Unknown";
			break;
		}
	}

	inline const char* RegionNames[] = {
	  "CIS", "Africa", "East", "Europe", "China", "Australia", "West", "Japan", "Unknown"
	};

	inline const char* LanguageNames [] = {
	 "English", "French", "German", "Italian", "Spanish ", "Portuguese ", "Polish", "Russian", "Korean", "Chinese", "Japanese", "Spanish (Mexico)", "Chinese (Simpified)"
	};

	static int selected_session_idx = -1;
}


void NetworkBrowserMenu::Run() {
	Framework::addSubmenu("Session Browser", "network_session_browser", [](Framework::Options::Core* core) {
	
		core->addOption(ToggleNumberOption<int>("Min Players")
			.addToggle(&m_Vars.m_EnablePlayerCountFilter).addNumber(&m_Vars.m_PlayerCountFilterMin).addMin(0).addMax(32));

		core->addOption(ToggleNumberOption<int>("Max Players")
			.addToggle(&m_Vars.m_EnablePlayerCountFilter).addNumber(&m_Vars.m_PlayerCountFilterMax).addMin(0).addMax(32));

		core->addOption(SubmenuOption("Filters")
			.setTarget("browser-filters"));

		core->addOption(ButtonOption("Refresh")
			.addClick([] {
				selected_session_idx = -1;
				if (!Menu::g_MatchmakingManager->Matchmake()) {
					Menu::Notify::stacked("Failed To Search For Sessions");
				}
			}));

		std::string BreakString = std::format("{}+ Sessions", Menu::g_MatchmakingManager->GetSessionsFound()).c_str();

		core->addOption(BreakOption(BreakString.c_str()));

		if (selected_session_idx == -1) {
			if (Menu::g_MatchmakingManager->GetSessionsFound()) {
				for (int i = 0; i < Menu::g_MatchmakingManager->GetSessionsFound(); i++) {
					m_Vars.m_FoundSession = Menu::g_MatchmakingManager->GetFoundSessions()[i];

					if (!m_Vars.m_FoundSession.IsValid)
						continue;

					std::string ToString = std::format("{}; {} Players", RegionTypeToString(m_Vars.m_FoundSession.Attributes.Region), m_Vars.m_FoundSession.Attributes.PlayerCount).c_str();
					core->addOption(SubmenuOption(ToString.c_str())
						.addClick([=] { selected_session_idx = i; }).setTarget("selected-browsed-session"));
				}
			}
			else {
				core->addOption(ButtonOption("~c~None"));
			}
		}

	});

	Framework::addSubmenu("Selected Session", "selected-browsed-session", [](Framework::Options::Core* core) {
		auto& Session = Menu::g_MatchmakingManager->GetFoundSessions()[selected_session_idx];

		core->addOption(ButtonOption("Join Session")
			.addClick([=] { Engine::JoinSession(Session.Info); }));
	
	});

	Framework::addSubmenu("Filters", "browser-filters", [](Framework::Options::Core* core) {

		core->addOption(ToggleOption("Region Filter")
			.addToggle(&m_Vars.m_EnableRegionFilter));

		if (m_Vars.m_EnableRegionFilter) {
			core->addOption(scrollOption<const char*, int>("Region")
				.addScroll(&RegionNames).setPosition(&m_Vars.m_RegionFilter));
		}

		core->addOption(ToggleOption("Language Filter")
			.addToggle(&m_Vars.m_EnableLanguageFilter));

		if (m_Vars.m_EnableLanguageFilter) {
			core->addOption(scrollOption<const char*, int>("Language")
				.addScroll(&LanguageNames).setPosition(&m_Vars.m_LangaugeFilter));
		}

		core->addOption(ToggleOption("Pool Filter")
			.addToggle(&m_Vars.m_EnablePoolFilter));

		if (m_Vars.m_EnablePoolFilter) {
			core->addOption(Framework::Options::ButtonOption("Normal")
				.addClick([] {}));

			core->addOption(Framework::Options::ButtonOption("Cheater")
				.addClick([] {}));

			core->addOption(Framework::Options::ButtonOption("Bad Sport")
				.addClick([] {}));

			core->addOption(Framework::Options::ButtonOption("Custom")
				.addClick([] {}));
		}

	});

}

void NetworkBrowserMenu::Update() {
	Run();
}