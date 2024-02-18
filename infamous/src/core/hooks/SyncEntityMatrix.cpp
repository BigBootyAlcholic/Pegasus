#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/menu/submenus/network/network_spoofing.hpp"
#include "rage/classes/ped/CPed.hpp"
#include "rage/engine.hpp"
namespace Hooks {

	struct EntityOrientation {
		char _0x0000[0xC0];
		Math::Matrix m_TransformedMatrix;
	};

	void SyncEntityMatrixHook(uint64_t Rcx, uint64_t Rdx) {
		EntityOrientation* Transform = (EntityOrientation*)Rdx;

		if (Transform) {
			auto& Vars = NetworkSpoofingMenuVars::m_Vars;
			if (Vars.m_VehSpinbot || Vars.m_VehStatic || Vars.m_VehJitter) {
				if (Menu::GetLocalPlayer().m_InVehicle) {
					if (*Patterns::Vars::g_PedFactory) {
						if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
							CVehicle* Vehicle = (CVehicle*)(*(uint64_t*)((Rcx - 200) + 0x50));

							if (Vehicle) {
								if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_vehicle == Vehicle) {
									memcpy(&Transform->m_TransformedMatrix, Vehicle->GetMatrix(), sizeof(Math::Matrix));
									Engine::HandleRotationValuesFromOrder(&Transform->m_TransformedMatrix, &Vars.m_VehHookValue, 2);
									return;
								}
							}
						}
					}
				}
			}
		}

		OgSyncEntityMatrix(Rcx, Rdx);
	}
}