#pragma once
#include "stdafx.hpp"
#include "options/submenu.hpp"
#include "types.hpp"
#include "options/core.hpp"

namespace Framework {
	struct header {
		float m_height = 0.098f, m_font_scale = 1.1f;
		int m_font = 7;

		Color m_color = { 255, 255, 255 };
		Color m_text_color = { 255, 255, 255 };
	};

	struct title {
		float m_height = 0.032f, m_font_scale = 0.28f;
		int m_font = (int)eFont::ChaletLondon;
		Math::Vector2<float> m_padding = { 2.05f, 2.05f };

		Color m_color = { 0, 0, 0, 255 };
		Color m_text_color = { 255, 255, 255, 255 };
	};

	struct option {
		float m_height = 0.032f, m_font_scale = 0.29f;
		int m_font = 0;
		Math::Vector2<float> m_padding = { 2.05f, 2.05f };


		Color m_color = { 0, 0, 0, 160 };
		Color m_text_color = { 220, 220, 220, 220 };
		Color m_selected_text_color = { 0, 0, 0, 255 };
	};

	struct footer {
		float m_height = 0.032f, m_font_scale = 0.28f;
		int m_font = (int)eFont::ChaletLondon;
		Math::Vector2<float> m_padding = { 2.05f, 2.05f };

		Color m_color = { 0,0,0,255 };
		Color m_text_color = { 255,255,255,255 };
	};

	class FrameWork {
	public:
		void Run();

		std::vector<std::unique_ptr<Framework::Options::AbstractSubmenu>>& GetAllSubmenus() { return m_AllSubmenus; }
		std::unordered_map<u32, Framework::Options::AbstractOption*>& GetAllOptions() { return m_AllOptions; }
		std::stack<Framework::Options::AbstractSubmenu*, std::vector<Framework::Options::AbstractSubmenu*>>& GetSubmenuStack() { return m_SubmenuStack; }
	public:
		bool m_Opened;
		bool m_Toggled;
		bool m_SmoothScroll = true;
		Math::Vector2<float> m_Position = { 0.13f, 0.05f };
		float m_DrawBaseY{};
		float m_Width = 0.21f;

		std::string tooltip;

		float test = 0.0935f;

		std::size_t m_MaxOptions = 12;

		float m_alpha = 0.0f; // Current alpha value for fade animation
		bool m_fadeIn = false; // Flag to indicate if fade-in animation is in progress


		float m_Target = { 0.f };
		float m_Current = { 0.f };
		float m_Speed = { 0.2281f };

		float m_fadespeed = { 0.4281f };

		float m_bg_target = { 0.f };
		float m_bg_current = { 0.f };

		Color m_ScrollerColor = { 255, 255, 255, 220 };

		std::string RenderQueue[100];
		int RenderQueueIndex = 0;

		header m_Header;
		title m_Title;
		option m_Option;
		footer m_Footer;
		float m_Wrap = -0.1f;

		bool m_color_opt = false;

		float m_delta = 0.f;
		float m_tooltip_x = 0.0985f;

		bool full_theme;

		std::int32_t m_open_delay = 200, m_close_delay = 300, m_back_delay = 300, m_enter_delay = 300, m_vertical_delay = 120, m_horizontal_delay = 110;
		bool m_open_key; bool m_back_key; bool m_enter_key; bool m_up_key; bool m_down_key; bool m_left_key; bool m_right_key;
		void ResetKeys();
		void CheckKeys();
		void HandleKeys();

		void UpdateScroller();
		void DrawScroller();

		//void RenderColorPreview(Framework::Options::AbstractOption* option);
		//void RenderTooltip();
		void DrawHeader();
		void DrawTitle(Framework::Options::AbstractSubmenu* sub);
		void DrawBackground(Framework::Options::AbstractSubmenu* sub);

		void RenderTooltip();

		bool m_hotkey;
		bool m_hotkey_pressed;

		//void drawOverlay();
		bool m_IsOverlayEnabled = true;
		float m_OverlayX = 0.11f;
		float m_OverlayY = 0.03f;
		float m_OverlayHeight = 0.035f;
		float m_OverlayLineHeight = 0.0035f;
		float m_OverlayPadding = 2.1f;
		float m_OverlayWidth = 0.20f;
		Color m_OverlayColor = Color(27, 28, 31, 220);
		Color m_OverlayTextColor = Color(255, 255, 255, 255);
		Color m_OverlayLineColor = m_Header.m_color;

		void DrawOption(Framework::Options::AbstractOption* option, bool selected);
		void DrawFooter();
		void DrawVehiclePreview(uint32_t hash);


		float test_x = 0.0260;
		float test_y = 0.120;

		void save_theme(const char* name);
		void load_theme(const char* name);
		void reset_theme();
		std::vector<std::string> m_cached_themes;

	public:
		std::vector<std::unique_ptr<Framework::Options::AbstractSubmenu>> m_AllSubmenus;
		std::stack<Framework::Options::AbstractSubmenu*, std::vector<Framework::Options::AbstractSubmenu*>> m_SubmenuStack;
		std::unordered_map<u32, Framework::Options::AbstractOption*> m_AllOptions;

		template <typename sub_type, typename ...TArgs>
		void AddSubmenu(TArgs&&... args) {
			auto sub = std::make_unique<sub_type>(std::forward<TArgs>(args)...);
			if (m_SubmenuStack.empty()) {
				m_SubmenuStack.push(sub.get());
			}
			m_AllSubmenus.push_back(std::move(sub));
		}

		void SwitchToSubmenu(std::uint32_t id) {
			for (auto&& sub : m_AllSubmenus) {
				if (sub->GetID() == id) {
					m_SubmenuStack.push(sub.get());
					return;
				}
			}
		}
	};

	inline FrameWork* GetFrameWork() {
		static FrameWork instance;
		return &instance;
	}

	inline void addSubmenu(const char* name, const char* target, std::function<void(Framework::Options::Core*)> action) {
		GetFrameWork()->AddSubmenu<Framework::Options::Core>(name, target, action);
	}

	inline void addPlayerSubmenu(uint32_t* player, const char* id, std::function<void(Framework::Options::PCore*)> action) {
		GetFrameWork()->AddSubmenu<Framework::Options::PCore>(player, id, action);
	}

	inline void addVehicleSubmenu(int* vehicle, uint32_t id, std::function<void(Framework::Options::VCore*)> action) {
		GetFrameWork()->AddSubmenu<Framework::Options::VCore>(vehicle, id, action);
	}

}
