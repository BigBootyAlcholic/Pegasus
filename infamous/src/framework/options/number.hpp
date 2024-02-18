#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "../framework.hpp"

namespace Framework::Options {
	template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
	class NumberOption : public BaseOption<NumberOption<Type>> {
	private:
		Type* m_number = nullptr;
		Type m_min = 0;
		Type m_max = 1;
		Type m_step = 1;
		std::size_t m_precision = 3;
		using base = BaseOption<NumberOption<Type>>;
		using display_type = std::conditional_t<sizeof(Type) == 1, std::uint32_t, Type>;
		std::function<void()> m_function;
		std::function<void()> m_on_hover = []() {};
		std::string m_name = "";
	public:
		explicit NumberOption(const char* text) {
			m_name = text;
			base::SetLeftText(m_name.c_str());
		}

		NumberOption& addTooltip(const char* const text)
		{
			base::set_tooltip(text);
			return *this;
		}

		NumberOption& addClick(std::function<void()> action = [] {}) {
			base::set_action(std::move(action));
			return *this;
		}

		NumberOption& addHover(std::function<void()> function) {
			m_on_hover = function;
			return *this;
		}

		NumberOption& addNumber(Type* number) {
			m_number = number;
			return *this;
		}

		NumberOption& addMin(Type min) {
			m_min = min;
			return *this;
		}

		NumberOption& addMax(Type max) {
			m_max = max;
			return *this;
		}

		NumberOption& addStep(Type step) {
			m_step = step;
			return *this;
		}

		NumberOption& setPrecision(Type precision) {
			m_precision = precision;
			return *this;
		}

		NumberOption& addTranslate() {
			const char* translation = (BaseOption::GetLeftText());
			//base::set_left_text(translation);
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
			m_on_hover();
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
			/*else if (action == eOptionAction::click) {
				static bool active = false;
				MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
				while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
					active = true;
					menu::renderer::getRenderer()->reset_keys();
					util::fiber::go_to_main();
				}
				active = false;
				if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
					return;

				*m_number = std::atoi(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
				if (base::m_action)
					std::invoke(base::m_action);
			}*/
			base::HandleAction(action);
		}

		bool GetFlag(eOptionFlag flag) override {
			if (flag == eOptionFlag::NumberOption) {
				return true;
			}
			return base::GetFlag(flag);
		}
	};

}