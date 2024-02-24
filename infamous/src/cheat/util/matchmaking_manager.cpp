#include "stdafx.hpp"
#include "matchmaking_manager.hpp"
#include "cheat/menu/submenus/network/network_browser.hpp"
#include "rage/classes/network/Network.hpp"
#include "core/patterns.hpp"
#include "core/hooks.hpp"
#include "util/fiber.hpp"
#include "util/fiber_pool.hpp"
namespace Menu {
	MatchmakingManager::MatchmakingManager()
	{
		g_MatchmakingManager = this;
	}

	MatchmakingManager::~MatchmakingManager()
	{
		g_MatchmakingManager = nullptr;
	}

	bool MatchmakingManager::Matchmake(std::optional<int> constraint)
	{
		for (auto& session : m_FoundSessions) {
			session.IsValid = true;
		}

		NetworkGameFilterMatchmakingComponent component{};
		strcpy(component.m_filter_name, "Group");
		component.m_game_mode = 0;
		component.m_num_parameters = 0;

		if (NetworkBrowserMenuVars::m_Vars.m_EnableRegionFilter)
		{
			component.SetParameter("MMATTR_REGION", 0, NetworkBrowserMenuVars::m_Vars.m_RegionFilter);
		}

		if (constraint)
		{
			component.SetParameter("MMATTR_DISCRIMINATOR", 1, constraint.value());
		}

		Rage::rlTaskStatus state{};
		static Rage::rlSessionInfo result_sessions[MAX_SESSIONS_TO_FIND];

		m_Active = true;

		if (Hooks::OgStartMatchmakingFind(0, 1, &component, MAX_SESSIONS_TO_FIND, result_sessions, &m_SessionsFound, &state)) {

			while (state.status == 1)
				Utils::GetFiberManager()->GoToMainFiber();

			if (state.status == 3)
			{
				for (int i = 0; i < m_SessionsFound; i++)
				{
					m_FoundSessions[i].Info = result_sessions[i];

					if (constraint && m_FoundSessions[i].Attributes.PlayerCount >= 30)
						m_FoundSessions[i].IsValid = false;

					if (NetworkBrowserMenuVars::m_Vars.m_EnableLanguageFilter
						&& m_FoundSessions[i].Attributes.Language != NetworkBrowserMenuVars::m_Vars.m_LangaugeFilter)
						m_FoundSessions[i].IsValid = false;

					if (NetworkBrowserMenuVars::m_Vars.m_EnablePlayerCountFilter
						&& (m_FoundSessions[i].Attributes.PlayerCount < NetworkBrowserMenuVars::m_Vars.m_PlayerCountFilterMin
							|| m_FoundSessions[i].Attributes.PlayerCount > NetworkBrowserMenuVars::m_Vars.m_PlayerCountFilterMax))
					{
						m_FoundSessions[i].IsValid = false;
					}

					if (NetworkBrowserMenuVars::m_Vars.m_EnablePoolFilter
						&& ((m_FoundSessions[i].Attributes.Discriminator & (1 << 14)) == (1 << 14))
						!= (bool)NetworkBrowserMenuVars::m_Vars.m_PoolFilter)
						m_FoundSessions[i].IsValid = false;
				}

				if (NetworkBrowserMenuVars::m_Vars.m_SortMethod != 0)
				{
					std::qsort(m_FoundSessions, m_SessionsFound, sizeof(Session), [](const void* a1, const void* a2) -> int {
						std::strong_ordering result;

						if (NetworkBrowserMenuVars::m_Vars.m_SortMethod == 1)
						{
							result = (((Session*)(a1))->Attributes.PlayerCount <=> ((Session*)(a2))->Attributes.PlayerCount);
						}

						if (result == 0)
							return 0;

						if (result > 0)
							return NetworkBrowserMenuVars::m_Vars.m_SortDirection ? -1 : 1;

						if (result < 0)
							return NetworkBrowserMenuVars::m_Vars.m_SortDirection ? 1 : -1;


						std::unreachable();
						});
				}

				m_Active = false;
				return true;
			}
		}
		else
		{
			m_Active = false;
			return false;
		}

		m_Active = false;
		return false;
	}
}