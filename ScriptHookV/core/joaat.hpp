#pragma once
#include <cstdint>
#include <string_view>

namespace Rage {
	using joaat_t = uint32_t;
	inline constexpr char tolower(char c) {
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}
	inline uint32_t Koaat(std::string_view str, bool forceLowerCase = true) {
		joaat_t hash{};
		for (auto& c : str) {
			hash += forceLowerCase ? tolower(c) : c; hash += (hash << 10); hash ^= (hash >> 6);
		}
		hash += (hash << 3); hash ^= (hash >> 11); hash += (hash << 15);
		return hash;
	}
	template <typename t = std::string_view>
	inline consteval uint32_t ConstexprJoaat(t str, bool const forceLowerCase = true) {
		uint32_t hash{};
		while (*str != '\0') {
			hash += forceLowerCase ? tolower(*str) : *str, ++str;
			hash += hash << 10, hash ^= hash >> 6;
		}
		hash += (hash << 3), hash ^= (hash >> 11), hash += (hash << 15);

		return hash;
	}
}
inline consteval uint32_t operator ""_Joaat(const char* str, size_t) { return Rage::ConstexprJoaat(str); }
inline consteval uint32_t operator ""_JoaatNormalised(const char* str, size_t) { return Rage::ConstexprJoaat(str, false); }