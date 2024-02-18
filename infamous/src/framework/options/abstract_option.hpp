#pragma once
#include "stdafx.hpp"
#include "../types.hpp"
namespace Framework::Options {
	class AbstractOption {
	public:
		virtual const char* GetLeftText() = 0;
		virtual const char* GetRightText() = 0;
		virtual const char* GetDescription() = 0;

		virtual Color GetColor() = 0;
		virtual void HandleAction(eOptionAction action) = 0;
		virtual void SetValue(double text) = 0;
		virtual void SetValue(std::string text) = 0;
		virtual bool GetFlag(eOptionFlag flag) = 0;
		virtual std::function<void()> GetAction() = 0;
		virtual bool* GetBoolPointer() = 0;//doesnt work properly


	};
}