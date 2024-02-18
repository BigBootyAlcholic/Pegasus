#pragma once
#include <cstdint>

namespace Rage
{
	struct NativeRegistration {
		uint64_t m_NextRegistration1;
		uint64_t m_NextRegistration2;
		uint64_t m_Handlers[7];
		uint32_t m_NumEntries1;
		uint32_t m_NumEntries2;
		uint64_t m_Hashes;

		__declspec(noinline) NativeRegistration* GetNextRegistration() {
			uintptr_t result;
			auto v5 = reinterpret_cast<uintptr_t>(&m_NextRegistration1);
			auto v12 = 2i64;
			auto v13 = v5 ^ m_NextRegistration2;
			auto v14 = (char*)&result - v5;
			do {
				*(DWORD*)&v14[v5] = v13 ^ *(DWORD*)v5;
				v5 += 4i64;
				--v12;
			} while (v12);

			return reinterpret_cast<NativeRegistration*>(result);
		}

		__declspec(noinline) uint32_t GetNumEntries() {
			return ((uintptr_t)&m_NumEntries1) ^ m_NumEntries1 ^ m_NumEntries2;
		}

		__declspec(noinline) uint64_t GetHash(uint32_t index) {
			auto naddr = 16 * index + reinterpret_cast<uintptr_t>(&m_NextRegistration1) + 0x54;
			auto v8 = 2i64;
			uint64_t result;
			auto v11 = (char*)&result - naddr;
			auto v10 = naddr ^ *(DWORD*)(naddr + 8);
			do {
				*(DWORD*)&v11[naddr] = v10 ^ *(DWORD*)(naddr);
				naddr += 4i64;
				--v8;
			} while (v8);

			return result;
		}
	};
}