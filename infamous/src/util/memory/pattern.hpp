#pragma once
#include "stdafx.hpp"

namespace Memory {
	class Ptr {
	public:
		Ptr(void* ptr = nullptr);
		explicit Ptr(std::uintptr_t ptr);

		template<typename T>
		std::enable_if_t<std::is_pointer_v<T>, T> As() const;

		template<typename T>
		std::enable_if_t<std::is_lvalue_reference_v<T>, T> As() const;

		template<typename T>
		std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> As() const;

		template<typename T>
		Ptr Add(T offset) const;

		template<typename T>
		Ptr Sub(T offset) const;

		Ptr Rip() const;

		Ptr Call() const;
		Ptr FromInstruction() const;
		Ptr Lea() const;

		explicit operator bool();

		friend bool operator==(Ptr a, Ptr b);
		friend bool operator!=(Ptr a, Ptr b);

	private:
		void* ptr;
	};

	inline Ptr::Ptr(void* ptr) : ptr(ptr) {}

	inline Ptr::Ptr(std::uintptr_t ptr) : ptr(reinterpret_cast<void*>(ptr)) {}

	template<typename T>
	inline std::enable_if_t<std::is_pointer_v<T>, T> Ptr::As() const {
		return static_cast<T>(ptr);
	}

	template<typename T>
	inline std::enable_if_t<std::is_lvalue_reference_v<T>, T> Ptr::As() const {
		return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(ptr);
	}

	template<typename T>
	inline std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> Ptr::As() const {
		return reinterpret_cast<std::uintptr_t>(ptr);
	}

	template<typename T>
	inline Ptr Ptr::Add(T offset) const {
		return Ptr(As<std::uintptr_t>() + offset);
	}

	template<typename T>
	inline Ptr Ptr::Sub(T offset) const {
		return Ptr(As<std::uintptr_t>() - offset);
	}

	inline Ptr Ptr::Rip() const {
		return Add(As<std::int32_t&>()).Add(4);
	}

	inline Ptr Ptr::Call() const {
		return Add(1).Rip();
	}

	inline Ptr Ptr::Lea() const {
		return Add(2).Rip();
	}

	inline Ptr Ptr::FromInstruction() const {
		return Add(3).Rip();
	}

	inline bool operator==(Ptr a, Ptr b) {
		return a.ptr == b.ptr;
	}

	inline bool operator!=(Ptr a, Ptr b) {
		return a.ptr != b.ptr;
	}

	inline Ptr::operator bool() {
		return ptr != nullptr;
	}

	inline std::optional<std::uint8_t> to_hex(char const c) {
		switch (c) {
		case '0': return static_cast<std::uint8_t>(0x0);
		case '1': return static_cast<std::uint8_t>(0x1);
		case '2': return static_cast<std::uint8_t>(0x2);
		case '3': return static_cast<std::uint8_t>(0x3);
		case '4': return static_cast<std::uint8_t>(0x4);
		case '5': return static_cast<std::uint8_t>(0x5);
		case '6': return static_cast<std::uint8_t>(0x6);
		case '7': return static_cast<std::uint8_t>(0x7);
		case '8': return static_cast<std::uint8_t>(0x8);
		case '9': return static_cast<std::uint8_t>(0x9);
		case 'a': return static_cast<std::uint8_t>(0xa);
		case 'b': return static_cast<std::uint8_t>(0xb);
		case 'c': return static_cast<std::uint8_t>(0xc);
		case 'd': return static_cast<std::uint8_t>(0xd);
		case 'e': return static_cast<std::uint8_t>(0xe);
		case 'f': return static_cast<std::uint8_t>(0xf);
		case 'A': return static_cast<std::uint8_t>(0xA);
		case 'B': return static_cast<std::uint8_t>(0xB);
		case 'C': return static_cast<std::uint8_t>(0xC);
		case 'D': return static_cast<std::uint8_t>(0xD);
		case 'E': return static_cast<std::uint8_t>(0xE);
		case 'F': return static_cast<std::uint8_t>(0xF);
		default: return std::nullopt;
		}
	}

	class Pattern {
	public:
		Pattern(std::string_view ida_sig) {
			const auto size = ida_sig.size();
			for (std::size_t i{}; i != size; ++i) {
				if (ida_sig[i] == ' ')
					continue;
				bool last = (i == ida_sig.size() - 1);
				if (ida_sig[i] != '?') {
					if (!last) {
						auto c1 = to_hex(ida_sig[i]);
						auto c2 = to_hex(ida_sig[i + 1]);
						if (c1 && c2) {
							m_bytes.emplace_back(static_cast<std::uint8_t>((*c1 * 0x10) + *c2));
						}
					}
				}
				else {
					m_bytes.push_back({});

					if (ida_sig[i + 1] == '?') {
						++i;
					}
				}
			}
		}
		explicit Pattern(const void* bytes, std::string_view mask) {
			const auto size = mask.size();
			for (std::size_t i{}; i != size; ++i) {
				if (mask[i] != '?')
					m_bytes.emplace_back(static_cast<const std::uint8_t*>(bytes)[i]);
				else
					m_bytes.push_back(std::nullopt);
			}
		}

		inline Pattern(const char* ida_sig) :
			Pattern(std::string_view(ida_sig))
		{}

		std::vector<std::optional<u8>> m_bytes;
	};

	class Range
	{
	public:
		Range(Ptr base, std::size_t size) :
			m_base(base),
			m_size(size)
		{
		}

		Ptr Begin() const {
			return m_base;
		}

		Ptr End() const {
			return m_base.Add(m_size);
		}

		std::size_t Size() const {
			return m_size;
		}

		bool Contains(Ptr h) const {
			return h.As<std::uintptr_t>() >= Begin().As<std::uintptr_t>() && h.As<std::uintptr_t>() <= End().As<std::uintptr_t>();
		}

		static Ptr ScanPattern(const std::optional<u8>* sig, std::size_t length, Ptr begin, std::size_t module_size) {
			std::size_t maxShift = length;
			std::size_t max_idx = length - 1;

			std::size_t wild_card_idx{ static_cast<size_t>(-1) };
			for (int i{ static_cast<int>(max_idx - 1) }; i >= 0; --i) {
				if (!sig[i]) {
					maxShift = max_idx - i;
					wild_card_idx = i;
					break;
				}
			}

			std::size_t shift_table[UINT8_MAX + 1]{};
			for (std::size_t i{}; i <= UINT8_MAX; ++i) {
				shift_table[i] = maxShift;
			}

			for (std::size_t i{ wild_card_idx + 1 }; i != max_idx; ++i) {
				shift_table[*sig[i]] = max_idx - i;
			}

			const auto scan_end = module_size - length;
			for (std::size_t current_idx{}; current_idx <= scan_end;) {
				for (std::ptrdiff_t sig_idx{ (std::ptrdiff_t)max_idx }; sig_idx >= 0; --sig_idx) {
					if (sig[sig_idx] && *begin.Add(current_idx + sig_idx).As<u8*>() != *sig[sig_idx]) {
						current_idx += shift_table[*begin.Add(current_idx + max_idx).As<u8*>()];
						break;
					}
					else if (sig_idx == NULL) {
						return begin.Add(current_idx);
					}
				}
			}
			return nullptr;
		}

		Ptr Scan(Pattern const& sig) const {
			auto data = sig.m_bytes.data();
			auto length = sig.m_bytes.size();

			if (auto result = ScanPattern(data, length, m_base, m_size); result)
			{
				return result;
			}

			return nullptr;
		}

		static bool PatternMatches(std::uint8_t* target, const std::optional<std::uint8_t>* sig, std::size_t length) {
			for (std::size_t i{}; i != length; ++i) {
				if (sig[i] && *sig[i] != target[i]) {
					return false;
				}
			}

			return true;
		}

		std::vector<Ptr> ScanAll(Pattern const& sig) const {
			std::vector<Ptr> result{};
			auto data = sig.m_bytes.data();
			auto length = sig.m_bytes.size();

			const auto scan_end = m_size - length;
			for (std::uintptr_t i{}; i != scan_end; ++i) {
				if (PatternMatches(m_base.Add(i).As<std::uint8_t*>(), data, length)) {
					result.push_back(m_base.Add(i));
				}
			}

			return std::move(result);
		}
	protected:
		Ptr m_base;
		std::size_t m_size;
	};

	class Module : public Range
	{
	public:
		Module(HMODULE mod) :
			Range(mod, 0)
		{
			auto dosHeader = m_base.As<IMAGE_DOS_HEADER*>();
			auto ntHeader = m_base.Add(dosHeader->e_lfanew).As<IMAGE_NT_HEADERS*>();
			m_size = ntHeader->OptionalHeader.SizeOfImage;
		}
		explicit Module(std::nullptr_t) :
			Module(GetModuleHandle(nullptr))
		{
		}
		explicit Module(std::string_view name) :
			Module(GetModuleHandleA(name.data()))
		{
		}
		explicit Module(std::wstring_view name) :
			Module(GetModuleHandleW(name.data()))
		{
		}

		Memory::Ptr get_export(std::string_view symbol_name)
		{
			return GetProcAddress(m_base.As<HMODULE>(), symbol_name.data());
		}
	};

	class Batch {
	public:
		explicit Batch() = default;
		~Batch() noexcept = default;

		void Add(std::pair<const char*, Pattern> entry, std::function<void(Ptr)> callback, bool& out);
		void Run(Range region);

		struct entry {
			std::string m_name;
			Pattern m_pattern;
			std::function<void(Memory::Ptr)> m_callback;
			bool m_out;

			explicit entry(std::string name, Pattern pattern, std::function<void(Memory::Ptr)> callback, bool out) :
				m_name(std::move(name)),
				m_pattern(std::move(pattern)),
				m_callback(std::move(callback)),
				m_out(std::move(out))
			{}
		};

	private:
		std::vector<entry> m_entries;
	};

	template<typename T>
	concept SpanCompatibleType = requires(T a)
	{
		std::span{ a };
	};
}