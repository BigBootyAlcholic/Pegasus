#include "ScriptEventUpdater.hpp"

void ScriptEvents::Write(int iteration, bool snake, bool camel, bool upper, bool toFile, bool print) {
	m_stream << std::endl;
	WriteEnum(snake, camel, upper);
	m_stream << " | Iteration: " << iteration;
	WriteHashCheck();
	std::string str{ m_stream.str() };
	if (toFile) {
		std::ofstream file{ "ScriptEvents.hpp" };
		file << str << std::endl;
	}
	if (print) {
		std::cout << str << std::endl;
	}
}
void ScriptEvents::Read() {
	ReadContents();
	GetMatches();
}
void ScriptEvents::ReadContents() {
	std::ifstream file{ "MP_Event_Enums.sch" };
	m_contents = { (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>() };
}
void ScriptEvents::GetMatches() {
	std::regex expression{ R"_(\w+\s*=\s*HASH\(\"(.*)\"\))_" };
	std::sregex_iterator iter{ m_contents.begin(), m_contents.end(), expression };
	std::sregex_iterator end{};
	while (iter != end) {
		m_matches.push_back(*iter);
		++iter;
	}
}
void ScriptEvents::FormatMatches(int enumIteration) {
	std::string strToReplace{ "Event_Enums12" }; //12 is from 1.58, 14 is from 1.62-1.64, 15 is from1.65-1.66
	for (auto& match : m_matches) {
		ScriptEvent se{};
		std::string str{ match.str(1) };
		if (size_t pos{ str.find(strToReplace) }; pos != std::string::npos) {
			std::string strBefore{ str.substr(0, pos) };
			std::string seperatedName{ str.substr(pos + strToReplace.size() + 1) }; //+1 for the period
			std::string nameHash{ strBefore + "Event_Enums" + std::to_string(enumIteration) + "." + seperatedName };
			std::string oldNameHash{ strBefore + "Event_Enums" + std::to_string(enumIteration - 1) + "." + seperatedName };
			std::string name{ seperatedName };
			if (size_t schPos{ name.find("sch.") }; schPos != std::string::npos) {
				name = seperatedName.substr(schPos + sizeof("sch.") - 1); //-1 for null terminator
			}
			se.m_Name = name;
			se.m_NameHash = nameHash;
			se.m_OldNameHash = oldNameHash;
			se.m_OldHash = Util::Joaat(se.m_OldNameHash);
			se.m_Hash = Util::Joaat(se.m_NameHash);
			m_events.push_back(se);
		}
	}
}
void ScriptEvents::WriteEnum(bool snake, bool camel, bool upper) {
	m_stream << "enum class eScriptEvents {" << '\n';
	for (int i{}; i != m_events.size() - 2; ++i) {
		auto& se{ m_events[i] };
		m_stream << Util::Enumify(se.m_Name, se.m_Hash, snake, camel, upper) << '\n';
	}
	auto& se{ m_events[m_events.size() - 1] };
	m_stream << Util::Enumify(se.m_Name, se.m_Hash, snake, camel, upper, true) << '\n';
	m_stream << "}; //Number of events: " << m_events.size();
}
void ScriptEvents::WriteAllOccurrences() {
	for (int i{}; i != 20; ++i) {
		FormatMatches(i);
	}
	for (int i{}; i != m_events.size(); ++i) {
		auto& se{ m_events[i] };
		if (se.m_NameHash.find("TICKER_EVENT_SWAP_TEAM") != -1) {
			m_stream << se.m_NameHash << " = " << se.m_Hash << '\n';
		}
	}
}
void ScriptEvents::WriteHashCheck() {
	m_events.clear();
	FormatMatches(7);
	for (int i{}; i != m_events.size(); ++i) {
		auto& se{ m_events[i] };
		if (se.m_NameHash.find("TICKER_EVENT_SWAP_TEAM") != -1) {
			if (se.m_Hash != -2070646108) {
				m_stream << " | Hashes broke at some point. Please do not use these hashes until fixed";
			}
			else {
				m_stream << " | Hashes are functioning properly";
			}
		}
	}
	m_events.clear();
}

void TypeLogic() {
	Vars::g_Iteration = std::stoi(Util::GetLine("Iteration"));
	Vars::g_snakeCase = Util::ConditionMet("Use snake case");
	Vars::g_camelCase = Util::ConditionMet("Use camel case");
	Vars::g_Uppercase = Util::ConditionMet("Uppercase");
	Vars::g_ToFile = Util::ConditionMet("Write to file");
	Vars::g_Print = Util::ConditionMet("Print contents");
}
void Run() {
	g_ScriptEvents.Read();
	TypeLogic();
	g_ScriptEvents.FormatMatches(Vars::g_Iteration);
	g_ScriptEvents.Write(Vars::g_Iteration, Vars::g_snakeCase, Vars::g_camelCase, Vars::g_Uppercase, Vars::g_ToFile, Vars::g_Print);
	if (Vars::g_Print) {
		Util::Wait();
	}
}
int main() {
	Run();
	return 0;
}