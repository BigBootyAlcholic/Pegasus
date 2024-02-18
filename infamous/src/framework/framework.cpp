#include "stdafx.hpp"
#include "framework.hpp"
#include "rage/invoker/natives.hpp"
#include "render.hpp"
#include "types.hpp"
#include "core/core.hpp"
#include "cheat/util/panels.hpp"
namespace Framework {

	bool IsKeyPressed(std::uint16_t key) {
		if (GetForegroundWindow() == Core::g_GameWindow) {
			if (GetAsyncKeyState(key) & 0x8000) {
				return true;
			}
		}
		return false;
	}

	void FrameWork::Run() {
		CheckKeys();
		HandleKeys();
		//UpdateLastKeyPressed();

		if (m_Opened) {
			Native::DisableControlAction(0, 27, true);
			Native::ClearHelp(true);
			Native::ClearAllHelpMessages();

			m_DrawBaseY = m_Position.m_Y;

			if (!m_SubmenuStack.empty()) {
				auto sub = m_SubmenuStack.top();
				sub->Reset();
				sub->Execute();

				DrawHeader();
				DrawTitle(sub);
				DrawBackground(sub);

				if (sub->GetOptionsSize() != 0) {
					std::size_t startPoint = 0;
					std::size_t endPoint = sub->GetOptionsSize() > m_MaxOptions ? m_MaxOptions : sub->GetOptionsSize();
					if (sub->GetOptionsSize() > m_MaxOptions && sub->GetSelectedOption() >= m_MaxOptions) {
						startPoint = sub->GetSelectedOption() - m_MaxOptions + 1;
						endPoint = sub->GetSelectedOption() + 1;
					}
					for (std::size_t i = startPoint, j = 0; i < endPoint; ++i, ++j) {
						DrawOption(sub->GetOption(i), i == sub->GetSelectedOption());
					}
				}
				UpdateScroller();
				DrawFooter();
				RenderTooltip();
			}
		}
	}

	void FrameWork::UpdateScroller() {
		if (m_Current != m_Target)
			m_Current = m_Current * (1 - m_Speed) + m_Target * m_Speed;
		if ((m_Current > m_Target - 0.0005) && (m_Current < m_Target + 0.0005))
			m_Current = m_Target;
	}

	void FrameWork::ResetKeys() {
		m_open_key = false, m_back_key = false, m_enter_key = false, m_up_key = false; m_down_key = false,
			m_left_key = false, m_right_key = false; m_hotkey = false;
	}

	void FrameWork::CheckKeys() {
		ResetKeys();

		m_open_key = IsKeyPressed(VK_F4) || (Native::IsDisabledControlPressed(0, ControlScriptRB) && Native::IsDisabledControlPressed(0, ControlFrontendX)), m_back_key = IsKeyPressed(VK_BACK) || Native::IsDisabledControlPressed(0, ControlFrontendCancel), m_enter_key = IsKeyPressed(VK_RETURN) || Native::IsDisabledControlPressed(0, ControlFrontendAccept),
			m_up_key = IsKeyPressed(VK_UP) || Native::IsDisabledControlPressed(0, ControlFrontendUp), m_down_key = IsKeyPressed(VK_DOWN) || Native::IsDisabledControlPressed(0, ControlFrontendDown), m_left_key = IsKeyPressed(VK_LEFT) || Native::IsDisabledControlPressed(0, ControlFrontendLeft),
			m_right_key = IsKeyPressed(VK_RIGHT) || Native::IsDisabledControlPressed(0, ControlFrontendRight); m_hotkey = IsKeyPressed(VK_F8);
	}

	void FrameWork::HandleKeys() {
		static auto openTicker = GetTickCount();
		if (m_open_key && GetTickCount() - openTicker >= static_cast<std::uint32_t>(m_open_delay)) {
			openTicker = GetTickCount();
			m_Opened ^= true;

			Native::PlaySoundFrontend(-1, m_Opened ? "SELECT" : "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
		}

		static Timer backTimer(0ms);
		backTimer.SetDelay(std::chrono::milliseconds(m_back_delay));
		if (m_Opened && m_back_key && backTimer.Update()) {
			Native::PlaySoundFrontend(-1, "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);

			if (m_SubmenuStack.size() <= 1) {
				m_Opened = false;
			}
			else {
				m_SubmenuStack.pop();
			}
		}

		static Timer hotkeyTimer(0ms);
		hotkeyTimer.SetDelay(std::chrono::milliseconds(m_back_delay));
		if (m_Opened && m_hotkey && hotkeyTimer.Update()) {
			Native::PlaySoundFrontend(-1, "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
			m_hotkey_pressed = !m_hotkey_pressed;
		}

		if (m_Opened && !m_SubmenuStack.empty()) {
			auto sub = m_SubmenuStack.top();

			static Timer enterTimer(0ms);
			enterTimer.SetDelay(std::chrono::milliseconds(m_enter_delay));
			if (m_enter_key && sub->GetOptionsSize() != 0 && enterTimer.Update()) {
				Native::PlaySoundFrontend(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
				if (auto opt = sub->GetOption(sub->GetSelectedOption())) {
					opt->HandleAction(eOptionAction::click);
				}
			}

			static Timer upTimer(0ms);
			upTimer.SetDelay(std::chrono::milliseconds(m_vertical_delay));
			if (m_up_key && sub->GetOptionsSize() != 0 && upTimer.Update()) {
				Native::PlaySoundFrontend(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
				sub->ScrollBackward();
			}

			static Timer downTimer(0ms);
			downTimer.SetDelay(std::chrono::milliseconds(m_vertical_delay));
			if (m_down_key && sub->GetOptionsSize() != 0 && downTimer.Update()) {
				Native::PlaySoundFrontend(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
				sub->ScrollForward();
			}

			static Timer leftTimer(0ms);
			leftTimer.SetDelay(std::chrono::milliseconds(m_horizontal_delay));
			if (m_left_key && sub->GetOptionsSize() != 0 && leftTimer.Update()) {
				Native::PlaySoundFrontend(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
				if (auto opt = sub->GetOption(sub->GetSelectedOption())) {
					opt->HandleAction(eOptionAction::left_click);
				}
			}

			static Timer rightTimer(0ms);
			rightTimer.SetDelay(std::chrono::milliseconds(m_horizontal_delay));
			if (m_right_key && sub->GetOptionsSize() != 0 && rightTimer.Update()) {
				Native::PlaySoundFrontend(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
				if (auto opt = sub->GetOption(sub->GetSelectedOption())) {
					opt->HandleAction(eOptionAction::right_click);
				}
			}
		}
	}

	void FrameWork::DrawScroller() {
		m_Target = Render::GetRectBase(m_Option.m_height);
		Native::_Set2DLayer(2);

		if (m_SmoothScroll)
			Render::DrawSprite({ "texturesss", "scroller" }, { m_Position.m_X, m_Current }, { m_Width, m_Option.m_height }, m_ScrollerColor, 0.f);
		else
			Render::DrawSprite({ "texturesss", "scroller" }, { m_Position.m_X, Render::GetRectBase(m_Option.m_height) }, { m_Width, m_Option.m_height }, m_ScrollerColor, 0.f);
	}

	void FrameWork::DrawHeader() {
		Native::_Set2DLayer(1);

		Render::DrawSprite({ "textures", "vvs" }, { m_Position.m_X,  Render::GetRectBase(m_Header.m_height) }, { m_Width, m_Header.m_height }, m_Header.m_color, 0.f);
		//Render::DrawText("Pegasus", JUSTIFY_CENTER, { m_Position.m_X, Render::GetRectBase(m_Header.m_height) }, m_Header.m_font_scale, m_Header.m_font, { 2.1f, 2.1f }, m_Header.m_text_color, true);

		m_DrawBaseY += m_Header.m_height;
	}

	void FrameWork::DrawTitle(Framework::Options::AbstractSubmenu* sub) {
		Native::_Set2DLayer(1);



		Render::DrawRect({ m_Position.m_X, Render::GetRectBase(m_Title.m_height) }, { m_Width, m_Title.m_height }, m_Title.m_color);
		//Render::DrawSprite({ "textures", "list_header" }, { m_Position.m_X, Render::GetRectBase(m_Title.m_height) }, { m_Width, m_Title.m_height }, m_Title.m_color);

		Render::DrawText(sub->GetName(), JUSTIFY_LEFT, {m_Position.m_X, Render::GetRectBase(m_Title.m_height)}, m_Title.m_font_scale, m_Title.m_font, m_Title.m_padding);
		char rightText[32] = {};
		std::snprintf(rightText, sizeof(rightText) - 1, "%zu / %zu", sub->GetSelectedOption() + 1, sub->GetOptionsSize());


		Render::DrawText(rightText, JUSTIFY_RIGHT, { m_Position.m_X + (m_Width / m_Title.m_padding.m_X), Render::GetRectBase(m_Title.m_height) }, m_Title.m_font_scale, m_Title.m_font, m_Title.m_padding);


		m_DrawBaseY += m_Title.m_height;
	}

	void FrameWork::DrawBackground(Framework::Options::AbstractSubmenu* sub) {
		Native::_Set2DLayer(1);

		int visible_options = sub->GetOptionsSize() > m_MaxOptions ? m_MaxOptions : sub->GetOptionsSize();

		float bg_height = m_Option.m_height * visible_options;
		m_bg_target = bg_height;

		if (m_bg_current != m_bg_target)
			m_bg_current = m_bg_current * (1 - m_Speed) + m_bg_target * m_Speed;
		if ((m_bg_current > m_bg_target - 0.0005) && (m_bg_current < m_bg_target + 0.0005))
			m_bg_current = m_bg_target;

		Render::DrawRect({ m_Position.m_X, m_DrawBaseY + (m_bg_current / 2.f) }, { m_Width, m_bg_current }, m_Option.m_color);
	}

	void FrameWork::DrawOption(Framework::Options::AbstractOption* option, bool selected) {
		if (selected)
			DrawScroller();

		auto sub = m_SubmenuStack.top();

		Native::_Set2DLayer(4);

		auto y_pos = selected ? m_Current : Render::GetRectBase(m_Option.m_height);

		if (option->GetFlag(eOptionFlag::SimpleOption)) {
			Render::DrawText(option->GetLeftText(), JUSTIFY_LEFT, { m_Position.m_X, y_pos }, m_Option.m_font_scale, m_Option.m_font, m_Option.m_padding, selected ? m_Option.m_selected_text_color : m_Option.m_text_color);
			Render::DrawText(option->GetRightText(), JUSTIFY_RIGHT, { m_Position.m_X + (m_Width / m_Option.m_padding.m_X), y_pos }, m_Option.m_font_scale, m_Option.m_font, m_Option.m_padding, selected ? m_Option.m_selected_text_color : m_Option.m_text_color);
		}

		if (option->GetFlag(eOptionFlag::VehicleOption)) {
			Render::DrawText(option->GetLeftText(), JUSTIFY_LEFT, { m_Position.m_X, y_pos }, m_Option.m_font_scale, m_Option.m_font, m_Option.m_padding, selected ? m_Option.m_selected_text_color : m_Option.m_text_color);
			if (selected) {
				Menu::VehicleInfoPanel(Native::GetHashKey(option->GetLeftText()));
			}
		}

		if (option->GetFlag(eOptionFlag::Indicator)) {
			auto size = Render::GetSpriteScale(0.02);
			Render::DrawText(option->GetLeftText(), JUSTIFY_LEFT, { m_Position.m_X, y_pos }, m_Option.m_font_scale, m_Option.m_font, m_Option.m_padding, selected ? m_Option.m_selected_text_color : m_Option.m_text_color);
			Render::DrawRightText(">", m_Position.m_X + (m_Width / m_Option.m_padding.m_X), m_DrawBaseY + (m_Option.m_height / 2.f)
				- (Render::GetTextHeight(2, 0.4) / 1.725f), 0.4, 2,
				selected ? m_Option.m_selected_text_color : m_Option.m_text_color, false, false);

		
		}

		if (option->GetFlag(eOptionFlag::ToggleOption)) {
			Render::DrawText(option->GetLeftText(), JUSTIFY_LEFT, { m_Position.m_X, y_pos }, m_Option.m_font_scale, m_Option.m_font, m_Option.m_padding, selected ? m_Option.m_selected_text_color : m_Option.m_text_color);

			auto size = Render::GetSpriteScale(0.028);
			if (m_Toggled) {
				Render::DrawSprite({ "textures", "Toggle On" }, { m_Position.m_X + (m_Width / m_Option.m_padding.m_X - 0.0045f),
					m_DrawBaseY + (m_Option.m_height / 2.f), }, { size.m_X, size.m_Y }, selected ? m_Option.m_selected_text_color : m_Option.m_text_color, 0.0f);
			}
			else {
				Render::DrawSprite({ "textures", "Toggle Off" }, { m_Position.m_X + (m_Width / m_Option.m_padding.m_X - 0.0045f),
					m_DrawBaseY + (m_Option.m_height / 2.f), }, { size.m_X, size.m_Y }, selected ? m_Option.m_selected_text_color : m_Option.m_text_color, 0.0f);
			}
		}

		if (option->GetFlag(eOptionFlag::NumberOption)) {
			Render::DrawText(option->GetLeftText(), JUSTIFY_LEFT, { m_Position.m_X, y_pos }, m_Option.m_font_scale, m_Option.m_font, m_Option.m_padding, selected ? m_Option.m_selected_text_color : m_Option.m_text_color);
			Render::DrawRightText(option->GetRightText(), m_Position.m_X + (m_Width / m_Option.m_padding.m_X),
				m_DrawBaseY + (m_Option.m_height / 2.f) - (Render::GetTextHeight(m_Option.m_font, m_Option.m_font_scale) / 1.5f) - 0.00007,
				m_Option.m_font_scale, m_Option.m_font, selected ? m_Option.m_selected_text_color : m_Option.m_text_color, false, false);
		}

		if (option->GetFlag(eOptionFlag::ToggleNumberOption)) {
			Render::DrawText(option->GetLeftText(), JUSTIFY_LEFT, { m_Position.m_X, y_pos }, m_Option.m_font_scale, m_Option.m_font, m_Option.m_padding, selected ? m_Option.m_selected_text_color : m_Option.m_text_color);
			auto size = Render::GetSpriteScale(0.028);
			if (m_Toggled) {
				Render::DrawSprite({ "textures", "Toggle On" }, { m_Position.m_X + (m_Width / m_Option.m_padding.m_X - 0.0045f),
					m_DrawBaseY + (m_Option.m_height / 2.f), }, { size.m_X, size.m_Y }, selected ? m_Option.m_selected_text_color : m_Option.m_text_color, 0.0f);
			}
			else {
				Render::DrawSprite({ "textures", "Toggle Off" }, { m_Position.m_X + (m_Width / m_Option.m_padding.m_X - 0.0045f),
					m_DrawBaseY + (m_Option.m_height / 2.f), }, { size.m_X, size.m_Y }, selected ? m_Option.m_selected_text_color : m_Option.m_text_color, 0.0f);
			}

			Render::DrawText(option->GetRightText(), JUSTIFY_RIGHT, { m_Position.m_X + test, y_pos }, m_Option.m_font_scale, m_Option.m_font, { m_Option.m_padding.m_X - test, m_Option.m_padding.m_Y }, selected ? m_Option.m_selected_text_color : m_Option.m_text_color);
		}

		if (option->GetFlag(eOptionFlag::BreakOption)) {
			if (m_down_key) {
				while (sub->GetOption(sub->GetSelectedOption())->GetFlag(eOptionFlag::BreakOption)) {
					sub->SetSelectedOption(sub->GetSelectedOption() + 1);
				}
			}
			else if (m_up_key) {
				while (sub->GetOption(sub->GetSelectedOption())->GetFlag(eOptionFlag::BreakOption)) {
					sub->SetSelectedOption(sub->GetSelectedOption() - 1);
				}
			}

			Render::DrawText(option->GetLeftText(), JUSTIFY_CENTER, { m_Position.m_X, m_DrawBaseY + (m_Option.m_height / 2.f) }, m_Option.m_font_scale, m_Option.m_font, m_Option.m_padding, m_Option.m_text_color);
		}

		if (option->GetFlag(eOptionFlag::ScrollOption)) {
			Render::DrawText(option->GetLeftText(), JUSTIFY_LEFT, { m_Position.m_X, y_pos }, m_Option.m_font_scale, m_Option.m_font, m_Option.m_padding, selected ? m_Option.m_selected_text_color : m_Option.m_text_color);

			Render::DrawRightText(option->GetRightText(), m_Position.m_X + (m_Width / m_Option.m_padding.m_X),
				m_DrawBaseY + (m_Option.m_height / 2.f) - (Render::GetTextHeight(m_Option.m_font, m_Option.m_font_scale) / 1.5f) - 0.00007,
				m_Option.m_font_scale, m_Option.m_font, selected ? m_Option.m_selected_text_color : m_Option.m_text_color, false, false);
		}


		if (option->GetFlag(eOptionFlag::NumberOption)) {
			Render::DrawText(option->GetLeftText(), JUSTIFY_LEFT, { m_Position.m_X, y_pos }, m_Option.m_font_scale, m_Option.m_font, m_Option.m_padding, selected ? m_Option.m_selected_text_color : m_Option.m_text_color);

			Render::DrawRightText(option->GetRightText(), m_Position.m_X + (m_Width / m_Option.m_padding.m_X),
				m_DrawBaseY + (m_Option.m_height / 2.f) - (Render::GetTextHeight(m_Option.m_font, m_Option.m_font_scale) / 1.5f) - 0.00007,
				m_Option.m_font_scale, m_Option.m_font, selected ? m_Option.m_selected_text_color : m_Option.m_text_color, false, false);
		}

		if (sub->GetSelectedOption() >= sub->GetOptionsSize())
			sub->SetSelectedOption(sub->GetOptionsSize() - 1);

		m_DrawBaseY += m_Option.m_height;
	}

	void FrameWork::DrawFooter() {

		Render::DrawSprite({ "texturesrr", "footer" }, { m_Position.m_X, Render::GetRectBase(m_Footer.m_height, m_Position.m_Y + m_Header.m_height + m_Title.m_height + m_bg_current) }, { m_Width, m_Footer.m_height }, m_Footer.m_color, 0.f); // so the footer animates like the option bg

		//render::draw_sprite({ "textures", "footer" }, { m_position.x,  m_draw_base_y + (m_footer.m_height / 2.f) }, { m_width, m_footer.m_height }, m_footer.m_color, 0.f);
		float size = 0.02f;
		auto sizee = Render::GetSpriteScale(size);

		//Render::DrawText("1.0", JUSTIFY_RIGHT, { m_Position.m_X + (m_Width / m_Title.m_padding.m_X), Render::GetRectBase(m_Footer.m_height, m_Position.m_Y + m_Header.m_height + m_Title.m_height + m_bg_current) }, m_Title.m_font_scale, m_Title.m_font, m_Title.m_padding);
		Render::DrawSprite({ "commonmenu", "shop_arrows_upanddown" }, { m_Position.m_X, Render::GetRectBase(m_Footer.m_height, m_Position.m_Y + m_Header.m_height + m_Title.m_height + m_bg_current) }, { 0.0215f, 0.0315f }, { 255, 255, 255, 255 }, 0.f);

		m_DrawBaseY += m_Footer.m_height;
	}

	void FrameWork::RenderTooltip() {
		if (!m_SubmenuStack.empty()) {
			auto sub = m_SubmenuStack.top();
			if (sub->GetOptionsSize()) {
				if (auto opt = sub->GetOption(sub->GetSelectedOption())) {
					tooltip = opt->GetDescription();
				}
			}
		}

		if (!tooltip.c_str() || !*tooltip.c_str())
			return;

		m_DrawBaseY += 0.001f;
		float y = 0.f;
		y += m_DrawBaseY;

		float scaled_body_height = Render::GetNormalizedFontScale(m_Option.m_font, m_Option.m_font_scale);
		Native::SetTextFont(m_Option.m_font);
		Native::SetTextScale(0.f, scaled_body_height);
		Native::SetTextWrap(m_Position.m_X - (m_Width / 2.f) + 0.004f, (1.0f - (1.0f - (m_Position.m_X + 0.1575f - (0.23f - m_Option.m_font_scale)) - m_Wrap)));
		Native::_BeginTextCommandLineCount("STRING");
		Native::AddTextComponentSubstringPlayerName(tooltip.c_str());

		float height = m_Option.m_height;

		int lines = Native::_EndTextCommandGetLineCount( m_Position.m_X - (m_Width / 2.f) + 0.004f, y + 0.005f );
		if (lines > 1) {
			height = (lines * Native::_GetTextScaleHeight(scaled_body_height, m_Option.m_font)) + (0.005f * lines) + 0.005f;
		}

		std::pair<std::string, std::string> texture = { "texturesrrr", "scrollerrr" };
		Render::DrawSpriteAligned(texture, { m_Position.m_X - (m_Width / 2.f), y }, { m_Width, height }, 0.f, m_Option.m_color);
		Render::DrawText2(tooltip.c_str(), { m_Position.m_X - (m_Width / 2.f) + 0.004f, y + 0.005f }, scaled_body_height, m_Option.m_font, { 255, 255, 255, 255 }, JUSTIFY_LEFT, { m_Position.m_X - (m_Width / 2.f) + 0.004f, (1.0f - (1.0f - (m_Position.m_X + 0.1575f - (0.23f - m_Option.m_font_scale)) - m_Wrap)) });

		m_DrawBaseY += m_Option.m_height;
	}
}