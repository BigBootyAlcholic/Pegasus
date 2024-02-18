#pragma once
#include "option.hpp"
#include "../render.hpp"
#include "../types.hpp"
namespace Framework::Options {
	class BreakOption : public BaseOption<BreakOption> {
	public:
		explicit BreakOption(const char* text) {
			SetLeftText(text);
		}
		bool GetFlag(eOptionFlag flag) override {
			if (flag == eOptionFlag::BreakOption) {
				return true;
			}
			return BaseOption::GetFlag(flag);
		}
	};
}