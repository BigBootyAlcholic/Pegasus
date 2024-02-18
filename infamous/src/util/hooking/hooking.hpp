#pragma once
#include "stdafx.hpp"
#include "minhook.h"
#include "rage/classes/script/scrProgram.hpp"
#include "rage/classes/rage/joaat.hpp"
namespace Hooking {
	class Hooking {
	public:

		template <typename T>
		bool Create(const char* Name, uint64_t Address, void* Function, T** Trampoline) {
			MH_STATUS InitStatus = MH_Initialize();//initialize minhook
			if (InitStatus == MH_OK || InitStatus == MH_ERROR_ALREADY_INITIALIZED) {
				if (Address && Function) {
					uint8_t Backup[20];
					memcpy(Backup, (void*)Address, 20);
					
					auto status = MH_CreateHook((LPVOID)Address, Function, (void**)Trampoline);
					LOG_CUSTOM_WARN("Hook", "Hooking %s", Name);
					if ((status == MH_OK || status == MH_ERROR_ALREADY_CREATED) && MH_EnableHook((LPVOID)Address) == MH_OK) {
						DetourHooks Hook;//initialize our detour hooks struct to create a new hook
						Hook.m_EncryptionKey = (uint8_t)(rand() % 255);
						Hook.m_EncryptedName = Name;
						Hook.m_EncryptedAddress = Address;

						memcpy(Hook.m_Original, Backup, 20);
						memcpy(Hook.m_Hooked, (void*)Address, 20);

						for (std::size_t i = 0; i < Hook.m_EncryptedName.length(); i++) {
							Hook.m_EncryptedName[i] ^= Hook.m_EncryptionKey;
						}

						uint8_t* Bytes = (uint8_t*)&Hook.m_EncryptedAddress;
						for (int i = 0; i < 8; i++) {
							Bytes[i] ^= Hook.m_EncryptionKey;
						}

						m_Hooks.push_back(Hook);

						LOG_CUSTOM_SUCCESS("Hook", "Hooked %s @ %llx", Name, Address);

						return true;
					}
				}
			}
			return false;
		}

		void Unload();
		bool Remove(void* Address);
		bool Disable(void* Address);
		bool GetOriginalBytes(std::string Name, uint64_t* Address, uint8_t* Buffer);
		bool GetHookedBytes(std::string Name, uint64_t* Address, uint8_t* Buffer);
		uint64_t GetHookAddress(std::string Name);
	private:
		struct DetourHooks {
			u8 m_EncryptionKey;//random bytes
			std::string m_EncryptedName;//hook name
			u64 m_EncryptedAddress;//location in memory of the hook address
			u8 m_Original[20];//original bytes
			u8 m_Hooked[20];//hooked bytes
		};

		std::vector<DetourHooks> m_Hooks;
	};

	Hooking* GetHooking();


	class IHook
	{
	public:
		IHook() = default;
		virtual ~IHook() = default;

		IHook(IHook&& that) = delete;
		IHook& operator=(IHook&& that) = delete;
		IHook(IHook const&) = delete;
		IHook& operator=(IHook const&) = delete;

		virtual const std::string_view Name() const = 0;

		virtual bool Enable() = 0;
		virtual bool Disable() = 0;

		virtual bool IsEnabled() const = 0;
	};

	//poopoo way but what can i do 
	class VMTHook final : public IHook
	{
	public:
		VMTHook(const std::string_view name, void*** vmtBaseAddr);
		~VMTHook();

		virtual const std::string_view Name() const override;
		virtual bool Enable() override;
		virtual bool Disable() override;
		virtual bool IsEnabled() const override;

		template<typename T>
		T Original(const std::uint32_t idx) const;

		void Hook(const std::uint32_t idx, void* detour);
		void UnHook(const std::uint32_t idx);

		std::size_t VMTSize() const;

	private:
		/**
		 * @brief Get the amount functions in the virtual method table
		 *
		 * @return std::size_t
		 */
		static std::size_t GetVMTSize(void** vmt);

	private:
		const std::string_view m_Name;
		const std::size_t m_VMTSize;
		bool m_Enabled;

		void*** m_VMTBaseAddr;
		void** m_OriginalVMT;
		std::unique_ptr<void* []> m_NewVMT;

	};

	template<typename T>
	inline T VMTHook::Original(const std::uint32_t idx) const
	{
		return reinterpret_cast<T>(m_OriginalVMT[idx]);
	}

	class ScriptHook
	{
	public:
		explicit ScriptHook(Rage::joaat_t script_hash, std::unordered_map<Rage::scrNativeHash, Rage::Invoker::NativeHandler> replacements);
		explicit ScriptHook(Rage::scrProgram* program, std::unordered_map<Rage::scrNativeHash, Rage::Invoker::NativeHandler> replacements);
		~ScriptHook();

		void Ensure();
	private:
		void HookInstance(Rage::scrProgram* program);

		static void scrProgramDetour(Rage::scrProgram* this_, bool free_memory);
		static std::unordered_map<Rage::scrProgram*, ScriptHook*> s_Map;

		Rage::joaat_t m_ScriptHash;
		Rage::scrProgram* m_Program;
		std::unique_ptr<VMTHook> m_VMTHook;
		std::unordered_map<Rage::scrNativeHash, Rage::Invoker::NativeHandler> m_NativeReplacementsPointer;
		std::unordered_map<Rage::scrNativeHash, Rage::Invoker::NativeHandler*> m_NativeHandlerPointer;
	};

	class NativeHooks final
	{
		using NativeDetour = std::pair<Rage::scrNativeHash, Rage::Invoker::NativeHandler>;

		std::unordered_map<Rage::joaat_t, std::vector<NativeDetour>> m_NativeRegistrations;
		std::unordered_map<Rage::scrProgram*, std::unique_ptr<ScriptHook>> m_ScriptHooks;

	public:
		NativeHooks();
		~NativeHooks();

		NativeHooks(const NativeHooks&) = delete;
		NativeHooks(NativeHooks&&) noexcept = delete;
		NativeHooks& operator=(const NativeHooks&) = delete;
		NativeHooks& operator=(NativeHooks&&) noexcept = delete;


		void AddNativeDetour(const std::string_view name, Rage::scrNativeHash hash, Rage::Invoker::NativeHandler detour);

		void AddNativeDetour(const std::string_view name, Rage::joaat_t script_hash, Rage::scrNativeHash hash, Rage::Invoker::NativeHandler detour);

		void HookProgram(Rage::scrProgram* program);

		void UnhookProgram(Rage::scrProgram* program);
	};

	inline NativeHooks* g_NativeHooks{};
}