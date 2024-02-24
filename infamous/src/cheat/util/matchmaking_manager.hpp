#pragma once

namespace Menu
{
	class MatchmakingManager {
	public:
		constexpr static int MAX_SESSIONS_TO_FIND = 1000;

		struct SessionAttr {
			int Discriminator;
			int PlayerCount;
			int Region;
			int Language;
		};

		struct Session {
			Rage::rlSessionInfo Info;
			SessionAttr Attributes;
			bool IsValid;
		};

	private:
		int m_SessionsFound = 0;
		bool m_Active = false;
		Session m_FoundSessions[MAX_SESSIONS_TO_FIND];

	public:
		MatchmakingManager();
		~MatchmakingManager();
		bool Matchmake(std::optional<int> constraint = std::nullopt);

		inline int GetSessionsFound() {
			return m_SessionsFound;
		}

		inline Session* GetFoundSessions() {
			return m_FoundSessions;
		}

		inline bool IsActive() {
			return m_Active;
		}
	};

	inline MatchmakingManager* g_MatchmakingManager;
}