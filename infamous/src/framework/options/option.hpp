#pragma once
#include "stdafx.hpp"
#include "abstract_option.hpp"
#include "../types.hpp"
namespace Framework::Options {
	template <typename T>
	class BaseOption : public AbstractOption {
	public:
		const char* GetLeftText() override {
			return &m_left_text[0];
		}
		const char* GetRightText() override {
			return &m_right_text[0];
		}
		const char* GetDescription() override {
			return &m_description[0];
		}

		Color GetColor() override {
			return m_color;
		}

		void HandleAction(eOptionAction action) override {
			if (action == eOptionAction::click) {
				if (m_action) {
					std::invoke(m_action);
				}
			}
		}
		bool GetFlag(eOptionFlag) override {
			return false;
		}
		std::function<void()> GetAction() override {
			return m_action;
		}

		bool* GetBoolPointer() override {
			return nullptr;
		}

		void SetValue(double text) override {}
		void SetValue(std::string text) {}

		T& HandleHover() {
			return static_cast<T&>(*this);
		}

		T& SetLeftText(const char* text) {
			std::strncpy(&m_left_text[0], text, sizeof(m_left_text) - 1);
			return static_cast<T&>(*this);
		}
		T& SetRightText(const char* text) {
			std::strncpy(&m_right_text[0], text, sizeof(m_right_text) - 1);
			return static_cast<T&>(*this);
		}

		T& SetToolTip(const char* text) {
			if (text)
				std::strncpy(&m_description[0], text, sizeof(m_description) - 1);
			return static_cast<T&>(*this);
		}
		T& SetAction(std::function<void()> action) {
			m_action = std::move(action);
			return static_cast<T&>(*this);
		}

		T& SetColor(Color col) {
			m_color = col;
			return static_cast<T&>(*this);
		}

	protected:
		std::function<void()> m_action;
		char m_left_text[128] = {};
		char m_right_text[128] = {};
		char m_description[1024] = {};
		Color m_color;
	};
}