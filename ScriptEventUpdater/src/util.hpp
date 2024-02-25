#pragma once
#include "stdafx.hpp"

namespace Vars {
	inline int g_Iteration{};
	inline bool g_snakeCase{};
	inline bool g_camelCase{};
	inline bool g_Uppercase{};
	inline bool g_ToFile{};
	inline bool g_Print{};
}
namespace Util {
	inline void Wait() {
		std::cout << "Press any key to exit..." << std::endl;
		while (true) {
			if (std::cin.get()) {
				break;
			}
		}
	}
	inline uint32_t Joaat(std::string_view str) {
		uint32_t hash{};
		for (char c : str) {
			hash += tolower(c);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		return hash;
	}
	inline std::string strTolower(std::string str) {
		std::vector<char> buffer{};
		for (char& c : str) {
			buffer.push_back(tolower(c));
		}
		return std::string(buffer.begin(), buffer.end());
	}
	inline char toLower(char c) {
		return char(tolower(c));
	}
	inline bool newGetChr() {
		if (std::cin.get() == int('\n'))
			return true;
		return false;
	}
	inline bool ConditionMet(std::string condition) {
		bool ret{};
		std::cout << condition << " (Y/N): ";
		char c{ (char)std::cin.get() };
		if (c == 'Y' || c == 'y') {
			ret = true;
		}
		newGetChr();
		return ret;
	}
	inline std::string GetLine(std::string value) {
		std::string str{};
		std::cout << value << ": ";
		std::getline(std::cin, str);
		return str;
	}
	inline std::string Enumify(std::string str, int32_t value, bool snake, bool camel, bool upper, bool last = false) {
		std::vector<char> buffer{};
		std::string spacer{ " = " };
		buffer.push_back('	');
		if (snake) {
			std::string data{ upper ? str : strTolower(str) };
			for (size_t i{}; i != str.size(); ++i) {
				buffer.push_back(data[i]);
			}
		}
		else {
			for (size_t i{}; i != str.size(); ++i) {
				char c{ upper ? str[i] : toLower(str[i]) };
				if (snake) {
					buffer.push_back(c);
					continue;
				}
				if (!camel && i == 0) {
					buffer.push_back(str[i]);
					continue;
				}
				else if (c == '_') {
					continue;
				}
				if (str[i - 1] == '_') {
					if (!upper && snake) {
						buffer.push_back(c);
						continue;
					}
					buffer.push_back(str[i]);
					continue;
				}
				buffer.push_back(c);
			}
		}
		for (char& c : spacer) {
			buffer.push_back(c);
		}
		for (char& c : std::to_string(value)) {
			buffer.push_back(c);
		}
		if (!last) {
			buffer.push_back(',');
		}
		return std::string(buffer.begin(), buffer.end());
	}
}