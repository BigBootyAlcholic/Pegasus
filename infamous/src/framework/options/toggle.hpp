#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "../framework.hpp"

namespace Framework::Options {
	class ToggleOption : public BaseOption<ToggleOption> {
	private:
		bool* m_Bool = nullptr;
		bool m_DisplayInverted = false;
		std::function<void()> m_action = [] {};
		std::string m_name = "";
	public:
		explicit ToggleOption(const char* const text) {
			m_name = text;
			BaseOption::SetLeftText(m_name.c_str());
		}

		ToggleOption& addTooltip(const char* const text) {
			BaseOption::SetToolTip(text);
			return *this;
		}

		ToggleOption& addClick(std::function<void()> action = [] {}) {
			m_action = action;
			BaseOption::SetAction(std::move(m_action));
			return *this;
		}

		ToggleOption& addToggle(bool* const b00l) {
			m_Bool = b00l;
			return *this;
		}

		ToggleOption& addHotkey() {
			addTooltip("Hotkeys Supported. Press F8 to bind");
			return *this;
		}

		ToggleOption& addTranslate() {
			//set_left_text(translation);
			return *this;
		}


		void HandleAction(eOptionAction action) override {
			if (action == eOptionAction::click) {
				*m_Bool = !*m_Bool;
			}
			BaseOption::HandleAction(action);
		}

		bool* GetBoolPointer() override {
			return m_Bool;
		}


		bool GetFlag(eOptionFlag flag) override {
			if (flag == eOptionFlag::ToggleOption) {
				Framework::GetFrameWork()->m_Toggled = *m_Bool;
				return true;
			}
			return BaseOption::GetFlag(flag);
		}
	};

	template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
	class ToggleNumberOption : public BaseOption<ToggleNumberOption<Type>> {
	private:
		Type* m_number = nullptr;
		bool* m_bool = nullptr;
		Type m_min = 0;
		Type m_max = 1;
		Type m_step = 1;
		std::size_t m_precision = 3;
		std::function<void()> m_function;
		using base = BaseOption<ToggleNumberOption<Type>>;
		using display_type = std::conditional_t<sizeof(Type) == 1, std::uint32_t, Type>;
	public:
		explicit ToggleNumberOption(const char* text) {
			base::SetLeftText(text);
		}

		ToggleNumberOption& addTooltip(const char* const text)
		{
			base::SetToolTip(text);
			return *this;
		}

		ToggleNumberOption& addClick(std::function<void()> action = [] {}) {
			base::SetAction(std::move(action));
			return *this;
		}

		ToggleNumberOption& addToggle(bool* const b00l) {
			m_bool = b00l;
			return *this;
		}

		ToggleNumberOption& addNumber(Type* number) {
			m_number = number;
			return *this;
		}

		ToggleNumberOption& addMin(Type min) {
			m_min = min;
			return *this;
		}

		ToggleNumberOption& addMax(Type max) {
			m_max = max;
			return *this;
		}

		ToggleNumberOption& addStep(Type step) {
			m_step = step;
			return *this;
		}

		ToggleNumberOption& setPrecision(Type precision) {
			m_precision = precision;
			return *this;
		}

		const char* GetRightText() {
			MemoryStringStream stream(base::m_right_text);
			stream
				<< std::setprecision(m_precision)
				<< std::fixed
				<< static_cast<display_type>(*m_number);
			return base::GetRightText();
		}

		void HandleAction(eOptionAction action) override {
			if (action == eOptionAction::left_click) {
				if (*m_number - m_step < m_min)
					*m_number = m_max;
				else
					*m_number -= m_step;
				if (base::m_action)
					std::invoke(base::m_action);
			}
			else if (action == eOptionAction::right_click) {
				if (*m_number + m_step > m_max)
					*m_number = m_min;
				else
					*m_number += m_step;
				if (base::m_action)
					std::invoke(base::m_action);
			}
			else if (action == eOptionAction::click) {
				*m_bool = !*m_bool;
			}
			base::HandleAction(action);
		}

		bool GetFlag(eOptionFlag flag) override {
			if (flag == eOptionFlag::ToggleNumberOption) {
				Framework::GetFrameWork()->m_Toggled = *m_bool;
				return true;
			}
			return base::GetFlag(flag);
		}
	};
}