
#include "stdafx.hpp"
#include "input.hpp"
#include "rage/invoker/natives.hpp"
namespace Menu::Util {
	struct KeyboardState {
		uint32_t m_Time;
		bool m_IsWithAlt;
		bool m_WasDownBefore;
		bool m_IsUpNow;
		bool m_JustUp;
		bool m_JustDown;
	}; KeyboardState g_KeyStates[255];


	bool Input::IsJustReleased(bool Keyboard, int Key, bool OverrideInput) {
		

		if (Keyboard) {
			bool result = GetTickCount() < (g_KeyStates[Key].m_Time + 100) && g_KeyStates[Key].m_IsUpNow;
			memset(&g_KeyStates[Key], 0, sizeof(g_KeyStates[0]));
			return result;
		}

		return Native::IsDisabledControlJustReleased(0, Key);
	}

	bool Input::IsJustPressed(bool Keyboard, int Key, bool OverrideInput) {
		

		if (Keyboard) {
			return ((GetAsyncKeyState(Key) & 0xFFFF) == 0x8001);

			/*bool result = (GetTickCount() - 100) < g_KeyStates[Key].m_Time && !g_KeyStates[Key].m_IsUpNow && g_KeyStates[Key].m_JustDown;
			if (result) g_KeyStates[Key].m_JustDown = false;
			return result;*/
		}

		return Native::IsDisabledControlJustPressed(0, Key);
	}

	bool Input::IsPressed(bool Keyboard, int Key, bool OverrideInput) {


		if (Keyboard) {
			return (GetAsyncKeyState(Key) & 0x8000);
			// return GetTickCount() < (g_KeyStates[Key].m_Time + 600000) && !g_KeyStates[Key].m_IsUpNow;
		}

		return Native::IsDisabledControlPressed(0, Key);
	}

	bool Input::IsOpenBindPressed(bool OverrideInput) {
		return (IsPressed(false, ControlFrontendRb) && IsPressed(false, ControlFrontendRight)) || IsJustPressed(true, VK_F4);
	}

	bool Input::IsAJustPressed(bool OverrideInput) {
		
		return Native::IsDisabledControlJustReleased(0, ControlFrontendAccept) || IsJustPressed(true, VK_NUMPAD5);
	}

	bool Input::IsBJustPressed(bool OverrideInput) {
	
		return Native::IsDisabledControlJustReleased(0, ControlFrontendCancel) || IsJustPressed(true, VK_NUMPAD0);
	}

	bool Input::IsOptionPressed(bool OverrideInput) {
		

		if (IsAJustPressed()) {
			Native::PlaySoundFrontend(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
			return true;
		}

		return false;
	}

	bool Input::IsLeftJustPressed(bool OverrideInput) {
		
		return IsJustPressed(false, ControlFrontendLeft) || IsJustPressed(true, VK_NUMPAD4);
	}

	bool Input::IsRightJustPressed(bool OverrideInput) {

		return IsJustPressed(false, ControlFrontendRight) || IsJustPressed(true, VK_NUMPAD6);
	}

	bool Input::IsLeftPressed(bool OverrideInput) {
	
		return IsPressed(false, ControlFrontendLeft) || IsPressed(true, VK_NUMPAD4);
	}

	bool Input::IsRightPressed(bool OverrideInput) {

		return IsPressed(false, ControlFrontendRight) || IsPressed(true, VK_NUMPAD6);
	}

	Input* GetInput() {
		static Input Instance;
		return &Instance;
	}
}