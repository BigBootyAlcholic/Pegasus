#pragma once
#include "option.hpp"
#include "../framework.hpp"

namespace Framework::Options {
	class ButtonOption : public BaseOption<ButtonOption> {
	private:
		std::function<void()> m_function;
		struct Keyboard {
			bool m_enabled = false;
			bool m_is_active = false;
			int m_max_chars = 0;
			std::function<void(ButtonOption*, const char*)> m_callback = {};
			std::string m_title = "";
			std::string m_default_text = "";
		};

		Keyboard m_keyboard;
	public:

		explicit ButtonOption(const char* text) {
			SetLeftText(text);
		}

		ButtonOption& addTooltip(const char* description) {
			BaseOption::SetToolTip(description);
			return *this;
		}

		ButtonOption& addRightText(const char* const text) {
			BaseOption::SetRightText(text);
			return *this;
		}

		ButtonOption& addClick(std::function<void()> function) {
			SetAction(std::move(function));
			return *this;
		}

		ButtonOption& addHotkey() {
			addTooltip("Hotkeys Supported. Press F8 to bind");
			return *this;
		}

		ButtonOption& addTranslate() {

			//set_left_text(translation);
			return *this;
		}


		bool GetFlag(eOptionFlag flag) override {
			if (flag == eOptionFlag::SimpleOption) {
				return true;
			}
			return BaseOption::GetFlag(flag);
		}
	};

	class VehicleOption : public BaseOption<VehicleOption> {
	private:
		std::function<void()> m_function;
	public:
		explicit VehicleOption(const char* text) {
			SetLeftText(text);
		}

		VehicleOption& addToolTip(const char* description) {
			BaseOption::SetToolTip(description);
			return *this;
		}

		VehicleOption& addRightText(const char* text) {
			BaseOption::SetRightText(text);
			return *this;
		}

		VehicleOption& addClick(std::function<void()>&& function) {
			SetAction(std::move(function));
			return *this;
		}

		bool GetFlag(eOptionFlag flag) override {
			if (flag == eOptionFlag::VehicleOption) {
				return true;
			}
			return BaseOption::GetFlag(flag);
		}
	};

	
}