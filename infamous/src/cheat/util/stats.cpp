#include "stdafx.hpp"
#include "stats.hpp"
#include "rage/invoker/natives.hpp"
#include "cheat/menu/submenus/recovery.hpp"

namespace Stats {
	template<class T>
	void Interface<T>::Set(const char* StatName, T Value) {
		bool UseCharacter = strstr(StatName, "MPPLY") == NULL && strstr(StatName, "MP_PLAYING_TIM") == NULL;

		char Stat[100];
		sprintf(Stat, "%s%s", UseCharacter ? ("MP" + std::to_string(RecoveryMenuVars::m_Vars.m_SelectedCharacter) + "_").c_str() : "", StatName);

		if (std::is_same<T, int>::value) {
			Native::StatSetInt(Native::GetHashKey(Stat), Value, true);
		}
		else if (std::is_same<T, float>::value) {
			Native::StatSetFloat(Native::GetHashKey(Stat), Value, true);
		}
		else if (std::is_same<T, bool>::value) {
			Native::StatSetBool(Native::GetHashKey(Stat), Value, true);
		}
	}

	template<class T>
	T Interface<T>::Get(const char* StatName) {
		T Return;
		bool UseCharacter = strstr(StatName, "MPPLY") == NULL && strstr(StatName, "MP_PLAYING_TIM") == NULL;

		char Stat[100];
		sprintf(Stat, "%s%s", UseCharacter ? ("MP" + std::to_string(RecoveryMenuVars::m_Vars.m_SelectedCharacter) + "_").c_str() : "", StatName);

		if (std::is_same<T, int>::value) {
			Native::StatGetInt(Native::GetHashKey(Stat), (int*)&Return, true);
		}
		else if (std::is_same<T, float>::value) {
			Native::StatGetFloat(Native::GetHashKey(Stat), (float*)&Return, true);
		}
		else if (std::is_same<T, bool>::value) {
			return Native::StatGetBool(Native::GetHashKey(Stat), (bool*)&Return, true);
		}

		return Return;
	}

	template<class T>
	void Interface<T>::Set(uint32_t StatHash, T Value) {
		if (std::is_same<T, int>::value) {
			Native::StatSetInt(StatHash, Value, true);
		}
		else if (std::is_same<T, float>::value) {
			Native::StatSetFloat(StatHash, Value, true);
		}
		else if (std::is_same<T, bool>::value) {
			Native::StatSetBool(StatHash, Value, true);
		}
	}

	template<class T>
	T Interface<T>::Get(uint32_t StatHash) {
		T Return;

		if (std::is_same<T, int>::value) {
			Native::StatGetInt(StatHash, (int*)&Return, true);
		}
		else if (std::is_same<T, float>::value) {
			Native::StatGetFloat(StatHash, (float*)&Return, true);
		}
		else if (std::is_same<T, bool>::value) {
			return Native::StatGetBool(StatHash, (bool*)&Return, true);
		}

		return Return;
	}

	template class Interface<int>;
	template class Interface<float>;
	template class Interface<bool>;
}