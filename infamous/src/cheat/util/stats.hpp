#pragma once
#include "stdafx.hpp"

namespace Stats {
	template<class T>
	class Interface {
	public:
		static void Set(const char* StatName, T Value);
		static T Get(const char* StatName);

		static void Set(uint32_t StatHash, T Value);
		static T Get(uint32_t StatHash);
	};
}