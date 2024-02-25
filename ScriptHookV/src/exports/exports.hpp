#pragma once
#include "stdafx.hpp"
#include "core/core.hpp"
#define DESTRINGIFY(v) x##v
#define EXPORT __declspec(dllexport)
typedef void(*TKeyboardFn)(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);
typedef void(*PresentCallback)(void*);

enum eGameVersion : int {
	VER_1_0_335_2_STEAM,
	VER_1_0_335_2_NOSTEAM,
	VER_1_0_350_1_STEAM,
	VER_1_0_350_2_NOSTEAM,
	VER_1_0_372_2_STEAM,
	VER_1_0_372_2_NOSTEAM,
	VER_1_0_393_2_STEAM,
	VER_1_0_393_2_NOSTEAM,
	VER_1_0_393_4_STEAM,
	VER_1_0_393_4_NOSTEAM,
	VER_1_0_463_1_STEAM,
	VER_1_0_463_1_NOSTEAM,
	VER_1_0_505_2_STEAM,
	VER_1_0_505_2_NOSTEAM,
	VER_1_0_573_1_STEAM,
	VER_1_0_573_1_NOSTEAM,
	VER_1_0_617_1_STEAM,
	VER_1_0_617_1_NOSTEAM,
	VER_1_0_678_1_STEAM,
	VER_1_0_678_1_NOSTEAM,
	VER_1_0_757_2_STEAM,
	VER_1_0_757_2_NOSTEAM,
	VER_1_0_757_4_STEAM,
	VER_1_0_757_4_NOSTEAM,
	VER_1_0_791_2_STEAM,
	VER_1_0_791_2_NOSTEAM,
	VER_1_0_877_1_STEAM,
	VER_1_0_877_1_NOSTEAM,
	VER_1_0_944_2_STEAM,
	VER_1_0_944_2_NOSTEAM,
	VER_1_0_1011_1_STEAM,
	VER_1_0_1011_1_NOSTEAM,
	VER_1_0_1032_1_STEAM,
	VER_1_0_1032_1_NOSTEAM,
	VER_1_0_1103_2_STEAM,
	VER_1_0_1103_2_NOSTEAM,
	VER_1_0_1180_2_STEAM,
	VER_1_0_1180_2_NOSTEAM,
	VER_1_0_1290_1_STEAM,
	VER_1_0_1290_1_NOSTEAM,
	VER_1_0_1365_1_STEAM,
	VER_1_0_1365_1_NOSTEAM,
	VER_1_0_1493_0_STEAM,
	VER_1_0_1493_0_NOSTEAM,
	VER_1_0_1493_1_STEAM,
	VER_1_0_1493_1_NOSTEAM,
	VER_1_0_1604_0_STEAM,
	VER_1_0_1604_0_NOSTEAM,
	VER_1_0_1604_1_STEAM,
	VER_1_0_1604_1_NOSTEAM,
	VER_1_0_1737_0_STEAM,
	VER_1_0_1737_0_NOSTEAM,
	VER_1_0_1737_6_STEAM,
	VER_1_0_1737_6_NOSTEAM,
	VER_1_0_1868_0_STEAM,
	VER_1_0_1868_0_NOSTEAM,
	VER_1_0_1868_1_STEAM,
	VER_1_0_1868_1_NOSTEAM,
	VER_1_0_1868_4_EPIC,
	VER_1_0_2060_0_STEAM,
	VER_1_0_2060_0_NOSTEAM,
	VER_1_0_2060_1_STEAM,
	VER_1_0_2060_1_NOSTEAM,
	VER_1_0_2189_0_STEAM,
	VER_1_0_2189_0_NOSTEAM,
	VER_1_0_2545_STEAM,
	VER_1_0_2545_0_NOSTEAM,
	VER_1_0_2612_STEAM,
	VER_1_0_2612_0_NOSTEAM,
	VER_SIZE,
	VER_UNK = -1
};

EXPORT void scriptRegister(HMODULE module, void(*LP_SCRIPT_MAIN)()) {
	auto fn{ (decltype(&scriptRegister))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(module, LP_SCRIPT_MAIN);
}
EXPORT void scriptRegisterAdditionalThread(HMODULE module, void(*LP_SCRIPT_MAIN)()) {
	auto fn{ (decltype(&scriptRegisterAdditionalThread))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(module, LP_SCRIPT_MAIN);
}
EXPORT void scriptUnregister(HMODULE module) {
	auto fn{ (void(*)(HMODULE module))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(module);
}
EXPORT void scriptUnregister(void(*LP_SCRIPT_MAIN)()) {
	auto fn{ (void(*)(void(*LP_SCRIPT_MAIN)()))GetProcAddress(Global::g_ExportModule, __FUNCTION__"Depricated") };
	if (!fn)
		return;
	return fn(LP_SCRIPT_MAIN);
}
EXPORT void nativeInit(uint64_t hash) {
	auto fn{ (decltype(&nativeInit))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(hash);
}
EXPORT void nativePush64(uint64_t val) {
	auto fn{ (decltype(&nativePush64))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(val);
}
EXPORT PUINT64 nativeCall() {
	auto fn{ (decltype(&nativeCall))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn();
}
EXPORT PUINT64 getGlobalPtr(int index) {
	auto fn{ (decltype(&getGlobalPtr))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(index);
}
EXPORT int createTexture(const char* fileName) {
	auto fn{ (decltype(&createTexture))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(fileName);
}
EXPORT void drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a) {
	auto fn{ (decltype(&drawTexture))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(id, index, level, time, sizeX, sizeY, centerX, centerY, posX, posY, rotation, screenHeightScaleFactor, r, g, b, a);
}
EXPORT void scriptWait(unsigned long waitTime) {
	auto fn{ (decltype(&scriptWait))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(waitTime);
}
EXPORT void keyboardHandlerRegister(TKeyboardFn function) {
	auto fn{ (decltype(&keyboardHandlerRegister))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(function);
}
EXPORT void keyboardHandlerUnregister(TKeyboardFn function) {
	auto fn{ (decltype(&keyboardHandlerUnregister))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(function);
}
EXPORT eGameVersion getGameVersion() {
	auto fn{ (decltype(&getGameVersion))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return eGameVersion::VER_UNK;
	return fn();
}
EXPORT BYTE* getScriptHandleBaseAddress(int handle) {
	auto fn{ (decltype(&getScriptHandleBaseAddress))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(handle);
}
EXPORT void presentCallbackUnregister(PresentCallback cb) {
	auto fn{ (decltype(&presentCallbackUnregister))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	return fn(cb);
}
EXPORT void presentCallbackRegister(PresentCallback cb) {
	auto fn{ (decltype(&presentCallbackRegister))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	return fn(cb);
}
EXPORT int worldGetAllVehicles(int* arr, int arrSize) {
	auto fn{ (decltype(&worldGetAllVehicles))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(arr, arrSize);
}
EXPORT int worldGetAllPeds(int* arr, int arrSize) {
	auto fn{ (decltype(&worldGetAllPeds))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(arr, arrSize);
}
EXPORT int worldGetAllObjects(int* arr, int arrSize) {
	auto fn{ (decltype(&worldGetAllObjects))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(arr, arrSize);
}
EXPORT int worldGetAllPickups(int* arr, int arrSize) {
	auto fn{ (decltype(&worldGetAllPickups))GetProcAddress(Global::g_ExportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(arr, arrSize);
}