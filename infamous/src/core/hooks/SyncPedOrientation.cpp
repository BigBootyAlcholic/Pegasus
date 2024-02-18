#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/menu/submenus/network/network_spoofing.hpp"
#include "rage/classes/ped/CPed.hpp"
namespace Hooks {
	void SyncPedOrientationHook(uint64_t Rcx, uint64_t Rdx) {
		auto& Vars = NetworkSpoofingMenuVars::m_Vars;
		if (Vars.m_PedSpinbot || Vars.m_PedFaceDirection || Vars.m_PedJitter) {
			if (Rcx) {
				if (*Patterns::Vars::g_PedFactory) {
					CPed* Ped = *(CPed**)(Rcx - 0x1B0);
					if (Vars.m_PedSpinbot || Vars.m_PedJitter) {
						*(float*)(Rdx + 0xC0) = Vars.m_PedHookValue.m_X;
						*(float*)(Rdx + 0xC4) = Vars.m_PedHookValue.m_Y;
						return;
					}
					if (Vars.m_PedFaceDirection) {
						float Val = 0.f;

						switch (Vars.m_PedFaceDirectionVal) {
						case 0: Val = -90.f; break;
						case 1: Val = 90.f; break;
						case 2: Val = 180.f; break;
						}

						if (Vars.m_PedRelative) {
							Val += Menu::GetLocalPlayer().m_Heading;
						}

						if (Val > 180.f) Val = -180.f + (Val - 180.f);

						*(float*)(Rdx + 0xC0) = Val * 0.0174532924f;
						*(float*)(Rdx + 0xC4) = Val * 0.0174532924f;
					}

					return;
				}
			}
		}
		OgSyncPedOrientation(Rcx, Rdx);
	}
}