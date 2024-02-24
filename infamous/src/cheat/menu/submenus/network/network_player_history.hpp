#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "core/core.hpp"

class NetworkPlayerHistoryMenu {
public:
	void Run();
	void Update();
};

static NetworkPlayerHistoryMenu* GetNetworkPlayerHistoryMenu() {
	static NetworkPlayerHistoryMenu instance;
	return &instance;
}

namespace NetworkPlayerHistoryMenuVars {
	struct RecentPlayer {
		char m_Name[0x20];
		char m_FirstEncountered[0x50];
		char m_LastEncountered[0x50];
		uint64_t m_RockstarID;
		int m_InternalEncountered;

		
	};


	struct Vars_ {
		std::vector<uint64_t> m_Queue;
		std::vector<std::pair<std::string, RecentPlayer>> m_Players;
		std::vector<std::pair<std::string, RecentPlayer>> m_PermanentPlayers;
		std::vector<std::pair<std::string, RecentPlayer>>* m_SelectedList;
		std::size_t m_SelectedPlayer;
	};

	void Save(std::string Name, uint64_t RockstarID);
	void MakePermanent();
	extern Vars_ m_Vars;
}