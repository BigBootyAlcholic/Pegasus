#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/menu/submenus/network/network_browser.hpp"
#include "util/fiber_pool.hpp"
#include "cheat/util/matchmaking_manager.hpp"
namespace Hooks {
	bool StartMatchmakingFindHook(int profileIndex, int availableSlots, NetworkGameFilterMatchmakingComponent* gameFilter, unsigned int maxSessions, Rage::rlSessionInfo* sessionInfo, int* sessionsFound, Rage::rlTaskStatus* taskStatus) {
		int Discriminator = gameFilter->m_param_values[0];

		if (NetworkBrowserMenuVars::m_Vars.m_Populate && gameFilter->m_filter_type == 1) {
			taskStatus->status = 1;

			Utils::GetFiberPool()->Push([maxSessions, sessionInfo, sessionsFound, taskStatus, Discriminator] {
				bool Result = false;
				Result = Menu::g_MatchmakingManager->Matchmake(Discriminator);

				if (Result) {
					for (int i = 0; i < Menu::g_MatchmakingManager->GetSessionsFound(); i++) {
						if (Menu::g_MatchmakingManager->GetFoundSessions()[i].IsValid) {
							sessionInfo[*sessionsFound] = Menu::g_MatchmakingManager->GetFoundSessions()[i].Info;
							(*sessionsFound)++;

							if (maxSessions <= *sessionsFound)
								break;
						}
					}

					taskStatus->status = 3;
				}
				else {
					taskStatus->status = 2;
				}
			});
			return true;
		}
		else {
			return OgStartMatchmakingFind(profileIndex, availableSlots, gameFilter, maxSessions, sessionInfo, sessionsFound, taskStatus);
		}
	}
}