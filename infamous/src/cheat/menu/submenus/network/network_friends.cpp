#include "stdafx.hpp"
#include "network_friends.hpp"
#include "cheat/util/player_manager.hpp"
#include "cheat/util/timer.hpp"
#include "cheat/util/global.hpp"
#include "util/va.hpp"
#include "rage/engine.hpp"
using namespace NetworkFriendsMenuVars;
using namespace Framework::Options;
namespace NetworkFriendsMenuVars {
	Vars_ m_Vars;
}

void NetworkFriendsMenu::Run() {
	Framework::addSubmenu("Friends", "network_friends", [](Framework::Options::Core* core) {
		if (Native::NetworkGetFriendCount() == 0) {
			core->addOption(ButtonOption("~c~None"));
		}
		else {
			for (int i = 0; i < Native::NetworkGetFriendCount(); i++) {
				Friends* Friend = (Friends*)&Patterns::Vars::g_Friends[i];
				core->addOption(SubmenuOption(Utils::VA::VA("%s%s", (abs(Friend->m_State) == 3) ? Friend->m_Name, "[Offline]" : "~c~", Friend->m_Name))
					.addClick([=] { m_Vars.m_Selected = Friend; m_Vars.m_Type = 0; }).setTarget("selected_friend_menu"));
			}
		}
	});

	Framework::addSubmenu("Friends", "selected_friend_menu", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ButtonOption("View Profile")
			.addClick([] {
				auto rid = m_Vars.m_Selected->m_RockstarID;
				if (rid > 0) {
					int netHandle[13];
					Native::NetworkHandleFromMemberId(std::to_string(rid).c_str(), netHandle, 13);
					Native::NetworkShowProfileUi(netHandle);
				}
				}));

		core->addOption(Framework::Options::ButtonOption("Invite To Session")
			.addClick([] {
				Engine::InvitePlayer(m_Vars.m_Selected->m_RockstarID);
			}));

		core->addOption(Framework::Options::ButtonOption("Join Player")
			.addClick([] {
				Engine::JoinSessionByRid(m_Vars.m_Selected->m_RockstarID);
			}));
	});
}



void NetworkFriendsMenu::Update() {
	Run();

}