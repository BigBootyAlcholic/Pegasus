#pragma once
#include "stdafx.hpp"
#include "core/patterns.hpp"

namespace Menu {
	class Global {
	private:
		void* m_handle;

	public:
		Global(int index) : m_handle(&::Patterns::Vars::g_GlobalCache[index >> 18 & 0x3F][index & 0x3FFFF]) {}
		Global(void* p) : m_handle(p) {}
		Global(const Global& copy) : m_handle(copy.m_handle) {}

		Global At(int index) const {
			return Global(reinterpret_cast<void**>(this->m_handle) + (index));
		}

		Global At(int index, int size) const {
			return this->At(1 + (index * size));
		}

		template <typename T>
		T* Get() {
			return reinterpret_cast<T*>(this->m_handle);
		}

		template <typename T>
		T& As() {
			return *this->Get<T>();
		}
	};


	class ScriptLocalStack {
	public:
		explicit ScriptLocalStack(Rage::scrThread* thread, std::size_t index);
		explicit ScriptLocalStack(PVOID stack, std::size_t index);

		ScriptLocalStack At(std::ptrdiff_t index);
		ScriptLocalStack At(std::ptrdiff_t index, std::size_t size);

		template<typename T>
		std::enable_if_t<std::is_pointer_v<T>, T> As() {
			return static_cast<T>(Get());
		}

		template<typename T>
		std::enable_if_t<std::is_lvalue_reference_v<T>, T> As() {
			return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(Get());
		}

	private:
		void* Get();
		std::size_t m_index;
		PVOID m_stack;
	};

	inline ScriptLocalStack::ScriptLocalStack(Rage::scrThread* thread, std::size_t index) :
		m_index(index),
		m_stack(thread->m_stack)
	{
	}
	inline ScriptLocalStack::ScriptLocalStack(PVOID stack, std::size_t index) :
		m_index(index),
		m_stack(stack)
	{
	}

	inline ScriptLocalStack ScriptLocalStack::At(std::ptrdiff_t index) {
		return ScriptLocalStack(m_stack, m_index + index);
	}

	inline ScriptLocalStack ScriptLocalStack::At(std::ptrdiff_t index, std::size_t size) {
		return ScriptLocalStack(m_stack, m_index + 1 + (index * size));
	}

	inline void* ScriptLocalStack::Get() {
		return reinterpret_cast<uintptr_t*>((uintptr_t)m_stack + (m_index * sizeof(uintptr_t)));
	}

	class ScriptLocal {
	private:
		int m_Index;
		uint64_t m_Storage;

	public:
		ScriptLocal(uint64_t Storage, int Index)
			: m_Storage(Storage), m_Index(Index) {}


		ScriptLocal At(int Index) { m_Index += Index; return *this; }
		ScriptLocal At(int Index, int Size) { return At(1 + (Index * Size)); }

		template <typename T>
		T* Get() {
			return reinterpret_cast<T*>(Data());
		}

		template <typename T>
		T& As() {
			return *this->Get<T>();
		}

	private:
		void* Data() { return (void*)(m_Storage + (m_Index << 3)); }
	};
}