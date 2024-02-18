#pragma once
#include "option.hpp"
#include "../framework.hpp"
#include "rage/classes/rage/joaat.hpp"
namespace Framework::Options {
	class SubmenuOption : public BaseOption<SubmenuOption> {
	private:
		std::uint32_t m_sub_id{};
		uint8_t m_custom_data[0x10];
		std::function<void(SubmenuOption*, int)> m_on_update = [](SubmenuOption*, int) {};
	public:
		explicit SubmenuOption(const char* text) {
			SetLeftText(text);
		}

		SubmenuOption& addClick(std::function<void()>&& function) {
			SetAction(std::move(function));
			return *this;
		}

		SubmenuOption& add_update(std::function<void(SubmenuOption*, int)> function) {
			m_on_update = function;
			return *this;
		}

		SubmenuOption& setTarget(const const char* id) {
			m_sub_id = Rage::LowerCaseJoaat(id);
			return *this;
		}

		SubmenuOption& add_custom_data(uint8_t* data) {
			memcpy(m_custom_data, data, 0x10);
			return *this;
		}

		SubmenuOption& addTooltip(const char* const text) {
			BaseOption::SetToolTip(text);
			return *this;
		}

		SubmenuOption& addTranslate() {
			//set_left_text(translation);
			return *this;
		}

		void HandleAction(eOptionAction action) override {
			if (action == eOptionAction::click) {
				Framework::GetFrameWork()->SwitchToSubmenu(m_sub_id);
			}
			BaseOption::HandleAction(action);
		}

		bool GetFlag(eOptionFlag flag) override {
			if (flag == eOptionFlag::Indicator) {
				return true;
			}
			return BaseOption::GetFlag(flag);
		}
	};
}