#include "stdafx.hpp"
#include "framework.hpp"
#include "rage/invoker/natives.hpp"
#include "render.hpp"
#include "types.hpp"
#include "core/core.hpp"
#include "cheat/util/panels.hpp"
#include "core/patterns.hpp"
#include "rage/engine.hpp"
#include "cheat/util/global.hpp"
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

	int GetIdFromName(const char* name) {
		for (std::uint32_t i = 0; i < 32; ++i) {
			if (!strcmp(Native::GetPlayerName(i), name))
				return i;
		}
		return 0;
	}

	void FrameWork::DrawOption(Framework::Options::AbstractOption* option, bool selected) {
		if (selected)
			DrawScroller();

		auto sub = m_SubmenuStack.top();

		Native::_Set2DLayer(4);

		auto y_pos = selected ? m_Current : Render::GetRectBase(m_Option.m_height);

		if (selected) {
			if (!strcmp(sub->GetName(), "Players")) {
			//	DrawPlayerInfo(GetIdFromName(option->GetLeftText()));
			}
		}


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

		if (option->GetFlag(eOptionFlag::KeyboardOption)) {
			auto size = Render::GetSpriteScale(0.028);
			Render::DrawText(option->GetLeftText(), JUSTIFY_LEFT, { m_Position.m_X, y_pos }, m_Option.m_font_scale, m_Option.m_font, m_Option.m_padding, selected ? m_Option.m_selected_text_color : m_Option.m_text_color);

			Render::DrawSprite({ "textures", "Edit" }, { m_Position.m_X + (m_Width / m_Option.m_padding.m_X - 0.004f),
				m_DrawBaseY + (m_Option.m_height / 2.f), }, size, selected ? m_Option.m_selected_text_color : m_Option.m_text_color, 0.f);

			Render::DrawRightText(option->GetRightText(), m_Position.m_X + (m_Width / m_Option.m_padding.m_X) - 0.012f,
				m_DrawBaseY + (m_Option.m_height / 2.f) - (Render::GetTextHeight(m_Option.m_font, m_Option.m_font_scale) / 1.5f) - 0.00007,
				m_Option.m_font_scale, m_Option.m_font, selected ? m_Option.m_selected_text_color : m_Option.m_text_color, false, false);
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

	void Text(const char* text, Color color, Math::Vector2<float> position, Math::Vector2<float> size, bool center)
	{
		Native::SetTextCentre(center);
		Native::SetTextColour(color.r, color.g, color.b, color.a);
		Native::SetTextFont(0);
		Native::SetTextScale(size.m_X, size.m_Y);
		Native::BeginTextCommandDisplayText("STRING");
		Native::AddTextComponentSubstringPlayerName((char*)text);
		Native::EndTextCommandDisplayText(position.m_X, position.m_Y, 0);


	}
	int GetHealth(Ped ped) {

		int health = (int)Native::GetEntityHealth(ped);
		int maxHealth = (int)Native::GetEntityMaxHealth(ped);

		//dont divide by 0
		if (health != 0) {
			int percentage = health * 100 / maxHealth;
			return percentage;
		}

		return 0;
	}
	int GetArmor(Ped ped, Player player) {
		int armor = (int)Native::GetPedArmour(ped);
		int maxArmor = (int)Native::GetPlayerMaxArmour(player);
		

		if (armor != 0) {
			int percentage = armor * 100 / maxArmor;
			return percentage;
		}

		return 0;
	}
	char* GetWanted(Player p)
	{
		char buffer[20];
		int Stars = (int)Native::GetPlayerWantedLevel(p);

		sprintf_s(buffer, "%i/5", Stars);


		return buffer;
	}

		//case 1:
			//Global_1845263[iVar0 /*877*/].f_205.f_26 = func_13934(joaat("MPPLY_KILL_DEATH_RATIO"));
			//Global_1845263[iVar0 /*877*/].f_205.f_24 = func_23535(170, -1);
			//Global_1845263[iVar0 /*877*/].f_205.f_25 = func_23535(172, -1);
			//Global_1845263[iVar0 /*877*/].f_205.f_28 = func_1951(joaat("MPPLY_KILLS_PLAYERS"));
			//Global_1845263[iVar0 /*877*/].f_205.f_29 = (func_1951(joaat("MPPLY_DEATHS_PLAYER")) + func_1951(joaat("MPPLY_DEATHS_PLAYER_SUICIDE")));
		//	*uParam0++;
		//	break;

		//case 2:
		//	Global_1845263[iVar0 /*877*/].f_205.f_15 = func_1951(joaat("MPPLY_TOTAL_RACES_WON"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_16 = func_1951(joaat("MPPLY_TOTAL_RACES_LOST"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_17 = func_1951(joaat("MPPLY_TIMES_FINISH_RACE_TOP_3"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_18 = func_1951(joaat("MPPLY_TIMES_FINISH_RACE_LAST"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_19 = func_1951(joaat("MPPLY_TIMES_RACE_BEST_LAP"));
		//	*uParam0++;
		//	break;

		//case 3:
		//	Global_1845263[iVar0 /*877*/].f_205.f_20 = func_1951(joaat("MPPLY_TOTAL_DEATHMATCH_WON"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_21 = func_1951(joaat("MPPLY_TOTAL_DEATHMATCH_LOST"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_22 = func_1951(joaat("MPPLY_TOTAL_TDEATHMATCH_WON"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_23 = func_1951(joaat("MPPLY_TOTAL_TDEATHMATCH_LOST"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_30 = func_1951(joaat("MPPLY_TIMES_FINISH_DM_TOP_3"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_31 = func_1951(joaat("MPPLY_TIMES_FINISH_DM_LAST"));
		//	*uParam0++;
		//	break;

		//case 4:
		//	Global_1845263[iVar0 /*877*/].f_205.f_32 = func_1951(joaat("MPPLY_DARTS_TOTAL_WINS"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_33 = func_1951(joaat("MPPLY_DARTS_TOTAL_MATCHES"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_34 = func_1951(joaat("MPPLY_ARMWRESTLING_TOTAL_WINS"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_35 = func_1951(joaat("MPPLY_ARMWRESTLING_TOTAL_MATCH"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_36 = func_1951(joaat("MPPLY_TENNIS_MATCHES_WON"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_37 = func_1951(joaat("MPPLY_TENNIS_MATCHES_LOST"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_38 = func_1951(joaat("MPPLY_BJ_WINS"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_39 = func_1951(joaat("MPPLY_BJ_LOST"));
		//	*uParam0++;
		//	break;

		//case 5:
		//	Global_1845263[iVar0 /*877*/].f_205.f_40 = func_1951(joaat("MPPLY_GOLF_WINS"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_41 = func_1951(joaat("MPPLY_GOLF_LOSSES"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_42 = func_1951(joaat("MPPLY_SHOOTINGRANGE_WINS"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_43 = func_1951(joaat("MPPLY_SHOOTINGRANGE_LOSSES"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_44 = func_23535(70, -1);
		//	Global_1845263[iVar0 /*877*/].f_205.f_47 = func_1951(joaat("MPPLY_HORDEWINS"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_48 = func_1951(joaat("MPPLY_CRHORDE"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_45 = func_1951(joaat("MPPLY_MCMWIN"));
		//	Global_1845263[iVar0 /*877*/].f_205.f_46 = func_1951(joaat("MPPLY_CRMISSION"));
		//	*uParam0++;
		//	break;

	int getPlayerRank(int player)
	{

		return *Menu::Global(1845263).At(player, 877).At(205).At(6).As<int*>();
	}
	int getPlayerCash(int player)
	{

		return *Menu::Global(1845263).At(player, 877).At(205).At(3).As<int*>();
	}
	int getPlayerWallet(int player)
	{

		return *Menu::Global(1845263).At(player, 877).At(205).At(56).As<int*>();
	}
	int getPlayerKD(int player)
	{

		return *Menu::Global(1845263).At(player, 877).At(205).At(26).As<int*>();
	}
	int kills(int player)
	{
		return *Menu::Global(1845263).At(player, 877).At(205).At(28).As<int*>();
	}

	int getPlayerAmmo(Ped ped, Hash WeaponHash)
	{
		int Ammo = (int)Native::GetAmmoInPedWeapon(ped, WeaponHash);
		if (Ammo == -1) {
			return (int)("Infinite");
		}
		else {
			return Ammo;
		}

	}
	float ItoF(int input)
	{
		float Output = *(float*)&input;
		return Output;
	}
	bool isNumericChar(char x)
	{
		return (x >= '0' && x <= '9') ? true : false;
	}
	int StoI(char* str)
	{
		if (*str == NULL)
			return 0;

		int res = 0,
			sign = 1,
			i = 0;

		if (str[0] == '-')
		{
			sign = -1;
			i++;
		}
		for (; str[i] != '\0'; ++i)
		{
			if (isNumericChar(str[i]) == false)
				return 0;
			res = res * 10 + str[i] - '0';
		}
		return sign * res;
	}
	char* ItoS(int num)
	{
		char buf[30];
		snprintf(buf, sizeof(buf), "%i", num);
		return buf;
	}
	char* FtoS(float input)
	{
		char returnvalue[64];
		int wholenumber = (int)input;
		input -= wholenumber;
		input *= 100;
		sprintf(returnvalue, "%d.%d", wholenumber, (int)input);
		return returnvalue;
	}
	const char* getPlayerVehicle(Ped ped)
	{
		bool inVehicle = Native::IsPedInAnyVehicle(ped, 0);

		if (inVehicle)
		{
			Hash vehicleHash = Native::GetEntityModel(Native::GetVehiclePedIsIn(ped, 0));
			const char* vehicleModel = Native::_GetLabelText(Native::GetDisplayNameFromVehicleModel(vehicleHash));

			return vehicleModel;
		}
		else {
			char result[16];
			sprintf_s(result, "%s", "None");
			return result;
		}

	}


	const char* InCutscene(int player)
	{
		if (Native::NetworkIsPlayerInMpCutscene(player))
		{
			return "Yes";
		}
		else {
			return "No";
		}
	}
	int GetSpeed(Ped ped) {

		int speed = (int)round(Native::GetEntitySpeed(ped) * 2.24);
		return speed;
	}
	bool is_player_freemode_host(std::int32_t player) {
		return (Native::NetworkGetHostOfScript("freemode", -1, 0) == player);
	}
	bool is_player_in_interior(Player player)
	{
		/*Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
		int currentInterior = INTERIOR::GET_INTERIOR_FROM_ENTITY(ped);
		return INTERIOR::IS_VALID_INTERIOR(currentInterior);*/
		int interior = Native::GetInteriorFromEntity(Native::GetPlayerPedScriptIndex(player));
		if (!interior) {
			interior = *Menu::Global(2689235).At(player, 453).At(248).As<int*>();
		}
		return interior;
	}

	void FrameWork::DrawPlayerInfo(Player _Player) {
		if (m_Opened) {
			Color m_white{ 255, 255, 255, 255 };
			float PosX = m_Position.m_X + 0.25f;

			float PosY = m_Position.m_Y + 0.168f;
			float LTextX = PosX - 0.1225;
			float RTextX = PosX + 0.009f;
			float TextY = m_Position.m_Y - 0.055;
			float SeperatorX = PosX + 0.05;

			float RTextX2 = PosX + 0.1215f;

			float rightresult = 0.49f;
			float righttext = SeperatorX - 0.048f;

			Ped ped = Native::GetPlayerPedScriptIndex(_Player);

			Render::DrawRect({ PosX, PosY + 0.0202f }, { 0.25f, 0.375f }, m_Option.m_color);//draw main info rect
			Render::DrawRect({ PosX, PosY - 0.0220f * 7.652f }, { 0.25f, 0.002f }, m_Header.m_color);//draw main info top bar 


			Render::DrawRect({ PosX + 0.195f, PosY + 0.0202f }, { 0.135f, 0.375f }, m_Option.m_color);//draw ped preview rect
			Render::DrawRect({ PosX + 0.195f, PosY - 0.0220f * 7.652f }, { 0.135f, 0.002f }, m_Header.m_color);//draw ped preview top bar 

			CNetGamePlayer* NetGamePlayer = Engine::GetNetworkGamePlayer(_Player);

			//std::string rank = std::to_string(getPlayerRank(_Player));
			//std::string cash = std::to_string(getPlayerCash(_Player));
			//std::string bank = std::to_string(getPlayerWallet(_Player));

			//float KD_Float = ItoF(getPlayerKD(_Player));
			//float kill_Float = ItoF(kills(_Player));
			//std::string kd = FtoS(KD_Float);
			//std::string kills = FtoS(kill_Float);

			Math::Vector3<float> get_coords = Native::GetEntityCoords(ped, 0);
			std::string wantedlvl = GetWanted(_Player);
			std::string ammo = std::format("{}", Native::GetAmmoInPedWeapon(ped, Native::GetSelectedPedWeapon(ped)));
			std::string coords = std::format("[{0},{1},{2}]", std::roundl(get_coords.m_X), std::roundl(get_coords.m_Y), std::roundl(get_coords.m_Z));



			Hash street[2]{};
			Native::GetStreetNameAtCoord(get_coords.m_X, get_coords.m_Y, get_coords.m_Z, &street[0], &street[1]);
			std::string Street = Native::GetStreetNameFromHashKey(street[0]);
			std::string Zone = Native::_GetLabelText(Native::GetNameOfZone(get_coords.m_X, get_coords.m_Y, get_coords.m_Z));
			std::string heading = std::format("{}", roundf(Native::GetEntityHeading(ped)));

			Math::Vector3<float> playerCoords = Native::GetEntityCoords(ped, false);
			Math::Vector3<float> selfCoords = Native::GetEntityCoords(Native::PlayerPedId(), false);
			float distance = Native::GetDistanceBetweenCoords(selfCoords.m_X, selfCoords.m_Y, selfCoords.m_Z, playerCoords.m_X, playerCoords.m_Y, playerCoords.m_Z, true);
			std::string Distance = std::format("{}m", roundf(distance));
			std::string Speed = std::to_string(GetSpeed(ped));
			std::string scid = std::to_string(NetGamePlayer->GetGamerInfo()->m_gamer_handle_2.m_rockstar_id);
			std::string slot = std::format("{}", NetGamePlayer->m_player_id);
			std::string public_ip = std::format("{0}.{1}.{2}.{3}", NetGamePlayer->GetGamerInfo()->m_internal_ip.m_field1, NetGamePlayer->GetGamerInfo()->m_internal_ip.m_field2, NetGamePlayer->GetGamerInfo()->m_internal_ip.m_field3, NetGamePlayer->GetGamerInfo()->m_internal_ip.m_field4);
			std::string local_ip = std::format("{0}.{1}.{2}.{3}", NetGamePlayer->GetGamerInfo()->m_external_ip.m_field1, NetGamePlayer->GetGamerInfo()->m_external_ip.m_field2, NetGamePlayer->GetGamerInfo()->m_external_ip.m_field3, NetGamePlayer->GetGamerInfo()->m_external_ip.m_field4);
			std::string token = std::to_string(NetGamePlayer->GetGamerInfo()->m_host_token);
			std::string public_port = std::to_string((uint16_t)NetGamePlayer->GetGamerInfo()->m_external_port);
			std::string local_port = std::to_string((uint16_t)NetGamePlayer->GetGamerInfo()->m_internal_port);
			std::string peer_id = std::to_string((uint64_t)NetGamePlayer->GetGamerInfo()->m_peer_id);
			std::string host = NetGamePlayer->is_host() ? std::format("Yes") : std::format("No");
			std::string scripthost = is_player_freemode_host(ped) ? "Yes" : "No";




			std::string passive = Native::_0x38D28DA81E4E9BF9(ped) ? "Yes" : "No";
			std::string rockstar = Native::NetworkPlayerIsRockstarDev(ped) ? "Yes" : "No";

			Text("Rank", { m_white }, { LTextX, TextY + 0.06f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText("100", SeperatorX - 0.0523f, TextY + 0.06f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			Render::DrawRect({ SeperatorX - 0.05f, TextY + 0.07f }, { 0.001f, 0.015f }, m_white);
			Text("Health", { m_white }, { SeperatorX - 0.048f, TextY + 0.06f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText("100", RTextX2, TextY + 0.06f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Text("Cash", { m_white }, { LTextX, TextY + 0.085f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText("2147483647", SeperatorX - 0.0523f, TextY + 0.085f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			Render::DrawRect({ SeperatorX - 0.05f, TextY + 0.095f }, { 0.001f, 0.015f }, m_white);
			Text("Armor", { m_white }, { SeperatorX - 0.048f, TextY + 0.085f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText("100", RTextX2, TextY + 0.085f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Text("Bank", { m_white }, { LTextX, TextY + 0.11f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText("2147483647", SeperatorX - 0.0523f, TextY + 0.11f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			Render::DrawRect({ SeperatorX - 0.05f, TextY + 0.12f }, { 0.001f, 0.015f }, m_white);
			Text("Wanted Level", { m_white }, { SeperatorX - 0.048f, TextY + 0.11f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(wantedlvl.c_str(), RTextX2, TextY + 0.11f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Text("K/D Ratio", { m_white }, { LTextX, TextY + 0.135f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText("1.50", SeperatorX - 0.0523f, TextY + 0.135f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			Render::DrawRect({ SeperatorX - 0.05f, TextY + 0.145f }, { 0.001f, 0.015f }, m_white);
			Text("Ammo", { m_white }, { SeperatorX - 0.048f, TextY + 0.135f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(ammo.c_str(), RTextX2, TextY + 0.135f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Text("Weapon", { m_white }, { LTextX, TextY + 0.16f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText("~c~None", RTextX2, TextY + 0.16f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Text("Zone", { m_white }, { LTextX, TextY + 0.185f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(Zone.c_str(), RTextX2, TextY + 0.185, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Text("Street", { m_white }, { LTextX, TextY + 0.21f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(Street.c_str(), RTextX2, TextY + 0.21, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);


			Text("Coords", { m_white }, { LTextX, TextY + 0.235f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(coords.c_str(), SeperatorX - 0.0523f, TextY + 0.235f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			Render::DrawRect({ SeperatorX - 0.05f, TextY + 0.245f }, { 0.001f, 0.015f }, m_white);
			Text("Heading", { m_white }, { SeperatorX - 0.048f, TextY + 0.235f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(heading.c_str(), RTextX2, TextY + 0.235f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Text("Distance", { m_white }, { LTextX, TextY + 0.26f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(Distance.c_str(), SeperatorX - 0.0523f, TextY + 0.26f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			Render::DrawRect({ SeperatorX - 0.05f, TextY + 0.27f }, { 0.001f, 0.015f }, m_white);
			Text("Speed", { m_white }, { SeperatorX - 0.048f, TextY + 0.26f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(Speed.c_str(), RTextX2, TextY + 0.26f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Text("Public Ip", { m_white }, { LTextX, TextY + 0.285f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(public_ip.c_str(), SeperatorX - 0.0523f, TextY + 0.285f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			Render::DrawRect({ SeperatorX - 0.05f, TextY + 0.295f }, { 0.001f, 0.015f }, m_white);
			Text("Local Ip", { m_white }, { SeperatorX - 0.048f, TextY + 0.285f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(local_ip.c_str(), RTextX2, TextY + 0.285f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Text("Public Port", { m_white }, { LTextX, TextY + 0.31f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(public_port.c_str(), SeperatorX - 0.0523f, TextY + 0.31f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			Render::DrawRect({ SeperatorX - 0.05f, TextY + 0.32f }, { 0.001f, 0.015f }, m_white);
			Text("Local Port", { m_white }, { SeperatorX - 0.048f, TextY + 0.31f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(local_port.c_str(), RTextX2, TextY + 0.31f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Text("Host", { m_white }, { LTextX, TextY + 0.335f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(host.c_str(), SeperatorX - 0.0523f, TextY + 0.335f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			Render::DrawRect({ SeperatorX - 0.05f, TextY + 0.345f }, { 0.001f, 0.015f }, m_white);
			Text("Script Host", { m_white }, { SeperatorX - 0.048f, TextY + 0.335f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(scripthost.c_str(), RTextX2, TextY + 0.335f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Text("Peer", { m_white }, { LTextX, TextY + 0.36f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(peer_id.c_str(), SeperatorX - 0.0523f, TextY + 0.36f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			Render::DrawRect({ SeperatorX - 0.05f, TextY + 0.37f }, { 0.001f, 0.015f }, m_white);
			Text("Token", { m_white }, { SeperatorX - 0.048f, TextY + 0.36f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(token.c_str(), RTextX2, TextY + 0.36f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			if (IsValidPtr(NetGamePlayer->GetGamerInfo()->m_gamer_handle_2.m_rockstar_id))
			{
				Text("Scid", { m_white }, { LTextX, TextY + 0.385f }, { 0.23f, 0.23f }, false);
				Render::DrawRightText(scid.c_str(), SeperatorX - 0.0523f, TextY + 0.385f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			}

			Render::DrawRect({ SeperatorX - 0.05f, TextY + 0.395f }, { 0.001f, 0.015f }, m_white);
			Text("Slot", { m_white }, { SeperatorX - 0.048f, TextY + 0.385f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(slot.c_str(), RTextX2, TextY + 0.385f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Text("Passive", { m_white }, { LTextX, TextY + 0.41f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(passive.c_str(), SeperatorX - 0.0523f, TextY + 0.41f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);
			Render::DrawRect({ SeperatorX - 0.05f, TextY + 0.42f }, { 0.001f, 0.015f }, m_white);
			Text("Rockstar", { m_white }, { SeperatorX - 0.048f, TextY + 0.41f }, { 0.23f, 0.23f }, false);
			Render::DrawRightText(rockstar.c_str(), RTextX2, TextY + 0.41f, 0.23f, (int)eFont::ChaletLondon, m_white, 0, 0);

			Rage::joaat_t SceneHash = 0x390DCCF5;//0xAD197067, 0x390DCCF5, 0x3D8F5C29, 0x5ADFAFD0
			Rage::joaat_t Element = 0;
			CPed* LocalPed = NetGamePlayer->m_player_info->m_ped;
			uintptr_t DrawManager = *(uint64_t*)(Patterns::Vars::g_UiDrawManager);

			Math::Vector3<float> pos = { PosX - 0.56f, PosY * 7.5f, 0.0f };

			if (DrawManager) {

				DWORD* preset2 = Caller::Call<DWORD*>(Patterns::Vars::g_GetScenePreset, DrawManager, &SceneHash);
				DWORD* preset = preset2 + (0x2A0 * Element);

				if (preset) {

					*(float*)(preset + 0x12) = 0;
					*(float*)(preset + 0x9) = -3.8f;//size
				}
				if (Caller::Call<bool>(Patterns::Vars::g_PushScenePresetManager, DrawManager, &SceneHash)) {
					if (Caller::Call<bool>(Patterns::Vars::g_AddElementToScene, DrawManager, &SceneHash, Element, LocalPed, pos, 1.0f)) {
						Caller::Call<bool>(Patterns::Vars::g_SetSceneElementLighting, DrawManager, &SceneHash, Element, 1.6f);
					}
				}
			}

		}
	}
}