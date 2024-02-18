#include "stdafx.hpp"
#include "../hooks.hpp"

namespace Hooks {
	bool ConstructJoinMessageHook(RemoteGamerInfoMsg* info, void* data, int size, int* bits) {
		if (!info->unk_0xC0)
			info->unk_0xC0 = 1;
		memset(info->m_handles, 0, sizeof(info->m_handles));
		info->m_num_handles = 0;

		return OgConstructJoinMessageHook(info, data, size, bits);
	}
}