#include "stdafx.hpp"
#include "../hooks.hpp"
#include "rage/classes/script/sysMemAllocator.hpp"

namespace Hooks {

	bool IsAllocatedMessage(Rage::netQueuedMessage* msg)
	{
		Rage::datBitBuffer buffer(msg->m_data_buffer, INT_MAX);
		buffer.m_flagBits |= 1;    // read
		buffer.Seek(10);           // size
		return buffer.ReadBits(1); // flags
	}

	void FreeMessage(Rage::netQueuedMessage* msg, Rage::sysMemAllocator* allocator)
	{
		if (msg->m_data_buffer)
			allocator->TryFree(msg->m_data_buffer);

		allocator->TryFree(msg);
	}

	void* AllocateMemoryMessageHook(Rage::netConnection* Rcx, int Rbx) {
		if (!Rcx || !Rbx)
			return nullptr;

		auto memory = reinterpret_cast<Rage::sysMemAllocator*>(Rcx->m_Alllocator)->Allocate(Rbx, 0, 0);

		if (memory)
			return memory;

		LOG_WARN("Failed To Allocate Memory Message");

		Caller::Call<u64>(Patterns::Vars::g_ConnMgrFreeMemory, Rcx->m_NetConnectionMgr);

		memory = reinterpret_cast<Rage::sysMemAllocator*>(Rcx->m_Alllocator)->Allocate(Rbx, 0, 0);

		if (memory)
			return memory;

		LOG_WARN("Failed To Allocate Memory Message 2");

		while (Rcx->m_NormalMessageQueue.m_count)
		{
			auto msg = Rcx->m_NormalMessageQueue.m_first;

			Caller::Call<u64>(Patterns::Vars::g_RemoveMessageFromQueue, &Rcx->m_NormalMessageQueue, msg);

			if (IsAllocatedMessage(msg))
				Caller::Call<u64>(Patterns::Vars::g_RemoveUnackedMessage, &Rcx->m_UnackedReliableMsgList, &msg->word4C);

			FreeMessage(msg, reinterpret_cast<Rage::sysMemAllocator*>(Rcx->m_Alllocator));
		}

		while (Rcx->m_ReliableResendMessageQueue.m_count)
		{
			auto msg = Rcx->m_ReliableResendMessageQueue.m_first;

			Caller::Call<u64>(Patterns::Vars::g_RemoveMessageFromQueue, &Rcx->m_ReliableResendMessageQueue, msg);
			Caller::Call<u64>(Patterns::Vars::g_RemoveUnackedMessage, &Rcx->m_UnackedReliableMsgList,
				&msg->word4C); // messages in this queue are always reliables
			FreeMessage(msg, reinterpret_cast<Rage::sysMemAllocator*>(Rcx->m_Alllocator));
		}


		memory = reinterpret_cast<Rage::sysMemAllocator*>(Rcx->m_Alllocator)->Allocate(Rbx, 0, 0);

		if (memory)
			return memory;

		LOG_WARN("Failed To Allocate Memory Message 3");

		return nullptr;
	}
}