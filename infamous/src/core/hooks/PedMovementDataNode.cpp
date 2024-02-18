#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/menu/submenus/network/network_spoofing.hpp"
#include "rage/classes/ped/CPed.hpp"
namespace Hooks {
	void PedMovementDataNodeHook(uint64_t Rcx, uint64_t Rdx) {
		OgPedMovementDataNode(Rcx, Rdx);

		if (NetworkSpoofingMenuVars::m_Vars.m_MoveAnimation) {
			*(bool*)(Rcx + 0xC0) = false; // Animation
			*(bool*)(Rcx + 0xC1) = false; // Animation
		}
	}
}