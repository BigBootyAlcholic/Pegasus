#pragma once
#include "stdafx.hpp"
#include "util/math.hpp"

namespace Rage::Invoker {
	namespace Crossmap {
		uint64_t SearchCrossmap(uint64_t hash);
		uint64_t SearchCrossmapMapped(uint64_t hash);
	}

	class NativeContext {
	protected:
		void* m_ReturnData;
		uint32_t m_ArgumentCount;
		void* m_ArgumentData;
		uint32_t m_DataCount;
		Math::Vector3<float>* m_ScriptVectors[4];
		Math::Vector4<float> m_ResultVectors[4];
	public:
		void Reset() {
			m_ArgumentCount = 0;
			m_DataCount = 0;
		}

		template<typename T>
		T GetArgument(int index) {
			uint64_t* _ = (uint64_t*)m_ArgumentData;
			return *(T*)&_[index];
		}

		template<typename T>
		void SetArgument(int index, T value) {
			uint64_t* _ = (uint64_t*)m_ArgumentData;
			*(T*)&_[index] = value;
		}

		template<typename T>
		T GetReturnResult(int index) {
			uint64_t* _ = (uint64_t*)m_ReturnData;
			return *(T*)&_[index];
		}

		template<typename T>
		void SetReturn(int index, T value) {
			uint64_t* _ = (uint64_t*)m_ReturnData;
			*(T*)&_[index] = value;
		}

		void* GetReturnData() {
			return m_ReturnData;
		}

		static void UpdateResultVectors(NativeContext* p) {
			for (uint32_t i = 0; i < p->m_DataCount; i++) {
				p->m_ScriptVectors[i]->m_X = p->m_ResultVectors[i].m_X;
				p->m_ScriptVectors[i]->m_Y = p->m_ResultVectors[i].m_Y;
				p->m_ScriptVectors[i]->m_Z = p->m_ResultVectors[i].m_Z;
			}

			p->m_DataCount = 0;
		}
	};

	class NativeSetup : public NativeContext {
	private:
		uint8_t m_TempBuffer[256];
	public:
		inline NativeSetup() {
			m_ArgumentData = &m_TempBuffer;
			m_ReturnData = &m_TempBuffer;

			m_ArgumentCount = 0;
			m_DataCount = 0;

			memset(m_TempBuffer, 0x0, sizeof(m_TempBuffer));
		}

		uint32_t GetArgumentCount() {
			return m_ArgumentCount;
		}

		template<typename T>
		void Push(T value) {
			memset(&m_TempBuffer[8 * m_ArgumentCount], 0, 8);
			*(T*)&m_TempBuffer[8 * m_ArgumentCount] = value;
			m_ArgumentCount++;
		}

		template<typename T>
		T GetReturn() {
			return *(T*)m_TempBuffer;
		}

		uint64_t* GetReturnPtr() {
			return (uint64_t*)m_TempBuffer;
		}
	};

	typedef void(*NativeHandler)(NativeContext*);

	NativeHandler GetNativeHandler(uint64_t hash);

	void CallNative(uint64_t hash, NativeSetup* setup);

	struct pass {
		template<typename ...type> pass(type...) {}
	};

	template<typename R, typename... meme>
	R Invoke(uint64_t hash, meme&&... args) {
		NativeSetup setup;

		pass{ ([&]() {
			setup.Push(args);
		}(), 1)... };

		CallNative(hash, &setup);
		NativeContext::UpdateResultVectors(&setup);
		return setup.GetReturn<R>();
	}
}