#pragma once
#include "stdafx.hpp"
#include "abstract_option.hpp"

namespace Framework::Options {
	class AbstractSubmenu {
	public:
		virtual const char* GetName() = 0;

		virtual std::uint32_t GetID() = 0;
		virtual void Execute() = 0;
		virtual void Reset() = 0;

		virtual AbstractOption* GetOption(std::size_t i) = 0;
		virtual std::size_t GetOptionsSize() = 0;
		virtual std::size_t GetSelectedOption() = 0;
		virtual void SetSelectedOption(std::size_t) = 0;

		void ScrollForward() {
			if (GetSelectedOption() < GetOptionsSize() - 1)
				SetSelectedOption(GetSelectedOption() + 1);
			else
				SetSelectedOption(0);
		}
		void ScrollBackward() {
			if (GetSelectedOption() > 0)
				SetSelectedOption(GetSelectedOption() - 1);
			else
				SetSelectedOption(GetOptionsSize() - 1);
		}
	};
}