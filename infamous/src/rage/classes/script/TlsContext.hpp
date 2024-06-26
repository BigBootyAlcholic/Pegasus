#pragma once
#include "stdafx.hpp"

namespace Rage {
	class tlsContext {
	public:
		char pad_0000[180]; //0x0000
		uint32_t m_unknown_byte; //0x00B4
		class sysMemAllocator* m_allocator; //0x00B8
		class sysMemAllocator* m_tls_entry; //0x00C0
		class sysMemAllocator* m_unk_allocator; //0x00C8
		uint32_t m_console_handle; //0x00D0
		char pad_00D4[188]; //0x00D4
		uint64_t unk_0190; //0x0190
		char pad_0194[1712]; //0x0194
		class scrThread* m_script_thread; //0x0848
		bool m_is_script_thread_active; //0x00850

		static tlsContext* get() { return *(tlsContext**)(__readgsqword(0x58)); }
		static tlsContext** getPointer() { return (tlsContext**)(__readgsqword(0x58)); }
	}; //Size: 0x0850
	static_assert(sizeof(tlsContext) == 0x858);
}