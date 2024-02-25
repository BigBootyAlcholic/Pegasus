#include "stdafx.hpp"
#include "network_player_history.hpp"
#include "cheat/util/player_manager.hpp"
#include "cheat/util/timer.hpp"
#include "cheat/util/global.hpp"
#include "util/json.hpp"
#include "util/config.hpp"
#include "rage/engine.hpp"
#include "cheat/util/notify.hpp"
using namespace NetworkPlayerHistoryMenuVars;
using namespace Framework::Options;
namespace NetworkPlayerHistoryMenuVars {
	Vars_ m_Vars;
	std::string GetCurrentTimeStr() {
		time_t curr_time;
		time(&curr_time);
		tm* curr_tm = localtime(&curr_time);

		char Buffer[40];
		strftime(Buffer, 40, "%a %d %b %Y %H:%M:%S", curr_tm);
		return std::string(Buffer);
	}

	void Save(std::string Name, uint64_t RockstarID) {
		nlohmann::json Json;
		std::vector<RecentPlayer> Players;
		std::vector<RecentPlayer> PermPlayers;

		std::ifstream File(Utils::GetConfig()->GetRecentPlayersPath());
		if (File.good()) {
			try {
				File >> Json;
			}
			catch (std::exception& E) {
				//LOG_ERROR("test");
			}

			File.close();
		}

		if (!Json["recent"].is_null()) {
			for (nlohmann::json::iterator it = Json["recent"].begin(); it != Json["recent"].end(); ++it) {
				RecentPlayer O;
				strcpy(O.m_Name, it.value()["name"].get<std::string>().c_str());
				strcpy(O.m_FirstEncountered, it.value()["first encountered"].get<std::string>().c_str());
				strcpy(O.m_LastEncountered, it.value()["last encountered"].get<std::string>().c_str());
				O.m_RockstarID = std::stoull(it.key());
				O.m_InternalEncountered = it.value()["internal"].get<int>();

				Players.push_back(O);
			}
		}

		if (!Json["perm"].is_null()) {
			for (nlohmann::json::iterator it = Json["perm"].begin(); it != Json["perm"].end(); ++it) {
				RecentPlayer O;
				strcpy(O.m_Name, it.value()["name"].get<std::string>().c_str());
				strcpy(O.m_FirstEncountered, it.value()["first encountered"].get<std::string>().c_str());
				strcpy(O.m_LastEncountered, it.value()["last encountered"].get<std::string>().c_str());

				O.m_RockstarID = std::stoull(it.key());
				O.m_InternalEncountered = it.value()["internal"].get<int>();

				PermPlayers.push_back(O);
			}
		}

		bool Exists = false;
		for (auto& P : Players) {
			if (P.m_RockstarID == RockstarID && !strcmp(P.m_Name, Name.c_str())) {
				Exists = true;
				strcpy(P.m_LastEncountered, GetCurrentTimeStr().c_str());
				P.m_InternalEncountered = (int)time(0);
				break;
			}
		}

		if (!Exists) {
			RecentPlayer NewPlayer;
			strcpy(NewPlayer.m_Name, Name.c_str());
			strcpy(NewPlayer.m_FirstEncountered, GetCurrentTimeStr().c_str());
			strcpy(NewPlayer.m_LastEncountered, GetCurrentTimeStr().c_str());
			NewPlayer.m_InternalEncountered = (int)time(0);
			NewPlayer.m_RockstarID = RockstarID;
			Players.push_back(NewPlayer);
		}

		std::sort(Players.begin(), Players.end(), [](RecentPlayer& first, RecentPlayer& second) { return first.m_InternalEncountered > second.m_InternalEncountered; });

		std::ofstream Output(Utils::GetConfig()->GetRecentPlayersPath());
		if (Output.good()) {
			nlohmann::json Resp;

			for (auto& p : PermPlayers) {
				Resp["perm"][std::to_string(p.m_RockstarID)]["name"] = p.m_Name;
				Resp["perm"][std::to_string(p.m_RockstarID)]["first encountered"] = p.m_FirstEncountered;
				Resp["perm"][std::to_string(p.m_RockstarID)]["last encountered"] = p.m_LastEncountered;
				Resp["perm"][std::to_string(p.m_RockstarID)]["internal"] = p.m_InternalEncountered;
			}

			int Count = 0;
			for (auto& p : Players) {
				if (Count > 100) break;
				Resp["recent"][std::to_string(p.m_RockstarID)]["name"] = p.m_Name;
				Resp["recent"][std::to_string(p.m_RockstarID)]["first encountered"] = p.m_FirstEncountered;
				Resp["recent"][std::to_string(p.m_RockstarID)]["last encountered"] = p.m_LastEncountered;
				Resp["recent"][std::to_string(p.m_RockstarID)]["internal"] = p.m_InternalEncountered;
				Count++;
			}

			Output << Resp.dump(4);
			Output.close();
		}
		//Menu::Notify::stacked("DONE");
	}

	void Load() {
		std::ifstream File(Utils::GetConfig()->GetRecentPlayersPath());
		if (File.good()) {
			nlohmann::json Json;

			try {
				File >> Json;
			}
			catch (std::exception& E) {
				Menu::Notify::stacked(std::format("[JSON] {}", E.what()).c_str());
			}

			File.close();

			Json = Json["recent"];

			m_Vars.m_Players.clear();
			for (nlohmann::json::iterator it = Json.begin(); it != Json.end(); ++it) {
				RecentPlayer O;
				strcpy(O.m_Name, it.value()["name"].get<std::string>().c_str());
				if (strlen(O.m_Name) == 0) strcpy(O.m_Name, "Invalid Name");

				strcpy(O.m_FirstEncountered, it.value()["first encountered"].get<std::string>().c_str());
				strcpy(O.m_LastEncountered, it.value()["last encountered"].get<std::string>().c_str());

				O.m_InternalEncountered = it.value()["internal"].get<int>();
				O.m_RockstarID = std::stoull(it.key());

				if (m_Vars.m_Players.size() == 100) break;
				if (O.m_RockstarID > 0x1000) m_Vars.m_Queue.push_back(O.m_RockstarID);
				m_Vars.m_Players.push_back({ it.key(), O });
			}

			std::sort(m_Vars.m_Players.begin(), m_Vars.m_Players.end(), [](std::pair<std::string, RecentPlayer>& first, std::pair<std::string, RecentPlayer>& second) { return first.second.m_InternalEncountered > second.second.m_InternalEncountered; });
		}
	}

	void MakePermanent() {
		try {
			nlohmann::json Json;

			std::ifstream File(Utils::GetConfig()->GetRecentPlayersPath());
			if (File.good()) {
				File >> Json;
				File.close();

				std::ofstream Output(Utils::GetConfig()->GetRecentPlayersPath());
				if (Output.good()) {
					auto p = m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer).second;
					Json["perm"][std::to_string(p.m_RockstarID)]["name"] = p.m_Name;
					Json["perm"][std::to_string(p.m_RockstarID)]["first encountered"] = p.m_FirstEncountered;
					Json["perm"][std::to_string(p.m_RockstarID)]["last encountered"] = p.m_LastEncountered;
					Json["perm"][std::to_string(p.m_RockstarID)]["internal"] = p.m_InternalEncountered;
					Output << Json.dump(4);
					Output.close();
				}
			}
		}
		catch (std::exception& E) {
			Menu::Notify::stacked(std::format("[JSON] {}", E.what()).c_str());
		}
	}

	void LoadPerm() {
		try {
			std::ifstream File(Utils::GetConfig()->GetRecentPlayersPath());
			if (File.good()) {
				nlohmann::json Json;
				File >> Json;
				File.close();

				Json = Json["perm"];

				::m_Vars.m_PermanentPlayers.clear();

				for (nlohmann::json::iterator it = Json.begin(); it != Json.end(); ++it) {
					RecentPlayer O;
					strcpy(O.m_Name, it.value()["name"].get<std::string>().c_str());
					if (strlen(O.m_Name) == 0) strcpy(O.m_Name, "Invalid Name");

					strcpy(O.m_FirstEncountered, it.value()["first encountered"].get<std::string>().c_str());
					strcpy(O.m_LastEncountered, it.value()["last encountered"].get<std::string>().c_str());
					O.m_InternalEncountered = it.value()["internal"].get<int>();
					O.m_RockstarID = std::stoull(it.key());

					if (m_Vars.m_PermanentPlayers.size() == 100) break;
					m_Vars.m_Queue.push_back(O.m_RockstarID);
					m_Vars.m_PermanentPlayers.push_back({ it.key(), O });
				}

				std::sort(m_Vars.m_PermanentPlayers.begin(), m_Vars.m_PermanentPlayers.end(), [](std::pair<std::string, RecentPlayer>& first, std::pair<std::string, RecentPlayer>& second) { return first.second.m_InternalEncountered > second.second.m_InternalEncountered; });
			}
		}
		catch (std::exception& E) {
			Menu::Notify::stacked(std::format("[JSON] {}", E.what()).c_str());
		}
	}
}


static bool init = false;
static bool init2 = false;
void NetworkPlayerHistoryMenu::Run() {
	Framework::addSubmenu("Player History", "network_player_history", [](Framework::Options::Core* core) {


		//if (!init) {
			Load();
			//init = true;
		//}//

		core->addOption(Framework::Options::SubmenuOption("Saved Players")
			.setTarget("history-saved"));

	
		core->addOption(BreakOption(std::format("Player History ({})", m_Vars.m_Players.size()).c_str()));

		if (m_Vars.m_Players.size()) {
			for (std::size_t i = 0; i < m_Vars.m_Players.size(); i++) {
				auto Player = m_Vars.m_Players[i];
			
				core->addOption(Framework::Options::SubmenuOption(Player.second.m_Name)
					.addClick([=] {
						m_Vars.m_SelectedPlayer = i;
						m_Vars.m_SelectedList = &m_Vars.m_Players;
					}).setTarget("history-selected"));
			}
		}
	});
	Framework::addSubmenu("Recent Player", "history-selected", [](Framework::Options::Core* core) {

		core->addOption(Framework::Options::ButtonOption("View Profile")
			.addClick([] {
				auto rid = m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer).second.m_RockstarID;
				if (rid > 0) {
					int netHandle[13];
					Native::NetworkHandleFromMemberId(std::to_string(rid).c_str(), netHandle, 13);
					Native::NetworkShowProfileUi(netHandle);
				}
				}));

		core->addOption(Framework::Options::ButtonOption("Invite To Session")
			.addClick([] {
				Engine::InvitePlayer(m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer).second.m_RockstarID);
				}));

		core->addOption(Framework::Options::ButtonOption("Join Player")
			.addClick([] {
				Engine::JoinSessionByRid(m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer).second.m_RockstarID);
				}));


		core->addOption(Framework::Options::ButtonOption("Save Player")
			.addClick([] { MakePermanent(); }));
		});
	Framework::addSubmenu("Saved Players", "history-saved", [](Framework::Options::Core* core) {

		if (!init2) {
			LoadPerm();
			init2 = true;
		}

		if (m_Vars.m_PermanentPlayers.size()) {
			for (std::size_t i = 0; i < m_Vars.m_PermanentPlayers.size(); i++) {
				auto Player = m_Vars.m_PermanentPlayers[i];

				core->addOption(Framework::Options::SubmenuOption(Player.second.m_Name)
					.addClick([=] {
						m_Vars.m_SelectedPlayer = i;
						m_Vars.m_SelectedList = &m_Vars.m_PermanentPlayers;
					}).setTarget("historysaved-selected"));

			}
		}
	});
	Framework::addSubmenu("Recent Saved Player", "historysaved-selected", [](Framework::Options::Core* core) {

		core->addOption(Framework::Options::ButtonOption("View Profile")
			.addClick([] {
				auto rid = m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer).second.m_RockstarID;
				if (rid > 0) {
					int netHandle[13];
					Native::NetworkHandleFromMemberId(std::to_string(rid).c_str(), netHandle, 13);
					Native::NetworkShowProfileUi(netHandle);
				}
		}));

		core->addOption(Framework::Options::ButtonOption("Invite To Session")
			.addClick([] {
				Engine::InvitePlayer(m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer).second.m_RockstarID);
		}));

		core->addOption(Framework::Options::ButtonOption("Join Player")
			.addClick([] {
				Engine::JoinSessionByRid(m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer).second.m_RockstarID);
		}));
	});
}

void NetworkPlayerHistoryMenu::Update() {
	Run();

}