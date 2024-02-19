#include "stdafx.hpp"
#include "panels.hpp"
#include "framework/render.hpp"
namespace Menu {

    void PanelManager::StartPanel(std::string title) {
        auto menu = Framework::GetFrameWork();

        g_panel_base_2 = g_panel_pos.m_Y;

        if (!m_NextTo) {
            g_panel_pos.m_X = menu->m_Position.m_X + (menu->m_Width / 2.f) + (m_Width + g_panel_spacing);
        }
        else {
            g_panel_pos.m_X = menu->m_Position.m_X + (menu->m_Width / 2.f) + (m_Width + g_panel_spacing) + m_Width;
            //g_panel_base = g_panel_base_2;
        }

        g_panel_base = g_panel_pos.m_Y;

        if (m_TitleHeight != 0) {
            g_panel_pos.m_Y = Framework::GetFrameWork()->m_Position.m_Y - 0.006f;
        }
        else {
            g_panel_pos.m_Y = Framework::GetFrameWork()->m_Position.m_Y - g_test_y;
        }

        if (m_TitleHeight != 0) {
            Framework::Render::DrawRect({ g_panel_pos.m_X, Framework::Render::GetRectBase(m_TitleHeight, g_panel_base + 0.01f) }, { m_Width, m_TitleHeight + 0.001f }, m_Background);
            Framework::Render::DrawRect({ g_panel_pos.m_X, Framework::Render::GetRectBase(m_TitleHeight, g_panel_base - 0.004f) }, { m_Width, 0.004f }, menu->m_Header.m_color);
            Framework::Render::DrawText(title.c_str(), eJustify::JUSTIFY_LEFT, { g_panel_pos.m_X, Framework::Render::GetRectBase(m_TitleHeight, g_panel_base + 0.013f) }, m_TitleFontScale, (int)m_TitleFont, m_Padding);
            g_panel_base += m_TitleHeight + 0.001;
        }
        else {
            Framework::Render::DrawRect({ g_panel_pos.m_X, Framework::Render::GetRectBase(m_TitleHeight, g_panel_base - 0.004f) }, { m_Width, 0.004f }, menu->m_Header.m_color);
            g_panel_base += 0.f;
        }
    }

    void PanelManager::Push(std::string left, std::string right) {
        if (m_TitleHeight != 0) {
            Framework::Render::DrawRect({ g_panel_pos.m_X, Framework::Render::GetRectBase(m_ItemHeight, g_panel_base + 0.01f) }, { m_Width, m_TitleHeight + 0.001f }, m_Background);
            Framework::Render::DrawText(left.c_str(), eJustify::JUSTIFY_LEFT, { g_panel_pos.m_X, Framework::Render::GetRectBase(m_ItemHeight, g_panel_base + 0.01f) }, m_FontScale, (int)m_ItemFont, m_Padding);
            Framework::Render::DrawText(right.c_str(), eJustify::JUSTIFY_RIGHT, { g_panel_pos.m_X + (m_Width / 2.f) - 0.003f, Framework::Render::GetRectBase(m_ItemHeight, g_panel_base + 0.01f) }, m_FontScale, (int)m_ItemFont, m_Padding);
            g_panel_base += m_ItemHeight + 0.001;
        }
        else {
            Framework::Render::DrawRect({ g_panel_pos.m_X, Framework::Render::GetRectBase(m_ItemHeight, g_panel_base - 0.002f) }, { m_Width, m_ItemHeight }, m_Background);
            Framework::Render::DrawText(left.c_str(), eJustify::JUSTIFY_LEFT, { g_panel_pos.m_X, Framework::Render::GetRectBase(m_ItemHeight, g_panel_base - 0.002f) }, m_FontScale, (int)m_ItemFont, m_Padding);
            Framework::Render::DrawText(right.c_str(), eJustify::JUSTIFY_RIGHT, { g_panel_pos.m_X + (m_Width / 2.f) - 0.003f, Framework::Render::GetRectBase(m_ItemHeight, g_panel_base - 0.002f) }, m_FontScale, (int)m_ItemFont, m_Padding);
            g_panel_base += m_ItemHeight;
        }
    }

    void PanelManager::PushSprite(std::pair<std::string, std::string> texture, Math::Vector2<float> scale) {
        Framework::Render::DrawRect({ g_panel_pos.m_X, Framework::Render::GetRectBase(scale.m_Y, g_panel_base) }, { m_Width, scale.m_Y + 0.004f }, m_Background);
        Framework::Render::DrawSprite(texture, { g_panel_pos.m_X, Framework::Render::GetRectBase(scale.m_Y, g_panel_base) }, { scale.m_X, scale.m_Y });
        g_panel_base += scale.m_Y + 0.004f;
    }

    void PanelManager::EndPanel() {
        g_panel_base += g_panel_spacing;
    }

    void PanelManager::SetNext() {
        m_NextTo = true;
    }


}