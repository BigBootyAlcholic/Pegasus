#pragma once

#include <cstdint>
#include <string_view>
#include <span>

namespace Rage
{
	using joaat_t = std::uint32_t;
	inline constexpr char joaat_to_lower(char c)
	{
		return c >= 'A' && c <= 'Z' ? c | 1 << 5 : c;
	}



	inline constexpr joaat_t LowerCaseJoaat(const std::string_view str)
	{
		joaat_t hash = 0;
		for (auto c : str)
		{
			hash += joaat_to_lower(c);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		return hash;
	}

	template<size_t N>
	inline constexpr joaat_t consteval_joaat(char const (&data)[N])
	{
		joaat_t hash = 0;

		for (std::size_t i = 0; i < N - 1; ++i)
		{
			hash += joaat_to_lower(data[i]);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}

	inline consteval joaat_t consteval_joaat(const std::span<const char>& data)
	{
		joaat_t hash = 0;

		for (std::size_t i = 0; i < data.size() - 1; ++i)
		{
			hash += joaat_to_lower(data[i]);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}
	static_assert(consteval_joaat("test") == 0x3f75ccc1);
};

#define RAGE_JOAAT_IMPL(str) (::Rage::consteval_joaat(str))
#define RAGE_JOAAT(str) (std::integral_constant<Rage::joaat_t, RAGE_JOAAT_IMPL(str)>::value)