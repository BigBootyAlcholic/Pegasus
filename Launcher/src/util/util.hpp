#pragma once
#include "stdafx.hpp"

namespace Util {
	void* AllocateMemory(int len);
	char* Va(const char* text, ...);
	wchar_t* Vaw(const char* text, ...);
	std::string CreateRandomString(size_t length);
	std::string ExtractPath(std::string strFilePath);
	std::string BuildFilePath(std::string filename);
	std::string ConvertBytesToString(BYTE* data, int len);
	std::string Rot13(std::string input);
	std::string Rot13Number(std::string input);
	void TimedClose(int sec, void(*callback)(int));
	void TimedMessage(int sec, void(*callback)(int));
}