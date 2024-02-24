#include "stdafx.hpp"
#include "pool_manager.hpp"
#include "util/memory/pattern.hpp"
#include "util/memory/memory.hpp"
#include "core/patterns.hpp"
namespace Menu::Pools {
	void PoolManager::Initialize() {
		Memory::Batch Batch;
		bool out = true;

		Batch.Add({ ("EP"), ("4C 8B 0D ? ? ? ? 44 8B C1 49 8B 41 08") }, [](Memory::Ptr ptr) {
			Patterns::Vars::g_EntityPool = ptr.FromInstruction().As<Rage::EntityPool*>();
		}, out);

		Batch.Add({ ("PP"), ("48 8B 05 ? ? ? ? 41 0F BF C8 0F BF 40 10") }, [](Memory::Ptr ptr) {
			Patterns::Vars::g_PedPool = ptr.FromInstruction().As<Rage::GenericPool*>();
		}, out);

		Batch.Add({ ("VP"), ("48 8B 0D ? ? ? ? 45 33 C9 44 8B C5 BA ? ? ? ? E8 ? ? ? ? 48") }, [](Memory::Ptr ptr) {
			Patterns::Vars::g_VehiclePool = ptr.FromInstruction().As<Rage::VehicleEntityPool*>();
		}, out);

		Batch.Add({ ("OP"), ("48 8B 05 ? ? ? ? 8B 78 10 85 FF") }, [](Memory::Ptr ptr) {
			Patterns::Vars::g_ObjectPool = ptr.FromInstruction().As<Rage::GenericPool*>();
		}, out);

		Batch.Add({ ("PP2"), ("48 8B 05 ? ? ? ? 0F B7 48 10 66 03 CA 66 44 03 C1 41 80 F9 04") }, [](Memory::Ptr ptr) {
			Patterns::Vars::g_PickupPool = ptr.FromInstruction().As<Rage::GenericPool*>();
			}, out);

		Batch.Add({ ("AP"), ("48 8B 0D ? ? ? ? E8 ? ? ? ? 48 85 C0 74 0E 48 8B D6 48 8B C8 E8 ? ? ? ? 48 8B D8 48 89 5F 48") }, [](Memory::Ptr ptr) {
			Patterns::Vars::g_AttachmentPool = ptr.FromInstruction().As<Rage::GenericPool*>();
		}, out);

		auto Region = Memory::Module("GTA5.exe");
		Batch.Run(Region);
	}

	PoolManager* GetPoolManager() {
		static PoolManager Instance;
		return &Instance;
	}
}