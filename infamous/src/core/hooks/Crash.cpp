#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/util/notify.hpp"
#include "rage/classes/netsync/data_nodes.hpp"
#include "core/core.hpp"
namespace Hooks {
	
#pragma pack(push, 8)
	struct TaskObject
	{
		uint16_t m_net_id;
		Rage::CDynamicEntity* m_entity;
	};
#pragma pack(pop)

	constexpr uint32_t ValidParachuteModels[] = {
		RAGE_JOAAT("p_parachute_s"),
		RAGE_JOAAT("vw_p_para_bag_vine_s"),
		RAGE_JOAAT("reh_p_para_bag_reh_s_01a"),
		RAGE_JOAAT("xm_prop_x17_scuba_tank"),
		RAGE_JOAAT("lts_p_para_bag_pilot2_s"),
		RAGE_JOAAT("lts_p_para_bag_lts_s"),
		RAGE_JOAAT("p_para_bag_tr_s_01a"),
		RAGE_JOAAT("p_para_bag_xmas_s"),
	};

	bool IsValidParachuteModel(Rage::joaat_t Hash) {
		for (auto& Model : ValidParachuteModels)
			if (Model == Hash)
				return true;

		return false;
	}

	bool FragmentCrashPatchHook(float* a1, float* a2) {
		if (!a1 || !a2)
			return false;

		return OgFragmentCrashPatchHook(a1, a2);
	}

	void InvalidDecalPatchHook(uintptr_t a1, int a2) {
		if (a1 && a2 == 2)
			//*(*(*(a1 + 0x48) + 0x30) + 0x2C8)
			if (const auto ptr = *reinterpret_cast<uintptr_t*>((a1 + 0x48)); ptr)
				if (const auto ptr2 = *reinterpret_cast<uintptr_t*>((ptr + 0x30)); ptr2)
					if (*reinterpret_cast<uintptr_t*>(ptr2 + 0x2C8) == 0)
						return;

		OgInvalidDecalPatchHook(a1, a2);
	}

	void InvalidModsPatchHook(int64_t a1, int64_t a2, int a3, char a4) {
		if (!*(int64_t*)(a1 + 0xD8))
			return;

		OgInvalidModsPatchHook(a1, a2, a3, a4);
	}

	__int64 RenderBigPedHook(__int64 renderer, CPed* ped, __int64 a3, __int64 a4) {
		if (*(int*)(((__int64)(*Patterns::Vars::g_DrawHandlerMgr) + 0x14730)) >= 512) {
			*(int*)(a4 + 4) = -2;
			return a4 + 0x14;
		}
		else {
			return OgRenderBigPedHook(renderer, ped, a3, a4);
		}
	}

	void RenderEntityHook(__int64 renderer, Rage::fwEntity* entity, int unk, bool a4) {
		if (*(int*)(((__int64)(*Patterns::Vars::g_DrawHandlerMgr) + 0x14730)) >= 512) {
			*(int*)(renderer + 4) &= ~0x80000000;
			*(int*)(renderer + 4) &= ~0x40000000;
			*(int*)(renderer + 4) |= (a4 & 1) << 30;
			*(int*)renderer = -2;
		}
		else {
			OgRenderEntityHook(renderer, entity, unk, a4);
		}
	}

	void* RenderPedHook(__int64 renderer, CPed* ped, __int64 a3, __int64 a4) {
		if (*(int*)(((__int64)(*Patterns::Vars::g_DrawHandlerMgr) + 0x14730)) >= 499) {
			return nullptr;
		}
		else {
			return OgRenderPedHook(renderer, ped, a3, a4);
		}
	}

	void SerializeParachuteTaskHook(__int64 info, Rage::CSyncDataBase* serializer) {
		auto object = reinterpret_cast<TaskObject*>(info + 0x30);
		OgSerializeParachuteTaskHook(info, serializer);

		if (object->m_entity && object->m_entity->m_entity_type != 5) {
			//g_pointers->m_gta.m_remove_reference(object->m_entity, &object->m_entity);
			Caller::Call<void>(Patterns::Vars::g_RemoveRefrence, object->m_entity, &object->m_entity);
			Menu::Notify::stacked("Blocked Invalid Parachute Task");
			object->m_entity = nullptr;
			object->m_net_id = 0;
		}
	}

	void SerializePedVariationHook(ClonedTakeOffPedVariationInfo* info, Rage::CSyncDataBase* serializer) {
		OgSerializePedVariationHook(info, serializer);

		if (!IsValidParachuteModel(info->m_prop_hash)) {
			Menu::Notify::stacked("Blocked Invalid Parachute Model");
			info->m_prop_hash = 0;
		}
	}

	int AmbientTaskPatchHook(uint64_t _this, int a2, int a3) {
		if (*(uint64_t*)(_this + 0x100)) {
			return OgAmbientTaskPatchHook(_this, a2, a3);
		}

		return 0;
	}

	int ParachuteTaskPatchHook(uint64_t _this, int a2, int a3) {
		if (a2 == 1 && a3 == 1) {
			if (auto ptr = *(uint64_t*)(_this + 16))
				if (auto ptr2 = *(uint64_t*)(ptr + 80))
					if (auto ptr3 = *(uint64_t*)(ptr2 + 64))
						return OgParachuteTaskPatchHook(_this, a2, a3);
			return 0;
		}
		return OgParachuteTaskPatchHook(_this, a2, a3);
	}

	bool AnimTaskPatchHook(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5) {
		if (!IsValidPtr(a2)) {
			Menu::Notify::stacked("Blocked Invalid Clone Animation");
			return 0;
		}
		return OgAnimTaskPatchHook(a1, a2, a3, a4, a5);
	}

	uint64_t KanyePatchHook(uintptr_t a1, bool a2) {
		if (const auto Pointer = *reinterpret_cast<std::uintptr_t*>(a1 + 0x10); a2 && Pointer) {
			if (const auto Pointer2 = *reinterpret_cast<std::uintptr_t*>(Pointer + 0x10F0); Pointer2) {
				if (const auto Pointer3 = *reinterpret_cast<std::uintptr_t*>(Pointer2 + 0x10); Pointer3) {
					if (const auto Pointer4 = *reinterpret_cast<std::uintptr_t*>(Pointer3 + 0x20); Pointer4) {
						if (const auto Pointer5 = *reinterpret_cast<std::uintptr_t*>(Pointer4 + 0x90); !Pointer5) {
							Menu::Notify::stacked("Blocked Out Of Bounds Track");
							return 0;
						}
					}
				}
			}
		}
		return OgKanyePatchHook(a1, a2);
	}

	void StandPatchHook(__int64 a1, __int64 a2, unsigned int a3, int a4, __int64 a5) {
		if (a3 <= 0) {
			Menu::Notify::stacked("Blocked Stand Signature Crash");
			return OgStandPatchHook(a1, a2, 1, a4, a5);
		}
		return OgStandPatchHook(a1, a2, a3, a4, a5);
	}

	//TODO: see if it works
	bool VehicleControlOutOfBoundsHook(__int64 a1, __m128* a2, __int64 a3, int a4, __int64 a5, __int64 a6, char a7) {

		if (!IsValidPtr((uintptr_t)Core::g_GameAddress + 0xF619DD)) {
			Menu::Notify::stacked("Blocked S5 Crash");
			return true;
		}

		return OgVehicleControlOutOfBoundsHook(a1, a2, a3, a4, a5, a6, a7);
		
	}
}