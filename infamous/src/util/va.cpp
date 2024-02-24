#include "stdafx.hpp"
#include "va.hpp"

namespace Utils::VA {
	std::unordered_map<uint32_t, char*> g_Storage;
	std::unordered_map<uint32_t, std::pair<wchar_t*, char*>> g_StorageW;

	const char* VA(const char* Format, ...) {
		if (g_Storage.find(GetCurrentThreadId()) == end(g_Storage)) {
			g_Storage[GetCurrentThreadId()] = new char[0x300];
			//printf("Creating VA buffer for thread %i\n", GetCurrentThreadId());
		}

		char* Buffer = g_Storage[GetCurrentThreadId()];
		memset(Buffer, 0, 0x300);

		va_list ap;
		va_start(ap, Format);
		vsprintf(Buffer, Format, ap);
		va_end(ap);
		return Buffer;
	}

	const char* VAP(char* Buffer, const char* Format, ...) {
		va_list ap;
		va_start(ap, Format);
		vsprintf(Buffer, Format, ap);
		va_end(ap);
		return Buffer;
	}

	const wchar_t* VAW(const char* Format, ...) {
		if (g_StorageW.find(GetCurrentThreadId()) == end(g_StorageW)) {
			g_StorageW[GetCurrentThreadId()].first = new wchar_t[0x100];
			g_StorageW[GetCurrentThreadId()].second = new char[0x300];
			printf("Creating WVA buffer for thread %i\n", GetCurrentThreadId());
		}

		char* Buffer = g_StorageW[GetCurrentThreadId()].second;
		memset(Buffer, 0, 0x300);

		wchar_t* Message = g_StorageW[GetCurrentThreadId()].first;
		memset(Message, 0, 0x100);

		va_list pArgList;
		va_start(pArgList, Format);
		vsprintf(Buffer, Format, pArgList);
		va_end(pArgList);

		mbstowcs(Message, Buffer, strlen(Buffer) + 1);
		return Message;
	}

	void Cleanup() {
		for (auto Ptr : g_Storage) {
			delete[] Ptr.second;
		}

		for (auto Ptr : g_StorageW) {
			delete[] Ptr.second.first;
			delete[] Ptr.second.second;
		}
	}
}