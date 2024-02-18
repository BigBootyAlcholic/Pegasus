#include "stdafx.hpp"
#include "hooking.hpp"
#include "rage/invoker/native_invoker.hpp"
#include "core/patterns.hpp"
#include "core/hooks.hpp"
namespace Hooking {
	bool Hooking::Remove(void* Address) {
		return MH_RemoveHook(Address) == MH_OK;
	}

	bool Hooking::Disable(void* Address) {
		return MH_DisableHook(Address) == MH_OK;
	}

	bool Hooking::GetOriginalBytes(std::string Name, uint64_t* Address, uint8_t* Buffer) {
		for (auto& Hook : m_Hooks) {
			std::string Name = Hook.m_EncryptedName;
			for (std::size_t i = 0; i < Name.length(); i++) {
				Name[i] ^= Hook.m_EncryptionKey;
			}

			if (!Name.compare(Name)) {
				uint64_t LocalEnc = Hook.m_EncryptedAddress;
				uint8_t* Bytes = (uint8_t*)&LocalEnc;
				for (int i = 0; i < 8; i++) {
					Bytes[i] ^= Hook.m_EncryptionKey;
				}

				*Address = LocalEnc;
				memcpy(Buffer, Hook.m_Original, 20);
				return true;
			}
		}

		return false;
	}

	bool Hooking::GetHookedBytes(std::string Name, uint64_t* Address, uint8_t* Buffer) {
		for (auto& Hook : m_Hooks) {
			std::string Name = Hook.m_EncryptedName;
			for (std::size_t i = 0; i < Name.length(); i++) {
				Name[i] ^= Hook.m_EncryptionKey;
			}

			if (!Name.compare(Name)) {
				uint64_t LocalEnc = Hook.m_EncryptedAddress;
				uint8_t* Bytes = (uint8_t*)&LocalEnc;
				for (int i = 0; i < 8; i++) {
					Bytes[i] ^= Hook.m_EncryptionKey;
				}

				*Address = LocalEnc;
				memcpy(Buffer, Hook.m_Hooked, 20);
				return true;
			}
		}

		return false;
	}

	uint64_t Hooking::GetHookAddress(std::string Name) {
		for (auto& Hook : m_Hooks) {
			std::string Name = Hook.m_EncryptedName;
			for (std::size_t i = 0; i < Name.length(); i++) {
				Name[i] ^= Hook.m_EncryptionKey;
			}

			if (!Name.compare(Name)) {
				uint64_t LocalEnc = Hook.m_EncryptedAddress;
				uint8_t* Bytes = (uint8_t*)&LocalEnc;
				for (int i = 0; i < 8; i++) {
					Bytes[i] ^= Hook.m_EncryptionKey;
				}

				return LocalEnc;
			}
		}

		return 0;
	}

	void Hooking::Unload() {
		for (auto& Hook : m_Hooks) {
			std::string Name = Hook.m_EncryptedName;
			for (std::size_t i = 0; i < Name.length(); i++) {
				Name[i] ^= Hook.m_EncryptionKey;
			}

			LOG_CUSTOM("Hook", "Unhooked %s", Name.c_str());

			uint8_t* Bytes = (uint8_t*)&Hook.m_EncryptedAddress;
			for (int i = 0; i < 8; i++) {
				Bytes[i] ^= Hook.m_EncryptionKey;
			}

			MH_QueueDisableHook((void*)Hook.m_EncryptedAddress);
		}

		MH_ApplyQueued();
		MH_Uninitialize();
	}

	Hooking* GetHooking() {
		static Hooking Instance;
		return &Instance;
	}


	//D:

	VMTHook::VMTHook(const std::string_view name, void*** vmtBaseAddr) :
		m_Name(name),
		m_VMTSize(VMTHook::GetVMTSize(*vmtBaseAddr)),
		m_Enabled(false),
		m_VMTBaseAddr(vmtBaseAddr)
	{
		m_OriginalVMT = *vmtBaseAddr;

		m_NewVMT = std::make_unique<void* []>(m_VMTSize);
		memcpy(m_NewVMT.get(), m_OriginalVMT, m_VMTSize * sizeof(void*));
	}

	VMTHook::~VMTHook()
	{
		if (m_Enabled)
		{
			*m_VMTBaseAddr = m_OriginalVMT;
		}
	}

	const std::string_view VMTHook::Name() const
	{
		return m_Name;
	}

	bool VMTHook::Enable()
	{
		if (m_Enabled)
			return false;

		*m_VMTBaseAddr = m_NewVMT.get();
		m_Enabled = true;

		LOG_CUSTOM_SUCCESS("HookV", "Hooked %s", m_Name);

		return true;
	}

	bool VMTHook::Disable()
	{
		if (!m_Enabled)
			return false;

		*m_VMTBaseAddr = m_OriginalVMT;
		m_Enabled = false;

		

		return true;
	}

	bool VMTHook::IsEnabled() const
	{
		return m_Enabled;
	}

	void VMTHook::Hook(const std::uint32_t idx, void* detour)
	{
		m_NewVMT[idx] = detour;
	}

	void VMTHook::UnHook(const std::uint32_t idx)
	{
		m_NewVMT[idx] = m_OriginalVMT[idx];
	}

	std::size_t VMTHook::VMTSize() const
	{
		return m_VMTSize;
	}

	std::size_t VMTHook::GetVMTSize(void** vmt)
	{
#ifdef _WIN64
		constexpr auto MAX_PTR_VAL = 0x000F000000000000;
#else
		constexpr auto MAX_PTR_VAL = 0xFFF00000;
#endif
		size_t i = 0;
		for (auto ptr = vmt[i]; ptr && ptr > reinterpret_cast<void*>(0x10000) && ptr < reinterpret_cast<void*>(MAX_PTR_VAL); ptr = vmt[i])
			i++;

		return i;
	}



	inline std::unordered_map<Rage::scrProgram*, ScriptHook*> ScriptHook::s_Map;

	ScriptHook::ScriptHook(Rage::joaat_t script_hash, std::unordered_map<Rage::scrNativeHash, Rage::Invoker::NativeHandler> replacements) :
		m_ScriptHash(script_hash),
		m_NativeReplacementsPointer(std::move(replacements))
	{
		Ensure();
	}

	ScriptHook::ScriptHook(Rage::scrProgram* program, std::unordered_map<Rage::scrNativeHash, Rage::Invoker::NativeHandler> replacements) :
		m_NativeReplacementsPointer(std::move(replacements))
	{
		HookInstance(program);
	}

	ScriptHook::~ScriptHook()
	{
		if (m_Program)
		{
			for (auto [hash, handler_ptr] : m_NativeHandlerPointer)
			{
				auto og_handler = Rage::Invoker::GetNativeHandler(hash);
				*handler_ptr = og_handler;
			}
		}

		if (m_VMTHook)
		{
			m_VMTHook->Disable();

			if (m_Program)
				s_Map.erase(m_Program);
		}
	}

	void ScriptHook::Ensure()
	{
		if (m_VMTHook)
			return;

		if (auto program = Patterns::Vars::g_ProgramTable->find_script(m_ScriptHash))
		{
			if (program->is_valid())
			{
				HookInstance(program);
				LOG_CUSTOM_WARN("HookS", "Hooked %s", program->m_name);

			}
		}
	}

	void ScriptHook::HookInstance(Rage::scrProgram* program)
	{
		m_Program = program;
		s_Map.emplace(m_Program, this);
		m_VMTHook = std::make_unique<VMTHook>("ScriptHook", (void***)m_Program);
		m_VMTHook->Hook(0, &scrProgramDetour);

		for (auto [replacement_hash, replacement_handler] : m_NativeReplacementsPointer)
		{
			auto hash = replacement_hash;
			Rage::Invoker::Crossmap::SearchCrossmapMapped(hash);

			auto og_handler = Rage::Invoker::GetNativeHandler(hash);
			if (!og_handler)
				continue;

			auto handler_ptr = m_Program->get_address_of_native_entrypoint(og_handler);
			if (!handler_ptr)
				continue;

			m_NativeHandlerPointer.emplace(hash, reinterpret_cast<Rage::Invoker::NativeHandler*>(handler_ptr));
			*handler_ptr = replacement_handler;
		}
	}

	void ScriptHook::scrProgramDetour(Rage::scrProgram* this_, bool free_memory)
	{
		if (auto it = s_Map.find(this_); it != s_Map.end())
		{
			auto hook = it->second;

			hook->m_Program = nullptr;
			s_Map.erase(it);

			auto og_func = hook->m_VMTHook->Original<decltype(&scrProgramDetour)>(0);
			hook->m_VMTHook->Disable();
			hook->m_VMTHook.reset();

			og_func(this_, free_memory);
		}
	}

	auto ALL_SCRIPT_HASH = Joaat("ALL_SCRIPTS");

	NativeHooks::NativeHooks()
	{
		/*AddNativeDetour("WAIT", Joaat("main_persistent"), 0x4EDE34FBADD967A6, Native::Wait);
		AddNativeDetour("IDP", Joaat("shop_controller"), 0x812595A0644CE1DE, Native::IsDlcPresent);
		AddNativeDetour("SCPW", 0xADF692B254977C0C, Native::SetCurrentPedWeapon);
		AddNativeDetour("DCA", 0xFE99B66D079CF6BC, Native::DisableControlAction);
		AddNativeDetour("HFWW", 0xEB354E5376BC81A7, Native::HudForceWeaponWheel);*/


		AddNativeDetour("SGI", Joaat("main_persistent"), 0x767FBC2AC802EF3D, Hooks::StatGetIntHook);
		AddNativeDetour("SWMWH", Joaat("shop_controller"), 0xDC38CC1E35B6A5D7, Hooks::SetWarningMessageWithHeader);
		AddNativeDetour("NSH", Joaat("maintransition"), 0x6F3D4ED9BEE4E61D, Hooks::SetWarningMessageWithHeader);
		/*
		AddNativeDetour("SCTNS", 0x6BFB12CE158E3DD4, Hooks::_0x6BFB12CE158E3DD4);       // Stops news.
		AddNativeDetour("SCTNSS", 0xFE4C1D0D3B9CC17E, Hooks::_0xFE4C1D0D3B9CC17E); // Stops news.

		AddNativeDetour("STMP", Joaat("maintransition"), 0x933BBEEB8C61B5F4, Hooks::IsSwitchToMultiFirstPartFinished);
		AddNativeDetour("SFE", Joaat("maintransition"), 0x198F77705FA0931D, Hooks::SetFocusEntity);
		AddNativeDetour("HHAR", Joaat("maintransition"), 0x719FF505F097FD20, Hooks::HideHudAndRadar);
		AddNativeDetour("AFM", Joaat("maintransition"), 0xEF01D36B9C9D0C7B, Hooks::ActivateFrontendMenu); // Let's you controll your ped when going sp to mp.
		AddNativeDetour("RFM", Joaat("maintransition"), 0x10706DC6AD2D49C0, Hooks::RestartFrontendMenu); // Let's you controll your ped when going sp to mp.
		AddNativeDetour("TPRP", Joaat("maintransition"), 0xDFC252D8A3E15AB7, Hooks::TogglePauseRenderPhase);
		AddNativeDetour("SEV", Joaat("maintransition"), 0xEA1C610A04DB6BBB, Hooks::SetEntityVisible); // Makes you visible.
		AddNativeDetour("SEC", Joaat("maintransition"), 0x06843DA7060A026B, Hooks::SetEntityCoords); // Prevents the game from teleporting you.
		AddNativeDetour("SEC2", Joaat("maintransition"), 0x1A9205C1B9EE827F, Hooks::SetEntityCollision); // Prevents you from falling.
		AddNativeDetour("SPC", Joaat("maintransition"), 0x8D32347D6D4C40A2, Hooks::SetPlayerControl); // Allows controll in session switch.
		AddNativeDetour("FEP", Joaat("maintransition"), 0x428CA6DBD1094446, Hooks::FreezeEntityPosition); // Allows controll in session switch.
		AddNativeDetour("RLP", Joaat("maintransition"), 0xEA23C49EAA83ACFB, Hooks::ReviveLocalPlayer); // Prevents player from teleporting after switch*/


		for (auto& entry : *Patterns::Vars::g_ProgramTable)
			if (entry.m_program)
				HookProgram(entry.m_program);

		g_NativeHooks = this;
	}

	NativeHooks::~NativeHooks()
	{
		m_ScriptHooks.clear();
		g_NativeHooks = nullptr;
	}

	void NativeHooks::AddNativeDetour(const std::string_view name, Rage::scrNativeHash hash, Rage::Invoker::NativeHandler detour)
	{
		AddNativeDetour(name, ALL_SCRIPT_HASH, hash, detour);
	}

	void NativeHooks::AddNativeDetour(const std::string_view name, Rage::joaat_t script_hash, Rage::scrNativeHash hash, Rage::Invoker::NativeHandler detour)
	{
		if (const auto& it = m_NativeRegistrations.find(script_hash); it != m_NativeRegistrations.end())
		{
			it->second.emplace_back(hash, detour);
			return;
		}

		m_NativeRegistrations.emplace(script_hash, std::vector<NativeDetour>({ { hash, detour } }));

		LOG_CUSTOM_SUCCESS("HookN", std::format("Hooked {}", name).c_str());
	}

	void NativeHooks::HookProgram(Rage::scrProgram* program)
	{
		std::unordered_map<Rage::scrNativeHash, Rage::Invoker::NativeHandler> native_replacements;
		const auto script_hash = program->m_name_hash;

		// Functions that need to be detoured for all scripts
		if (const auto& pair = m_NativeRegistrations.find(ALL_SCRIPT_HASH); pair != m_NativeRegistrations.end())
			for (const auto& native_hook_reg : pair->second)
				native_replacements.insert(native_hook_reg);

		// Functions that only need to be detoured for a specific script
		if (const auto& pair = m_NativeRegistrations.find(script_hash); pair != m_NativeRegistrations.end())
			for (const auto& native_hook_reg : pair->second)
				native_replacements.insert(native_hook_reg);

		if (!native_replacements.empty())
		{
			m_ScriptHooks.emplace(
				program,
				std::make_unique<ScriptHook>(program, native_replacements)
			);
		}
	}

	void NativeHooks::UnhookProgram(Rage::scrProgram* program)
	{
		m_ScriptHooks.erase(program);
	}
}