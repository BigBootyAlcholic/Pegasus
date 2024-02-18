#pragma once
#include "stdafx.hpp"
#include "rage/invoker/natives.hpp"
#include "framework.hpp"
#include "util/math.hpp"
#include "types.hpp"
namespace Framework::Render {
	inline float GetAccurateFontScale(int font, float scale) {
		switch (font) {
		case 0: return (scale * 1.0f);
		case 1: return (scale * 1.3f);
		case 2: return (scale * 1.11f);
		case 4: return (scale * 1.11f);
		case 7: return (scale * 1.29f);
		}
		return scale;
	}

	inline float GetTextWidth(std::string string, int font, float scale) {
		Native::_BeginTextCommandWidth("STRING");
		Native::AddTextComponentSubstringPlayerName(string.c_str());
		Native::SetTextScale(0.f, scale);
		return Native::_EndTextCommandGetWidth(font);
	}

	inline float GetTextHeight(int font, float size) {
		return Native::_GetTextScaleHeight(size, font);
	}

	inline void DrawRect(Math::Vector2<float> position, Math::Vector2<float> scale, Color color, bool alligned = false) {
		if (alligned) {
			Native::DrawRect({ position.m_X + (scale.m_X * 0.5f), position.m_Y + (scale.m_Y * 0.5f) }, scale.m_X, scale.m_Y, color.r, color.g, color.b, color.a, 0);
		}
		else {
			Native::DrawRect({ position.m_X, position.m_Y }, scale.m_X, scale.m_Y, color.r, color.g, color.b, color.a, 0);
		}
	}

	inline float GetRectBase(float rect_height, float draw_base = Framework::GetFrameWork()->m_DrawBaseY) {
		return draw_base + (rect_height / 2.f);
	}

	inline void DrawText(const char* text, eJustify justify, Math::Vector2<float> position, float size, int font, Math::Vector2<float> padding, Color color = { 255, 255, 255, 255 }, bool half = false, bool outline = false, bool shadow = false) {
		if (justify == eJustify::JUSTIFY_CENTER) {
			Native::SetTextCentre(true);
			position.m_X = position.m_X;
		}
		if (justify == eJustify::JUSTIFY_RIGHT) {
			Native::SetTextWrap(0.f, position.m_X);
			Native::SetTextRightJustify(true);
			position.m_X = position.m_X + (Framework::GetFrameWork()->m_Width / padding.m_Y);
		}
		if (justify == eJustify::JUSTIFY_LEFT) {
			position.m_X = position.m_X - (Framework::GetFrameWork()->m_Width / padding.m_X);
		}
		if (half) {
			position.m_Y -= GetTextHeight(font, size) / 2.f;
		}
		else {
			position.m_Y -= GetTextHeight(font, size) / 1.5f;
		}
		Native::SetTextScale(size, size);
		Native::SetTextFont(static_cast<int>(font));
		Native::SetTextColour(color.r, color.g, color.b, color.a);
		if (outline)
			Native::SetTextOutline();
		if (shadow)
			Native::SetTextDropShadow();

		/*	if (text.length() >= 98) {
				renderer::getRenderer()->render_queue[renderer::getRenderer()->render_queue_index] = text;
				renderer::getRenderer()->render_queue_index++;
				renderer::getRenderer()->render_queue_index %= 100;
			}*/

		Native::BeginTextCommandDisplayText("STRING");
		Native::AddTextComponentSubstringPlayerName(text);
		Native::EndTextCommandDisplayText(position.m_X, position.m_Y, 0);
	}

	inline void DrawText2(std::string text, Math::Vector2<float> position, float scale, int font, Color color, eJustify justification = JUSTIFY_LEFT, Math::Vector2<float> wrap = { 0.f, 5.f }) {
		Native::SetTextWrap(wrap.m_X, wrap.m_Y);

		if (justification != JUSTIFY_LEFT) {
			//  HUD::SET_TEXT_JUSTIFICATION(justification = JUSTIFY_CENTER ? 0 : 2);
		}

		Native::SetTextCentre(justification == JUSTIFY_CENTER);
		Native::SetTextScale(0.f, scale);
		Native::SetTextColour(color.r, color.g, color.b, color.a);
		Native::SetTextFont(font);

		if (text.length() >= 98) {
			Framework::GetFrameWork()->RenderQueue[Framework::GetFrameWork()->RenderQueueIndex] = text;
			Framework::GetFrameWork()->RenderQueueIndex++;
			Framework::GetFrameWork()->RenderQueueIndex %= 100;
		}

		Native::BeginTextCommandDisplayText("STRING");
		Native::AddTextComponentSubstringPlayerName(text.c_str());
		Native::EndTextCommandDisplayText(position.m_X, position.m_Y, 0);
	}

	inline void DrawLeftText(const char* text, float x, float y, float size, int font, Color color, bool outline, bool shadow) {
		Native::SetTextScale(size, size);
		Native::SetTextFont(static_cast<int>(font));
		Native::SetTextColour(color.r, color.g, color.b, color.a);
		if (outline)
			Native::SetTextOutline();
		if (shadow)
			Native::SetTextDropShadow();
		Native::BeginTextCommandDisplayText("STRING");
		Native::AddTextComponentSubstringPlayerName(text);
		Native::EndTextCommandDisplayText( x, y , 0);
	}

	inline void DrawRightText(const char* text, float x, float y, float size, int font, Color color, bool outline, bool shadow) {
		Native::SetTextWrap(0.f, x);
		Native::SetTextRightJustify(true);
		DrawLeftText(text, x, y, size, font, color, outline, shadow);
	}

	inline void DrawSprite(std::pair<std::string, std::string> asset, Math::Vector2<float> position, Math::Vector2<float> scale, Color color = { 255, 255, 255, 255 }, float rotation = 0.f, bool alligned = false) {
		if (!Native::HasStreamedTextureDictLoaded(asset.first.c_str()) && asset.first != "aether") {
			Native::RequestStreamedTextureDict(asset.first.c_str(), true);
		}
		if (alligned) {
			Native::DrawSprite(asset.first.c_str(), asset.second.c_str(),  position.m_X + (scale.m_X * 0.5f), position.m_Y + (scale.m_Y * 0.5f), scale.m_X, scale.m_Y, rotation, color.r, color.g, color.b, color.a, TRUE);
		}
		else {
			Native::DrawSprite(asset.first.c_str(), asset.second.c_str(),  position.m_X, position.m_Y , scale.m_X, scale.m_Y, rotation, color.r, color.g, color.b, color.a, 0);
		}
	}

	inline Math::Vector2<float> GetSpriteScale(float size) {
		int x, y;
		Native::_GetActiveScreenResolution(&x, &y);
		return { (static_cast<float>(y) / static_cast<float>(x)) * size, size };
	}

	inline float CaculateStringWidth(std::string string, int font, float scale) {
		Native::_BeginTextCommandWidth("STRING");
		Native::AddTextComponentSubstringPlayerName(string.c_str());
		Native::SetTextScale(0.f, scale);
		return Native::_EndTextCommandGetWidth(font);
	}

	inline float GetNormalizedFontScale(int font, float scale) {
		switch (font) {
		case 0: return (scale * 1.0f);
		case 1: return (scale * 1.3f);
		case 2: return (scale * 1.11f);
		case 4: return (scale * 1.11f);
		case 7: return (scale * 1.29f);
		}

		return scale;
	}

	inline void DrawSpriteAligned(std::pair<std::string, std::string> asset, Math::Vector2<float> position, Math::Vector2<float> scale, float rotation, Color color) {

		if (!Native::HasStreamedTextureDictLoaded(asset.first.c_str()) && asset.first != "aethertextures") {
			Native::RequestStreamedTextureDict(asset.first.c_str(), true);
		}

		Native::DrawSprite(asset.first.c_str(), asset.second.c_str(),  position.m_X + (scale.m_X * 0.5f), position.m_Y + (scale.m_Y * 0.5f), scale.m_X, scale.m_Y, rotation, color.r, color.g, color.b, color.a, TRUE);
	}
}