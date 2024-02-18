#pragma once
#include "stdafx.hpp"

namespace Menu::Util {
	class Input {
	public:
		static LRESULT APIENTRY WindowProcessCallback(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);

		void Unload();
		void Update();
		void ScrollUp();
		void ScrollDown(bool DisableSound = false);
		void ScrollBottom();
		void ScrollTop();

		bool IsJustReleased(bool Keyboard, int Key, bool OverrideInput = false);
		bool IsJustPressed(bool Keyboard, int Key, bool OverrideInput = false);
		bool IsPressed(bool Keyboard, int Key, bool OverrideInput = false);

		bool IsOpenBindPressed(bool OverrideInput = false);
		bool IsAJustPressed(bool OverrideInput = false);
		bool IsBJustPressed(bool OverrideInput = false);
		bool IsOptionPressed(bool OverrideInput = false);
		bool IsLeftJustPressed(bool OverrideInput = false);
		bool IsRightJustPressed(bool OverrideInput = false);
		bool IsLeftPressed(bool OverrideInput = false);
		bool IsRightPressed(bool OverrideInput = false);

		WNDPROC GetWindowProcess() { return m_WindowProcess; }
		int m_LastKey;
	private:
		WNDPROC m_WindowProcess;
	};

	Input* GetInput();
}