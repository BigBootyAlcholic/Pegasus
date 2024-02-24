#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/menu/submenus/network/network_browser.hpp"
#include "util/fiber_pool.hpp"
#include "cheat/util/matchmaking_manager.hpp"

namespace Rage {
	class XmlNode {
	public:
		char* m_Key;                    //0x0000
		char pad_0008[32];              //0x0008
		class Rage::XmlNode* m_Sibling;//0x0028
		class Rage::XmlNode* m_Child;  //0x0030
		char* m_Value;                  //0x0038
		char pad_0040[8];               //0x0040

		inline XmlNode* GetChildNode(const char* name) {
			for (auto node = m_Child; node; node = node->m_Sibling) {
				if (strcmp(name, node->m_Key) == 0)
					return node;
			}

			return nullptr;
		}
	};//Size: 0x0048
	static_assert(sizeof(Rage::XmlNode) == 0x48);
}
namespace {
	static std::vector<std::string> SplitString(const std::string& s, char seperator)
	{
		std::vector<std::string> output;

		std::string::size_type prev_pos = 0, pos = 0;

		while ((pos = s.find(seperator, pos)) != std::string::npos) {
			std::string substring(s.substr(prev_pos, pos - prev_pos));

			output.push_back(substring);

			prev_pos = ++pos;
		}

		output.push_back(s.substr(prev_pos, pos - prev_pos));

		return output;
	}
}

namespace Hooks {
	bool ProcessMatchmakingResponseHook(void* _This, void* unusedUnk, Rage::XmlNode* xmlNode, int* unk) {
		bool ret = OgProcessMatchmakingResponse(_This, unusedUnk, xmlNode, unk);

		if (Menu::g_MatchmakingManager->IsActive()) {
			int i = 0;
			for (auto result = xmlNode->GetChildNode("Results")->m_Child; result; result = result->m_Sibling) {
				const auto& values = SplitString(result->GetChildNode("Attributes")->m_Value, ',');
				Menu::g_MatchmakingManager->GetFoundSessions()[i].Attributes.Discriminator = std::stoi(values[2]);
				Menu::g_MatchmakingManager->GetFoundSessions()[i].Attributes.PlayerCount = std::stoi(values[4]);
				Menu::g_MatchmakingManager->GetFoundSessions()[i].Attributes.Language = std::stoi(values[5]);
				Menu::g_MatchmakingManager->GetFoundSessions()[i].Attributes.Region = std::stoi(values[6]);
				i++;
			}
		}

		return ret;
	}
}