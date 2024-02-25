#pragma once
#include "util.hpp"
extern void TypeLogic();
extern void Run();
class ScriptEvent {
public:
	std::string m_Name{};
	std::string m_OldNameHash{};
	std::string m_NameHash{};
	int32_t m_OldHash{};
	int32_t m_Hash{};
};
class ScriptEvents {
public:
	void Write(int iteration, bool snake, bool camel, bool upper, bool toFile, bool print);
	void Read();
private:
	void ReadContents();
	void GetMatches();
public:
	void FormatMatches(int enumIteration);
private:
	void WriteEnum(bool snake, bool camel, bool upper);
	void WriteAllOccurrences();
	void WriteHashCheck();
private:
	std::stringstream m_stream{};
	std::string m_contents{};
	std::vector<std::smatch> m_matches{};
	std::vector<ScriptEvent> m_events{};
};
inline ScriptEvents g_ScriptEvents{};