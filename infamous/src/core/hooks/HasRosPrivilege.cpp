#include "stdafx.hpp"
#include "../hooks.hpp"

namespace Hooks {

	bool IsBanned = false;

	bool HasRosPrivilegeHook(uint64_t* Rcx, int32_t Rdx) {
		switch (Rdx) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6: { return !IsBanned ? true : false; } break;
		case 7: { return !IsBanned ? false : true; } break;
		case 8:
		case 9:
		case 10:
		case 11: { return !IsBanned ? true : false; } break;
		case 17:
		case 18:
		case 19: { return !IsBanned ? true : false; } break;
		case 20:
		case 21:
		case 22:
		case 24:
		case 25: { return !IsBanned ? false : true; } break;
		}

		return OgHasRosPrivilege(Rcx, Rdx);
	}
}