#pragma once
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS
#include <windows.h>
#include <winternl.h>
#include <d3d11.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <cassert>
#include <cinttypes>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <type_traits>
#include <map>
#include <unordered_map>
#include <vector>
#include <array>
#include <wrl/client.h>
#include <random>
#include <filesystem>
#include <timeapi.h>
#include <span>
#include <set>
#include <shlobj.h>

#include "core/log/log.hpp"


#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4244)

#define CURL_STATICLIB
#include "../../includes/CURL/curl.h"
#pragma comment(lib, "../includes/CURL/libcurl_imp.lib")
template<typename T, int N>
constexpr int NUMOF(T(&)[N]) { return N; }



template<typename T>
inline bool IsValidPtr(T Ptr) {
	uint64_t Address = (uint64_t)Ptr;
	if (Address < 0x5000) return false;
	if ((Address & 0xFFFFFFFF) == 0xFFFFFFFF) return false;
	if ((Address & 0xFFFFFFFF) <= 0xFF) return false;
	if (Address > 0xFFFFFFFFFFFF) return false;
	return true;
}


constexpr char CharacterMap[] = {
	'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
	'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
	'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
	'_', '_', '_', '_', '_', '_', '_', '.', '/', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'_', '_',  '_', '_', '_', '_',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
};

constexpr static __forceinline uint32_t JenkinsHash32(const char* String, uint32_t CurrentHashValue = NULL) {
	while (*String != NULL) {
		CurrentHashValue += CharacterMap[*(String++)];
		CurrentHashValue += (CurrentHashValue << 10);
		CurrentHashValue ^= (CurrentHashValue >> 6);
	}

	CurrentHashValue += (CurrentHashValue << 3);
	CurrentHashValue ^= (CurrentHashValue >> 11);
	CurrentHashValue += (CurrentHashValue << 15);

	return CurrentHashValue;
}

#define joaat(String) \
    []( ) -> auto { \
        constexpr auto HashValue = JenkinsHash32( String ); \
        \
        return HashValue; \
    }( )

#define Joaat(String) \
    []( ) -> auto { \
        constexpr auto HashValue = JenkinsHash32( String ); \
        \
        return HashValue; \
    }( )


#undef min
#undef max
#undef GetTempPath

#undef GetCurrentProcess
#define GetCurrentProcess() (HANDLE)-1

using namespace std::chrono;
using namespace std::chrono_literals;

//rust defines
using u64 = uint64_t;
using i64 = int64_t;
using u32 = uint32_t;
using i32 = int32_t;
using u16 = uint16_t;
using i16 = int16_t;
using u8 = uint8_t;
using i8 = int8_t;
using cc = const char;
using db = double;
using s64 = i64;
using s32 = i32;
using s16 = i16;
using s8 = i8;
#include "rage/classes/network/CNetGamePlayer.hpp"
#include "rage/classes/enums/enums.hpp"
#include "rage/invoker/natives.hpp"


inline const char* get_vehicle_class_name(int id) {
	std::stringstream ss; ss << "VEH_CLASS_" << id;
	return Native::_GetLabelText(ss.str().c_str()) == "NULL" ? "Unknown Class" : Native::_GetLabelText(ss.str().c_str());
}

inline CNetGamePlayer* g_SyncingPlayer = nullptr;
inline eNetObjType g_SyncingObjectType = (eNetObjType)-1;

struct ProtectionEventContext {
	bool m_Block;
	bool m_Redirect;
	bool m_Notify;
	bool m_CanRedirect;

	std::string m_Tooltip = "";
};

namespace Rage {
#pragma pack(push, 1)
	class HashList
	{
	public:
		std::uint64_t* m_List; //0x0000
		std::int32_t m_Max; //0x0008
		char pad_000C[4]; //0x000C
		std::int32_t m_Free; //0x0010
		char pad_0014[4]; //0x0014
		std::uint64_t m_ItemSize; //0x0018
		char pad_0020[16]; //0x0020
		std::uint32_t* m_Data; //0x0030
		char pad_0038[16]; //0x0038
		std::int32_t m_Occupied; //0x0048
		const std::uint64_t GetHashFromList(std::int32_t index) const {
			if ((this->m_Data[index >> 5] >> (index & 0x1F)) & 1)
				return this->m_List[index];
			return NULL;
		}
	}; // Size: 0x004C
#pragma pack(pop)
}


enum eTimeCycleVar : __int32
{
	TCVAR_LIGHT_DIR_COL_R = 0x0,
	TCVAR_LIGHT_DIR_COL_G = 0x1,
	TCVAR_LIGHT_DIR_COL_B = 0x2,
	TCVAR_LIGHT_DIR_MULT = 0x3,
	TCVAR_LIGHT_DIRECTIONAL_AMB_COL_R = 0x4,
	TCVAR_LIGHT_DIRECTIONAL_AMB_COL_G = 0x5,
	TCVAR_LIGHT_DIRECTIONAL_AMB_COL_B = 0x6,
	TCVAR_LIGHT_DIRECTIONAL_AMB_INTENSITY = 0x7,
	TCVAR_LIGHT_DIRECTIONAL_AMB_INTENSITY_MULT = 0x8,
	TCVAR_LIGHT_DIRECTIONAL_AMB_BOUNCE_ENABLED = 0x9,
	TCVAR_LIGHT_AMB_DOWN_WRAP = 0xA,
	TCVAR_LIGHT_NATURAL_AMB_DOWN_COL_R = 0xB,
	TCVAR_LIGHT_NATURAL_AMB_DOWN_COL_G = 0xC,
	TCVAR_LIGHT_NATURAL_AMB_DOWN_COL_B = 0xD,
	TCVAR_LIGHT_NATURAL_AMB_DOWN_INTENSITY = 0xE,
	TCVAR_LIGHT_NATURAL_AMB_BASE_COL_R = 0xF,
	TCVAR_LIGHT_NATURAL_AMB_BASE_COL_G = 0x10,
	TCVAR_LIGHT_NATURAL_AMB_BASE_COL_B = 0x11,
	TCVAR_LIGHT_NATURAL_AMB_BASE_INTENSITY = 0x12,
	TCVAR_LIGHT_NATURAL_AMB_BASE_INTENSITY_MULT = 0x13,
	TCVAR_LIGHT_NATURAL_PUSH = 0x14,
	TCVAR_LIGHT_AMBIENT_BAKE_RAMP = 0x15,
	TCVAR_LIGHT_ARTIFICIAL_INT_AMB_DOWN_COL_R = 0x16,
	TCVAR_LIGHT_ARTIFICIAL_INT_AMB_DOWN_COL_G = 0x17,
	TCVAR_LIGHT_ARTIFICIAL_INT_AMB_DOWN_COL_B = 0x18,
	TCVAR_LIGHT_ARTIFICIAL_INT_AMB_DOWN_INTENSITY = 0x19,
	TCVAR_LIGHT_ARTIFICIAL_INT_AMB_BASE_COL_R = 0x1A,
	TCVAR_LIGHT_ARTIFICIAL_INT_AMB_BASE_COL_G = 0x1B,
	TCVAR_LIGHT_ARTIFICIAL_INT_AMB_BASE_COL_B = 0x1C,
	TCVAR_LIGHT_ARTIFICIAL_INT_AMB_BASE_INTENSITY = 0x1D,
	TCVAR_LIGHT_ARTIFICIAL_EXT_AMB_DOWN_COL_R = 0x1E,
	TCVAR_LIGHT_ARTIFICIAL_EXT_AMB_DOWN_COL_G = 0x1F,
	TCVAR_LIGHT_ARTIFICIAL_EXT_AMB_DOWN_COL_B = 0x20,
	TCVAR_LIGHT_ARTIFICIAL_EXT_AMB_DOWN_INTENSITY = 0x21,
	TCVAR_LIGHT_ARTIFICIAL_EXT_AMB_BASE_COL_R = 0x22,
	TCVAR_LIGHT_ARTIFICIAL_EXT_AMB_BASE_COL_G = 0x23,
	TCVAR_LIGHT_ARTIFICIAL_EXT_AMB_BASE_COL_B = 0x24,
	TCVAR_LIGHT_ARTIFICIAL_EXT_AMB_BASE_INTENSITY = 0x25,
	TCVAR_PED_LIGHT_COL_R = 0x26,
	TCVAR_PED_LIGHT_COL_G = 0x27,
	TCVAR_PED_LIGHT_COL_B = 0x28,
	TCVAR_PED_LIGHT_MULT = 0x29,
	TCVAR_PED_LIGHT_DIRECTION_X = 0x2A,
	TCVAR_PED_LIGHT_DIRECTION_Y = 0x2B,
	TCVAR_PED_LIGHT_DIRECTION_Z = 0x2C,
	TCVAR_LIGHT_AMB_OCC_MULT = 0x2D,
	TCVAR_LIGHT_AMB_OCC_MULT_PED = 0x2E,
	TCVAR_LIGHT_AMB_OCC_MULT_VEH = 0x2F,
	TCVAR_LIGHT_AMB_OCC_MULT_PROP = 0x30,
	TCVAR_LIGHT_AMB_VOLUMES_IN_DIFFUSE = 0x31,
	TCVAR_LIGHT_SSAO_INTEN = 0x32,
	TCVAR_LIGHT_SSAO_TYPE = 0x33,
	TCVAR_LIGHT_SSAO_CP_STRENGTH = 0x34,
	TCVAR_LIGHT_SSAO_QS_STRENGTH = 0x35,
	TCVAR_LIGHT_PED_RIM_MULT = 0x36,
	TCVAR_LIGHT_DYNAMIC_BAKE_TWEAK = 0x37,
	TCVAR_LIGHT_VEHICLE_SECOND_SPEC_OVERRIDE = 0x38,
	TCVAR_LIGHT_VEHICLE_INTENSITY_SCALE = 0x39,
	TCVAR_LIGHT_DIRECTION_OVERRIDE = 0x3A,
	TCVAR_LIGHT_DIRECTION_OVERRIDE_OVERRIDES_SUN = 0x3B,
	TCVAR_SUN_DIRECTION_X = 0x3C,
	TCVAR_SUN_DIRECTION_Y = 0x3D,
	TCVAR_SUN_DIRECTION_Z = 0x3E,
	TCVAR_MOON_DIRECTION_X = 0x3F,
	TCVAR_MOON_DIRECTION_Y = 0x40,
	TCVAR_MOON_DIRECTION_Z = 0x41,
	TCVAR_LIGHT_RAY_COL_R = 0x42,
	TCVAR_LIGHT_RAY_COL_G = 0x43,
	TCVAR_LIGHT_RAY_COL_B = 0x44,
	TCVAR_LIGHT_RAY_MULT = 0x45,
	TCVAR_LIGHT_RAY_UNDERWATER_MULT = 0x46,
	TCVAR_LIGHT_RAY_DIST = 0x47,
	TCVAR_LIGHT_RAY_HEIGHTFALLOFF = 0x48,
	TCVAR_LIGHT_RAY_HEIGHTFALLOFFSTART = 0x49,
	TCVAR_LIGHT_RAY_ADD_REDUCER = 0x4A,
	TCVAR_LIGHT_RAY_BLIT_SIZE = 0x4B,
	TCVAR_LIGHT_RAY_RAY_LENGTH = 0x4C,
	TCVAR_POSTFX_EXPOSURE = 0x4D,
	TCVAR_POSTFX_EXPOSURE_MIN = 0x4E,
	TCVAR_POSTFX_EXPOSURE_MAX = 0x4F,
	TCVAR_POSTFX_BRIGHT_PASS_THRESH_WIDTH = 0x50,
	TCVAR_POSTFX_BRIGHT_PASS_THRESH = 0x51,
	TCVAR_POSTFX_INTENSITY_BLOOM = 0x52,
	TCVAR_POSTFX_CORRECT_COL_R = 0x53,
	TCVAR_POSTFX_CORRECT_COL_G = 0x54,
	TCVAR_POSTFX_CORRECT_COL_B = 0x55,
	TCVAR_POSTFX_CORRECT_CUTOFF = 0x56,
	TCVAR_POSTFX_SHIFT_COL_R = 0x57,
	TCVAR_POSTFX_SHIFT_COL_G = 0x58,
	TCVAR_POSTFX_SHIFT_COL_B = 0x59,
	TCVAR_POSTFX_SHIFT_CUTOFF = 0x5A,
	TCVAR_POSTFX_DESATURATION = 0x5B,
	TCVAR_POSTFX_NOISE = 0x5C,
	TCVAR_POSTFX_NOISE_SIZE = 0x5D,
	TCVAR_POSTFX_TONEMAP_FILMIC_OVERRIDE_DARK = 0x5E,
	TCVAR_POSTFX_TONEMAP_FILMIC_EXPOSURE_DARK = 0x5F,
	TCVAR_POSTFX_TONEMAP_FILMIC_A_DARK = 0x60,
	TCVAR_POSTFX_TONEMAP_FILMIC_B_DARK = 0x61,
	TCVAR_POSTFX_TONEMAP_FILMIC_C_DARK = 0x62,
	TCVAR_POSTFX_TONEMAP_FILMIC_D_DARK = 0x63,
	TCVAR_POSTFX_TONEMAP_FILMIC_E_DARK = 0x64,
	TCVAR_POSTFX_TONEMAP_FILMIC_F_DARK = 0x65,
	TCVAR_POSTFX_TONEMAP_FILMIC_W_DARK = 0x66,
	TCVAR_POSTFX_TONEMAP_FILMIC_OVERRIDE_BRIGHT = 0x67,
	TCVAR_POSTFX_TONEMAP_FILMIC_EXPOSURE_BRIGHT = 0x68,
	TCVAR_POSTFX_TONEMAP_FILMIC_A_BRIGHT = 0x69,
	TCVAR_POSTFX_TONEMAP_FILMIC_B_BRIGHT = 0x6A,
	TCVAR_POSTFX_TONEMAP_FILMIC_C_BRIGHT = 0x6B,
	TCVAR_POSTFX_TONEMAP_FILMIC_D_BRIGHT = 0x6C,
	TCVAR_POSTFX_TONEMAP_FILMIC_E_BRIGHT = 0x6D,
	TCVAR_POSTFX_TONEMAP_FILMIC_F_BRIGHT = 0x6E,
	TCVAR_POSTFX_TONEMAP_FILMIC_W_BRIGHT = 0x6F,
	TCVAR_POSTFX_VIGNETTING_INTENSITY = 0x70,
	TCVAR_POSTFX_VIGNETTING_RADIUS = 0x71,
	TCVAR_POSTFX_VIGNETTING_CONTRAST = 0x72,
	TCVAR_POSTFX_VIGNETTING_COL_R = 0x73,
	TCVAR_POSTFX_VIGNETTING_COL_G = 0x74,
	TCVAR_POSTFX_VIGNETTING_COL_B = 0x75,
	TCVAR_POSTFX_GRAD_TOP_COL_R = 0x76,
	TCVAR_POSTFX_GRAD_TOP_COL_G = 0x77,
	TCVAR_POSTFX_GRAD_TOP_COL_B = 0x78,
	TCVAR_POSTFX_GRAD_MIDDLE_COL_R = 0x79,
	TCVAR_POSTFX_GRAD_MIDDLE_COL_G = 0x7A,
	TCVAR_POSTFX_GRAD_MIDDLE_COL_B = 0x7B,
	TCVAR_POSTFX_GRAD_BOTTOM_COL_R = 0x7C,
	TCVAR_POSTFX_GRAD_BOTTOM_COL_G = 0x7D,
	TCVAR_POSTFX_GRAD_BOTTOM_COL_B = 0x7E,
	TCVAR_POSTFX_GRAD_MIDPOINT = 0x7F,
	TCVAR_POSTFX_GRAD_TOP_MIDDLE_MIDPOINT = 0x80,
	TCVAR_POSTFX_GRAD_MIDDLE_BOTTOM_MIDPOINT = 0x81,
	TCVAR_POSTFX_SCANLINEINTENSITY = 0x82,
	TCVAR_POSTFX_SCANLINE_FREQUENCY_0 = 0x83,
	TCVAR_POSTFX_SCANLINE_FREQUENCY_1 = 0x84,
	TCVAR_POSTFX_SCANLINE_SPEED = 0x85,
	TCVAR_POSTFX_MOTIONBLUR_LENGTH = 0x86,
	TCVAR_DOF_FAR = 0x87,
	TCVAR_DOF_BLUR_MID = 0x88,
	TCVAR_DOF_BLUR_FAR = 0x89,
	TCVAR_DOF_ENABLE_HQDOF = 0x8A,
	TCVAR_DOF_HQDOF_SMALLBLUR = 0x8B,
	TCVAR_DOF_HQDOF_SHALLOWDOF = 0x8C,
	TCVAR_DOF_HQDOF_NEARPLANE_OUT = 0x8D,
	TCVAR_DOF_HQDOF_NEARPLANE_IN = 0x8E,
	TCVAR_DOF_HQDOF_FARPLANE_OUT = 0x8F,
	TCVAR_DOF_HQDOF_FARPLANE_IN = 0x90,
	TCVAR_ENV_BLUR_IN = 0x91,
	TCVAR_ENV_BLUR_OUT = 0x92,
	TCVAR_ENV_BLUR_SIZE = 0x93,
	TCVAR_BOKEH_BRIGHTNESS_MIN = 0x94,
	TCVAR_BOKEH_BRIGHTNESS_MAX = 0x95,
	TCVAR_BOKEH_FADE_MIN = 0x96,
	TCVAR_BOKEH_FADE_MAX = 0x97,
	TCVAR_NV_LIGHT_DIR_MULT = 0x98,
	TCVAR_NV_LIGHT_AMB_DOWN_MULT = 0x99,
	TCVAR_NV_LIGHT_AMB_BASE_MULT = 0x9A,
	TCVAR_NV_LOWLUM = 0x9B,
	TCVAR_NV_HIGHLUM = 0x9C,
	TCVAR_NV_TOPLUM = 0x9D,
	TCVAR_NV_SCALERLUM = 0x9E,
	TCVAR_NV_OFFSETLUM = 0x9F,
	TCVAR_NV_OFFSETLOWLUM = 0xA0,
	TCVAR_NV_OFFSETHIGHLUM = 0xA1,
	TCVAR_NV_NOISELUM = 0xA2,
	TCVAR_NV_NOISELOWLUM = 0xA3,
	TCVAR_NV_NOISEHIGHLUM = 0xA4,
	TCVAR_NV_BLOOMLUM = 0xA5,
	TCVAR_NV_COLORLUM_R = 0xA6,
	TCVAR_NV_COLORLUM_G = 0xA7,
	TCVAR_NV_COLORLUM_B = 0xA8,
	TCVAR_NV_COLORLOWLUM_R = 0xA9,
	TCVAR_NV_COLORLOWLUM_G = 0xAA,
	TCVAR_NV_COLORLOWLUM_B = 0xAB,
	TCVAR_NV_COLORHIGHLUM_R = 0xAC,
	TCVAR_NV_COLORHIGHLUM_G = 0xAD,
	TCVAR_NV_COLORHIGHLUM_B = 0xAE,
	TCVAR_HH_RANGESTART = 0xAF,
	TCVAR_HH_RANGEEND = 0xB0,
	TCVAR_HH_MININTENSITY = 0xB1,
	TCVAR_HH_MAXINTENSITY = 0xB2,
	TCVAR_HH_DISPU = 0xB3,
	TCVAR_HH_DISPV = 0xB4,
	TCVAR_HH_TEX1_USCALE = 0xB5,
	TCVAR_HH_TEX1_VSCALE = 0xB6,
	TCVAR_HH_TEX1_UOFFSET = 0xB7,
	TCVAR_HH_TEX1_VOFFSET = 0xB8,
	TCVAR_HH_TEX2_USCALE = 0xB9,
	TCVAR_HH_TEX2_VSCALE = 0xBA,
	TCVAR_HH_TEX2_UOFFSET = 0xBB,
	TCVAR_HH_TEX2_VOFFSET = 0xBC,
	TCVAR_HH_TEX1_OFFSET = 0xBD,
	TCVAR_HH_TEX1_FREQUENCY = 0xBE,
	TCVAR_HH_TEX1_AMPLITUDE = 0xBF,
	TCVAR_HH_TEX1_SCROLLING = 0xC0,
	TCVAR_HH_TEX2_OFFSET = 0xC1,
	TCVAR_HH_TEX2_FREQUENCY = 0xC2,
	TCVAR_HH_TEX2_AMPLITUDE = 0xC3,
	TCVAR_HH_TEX2_SCROLLING = 0xC4,
	TCVAR_LENS_DISTORTION_COEFF = 0xC5,
	TCVAR_LENS_DISTORTION_CUBE_COEFF = 0xC6,
	TCVAR_LENS_CHROMATIC_ABERRATION_COEFF = 0xC7,
	TCVAR_LENS_CHROMATIC_ABERRATION_CUBE_COEFF = 0xC8,
	TCVAR_LENS_ARTEFACTS_INTENSITY = 0xC9,
	TCVAR_LENS_ARTEFACTS_INTENSITY_MIN_EXP = 0xCA,
	TCVAR_LENS_ARTEFACTS_INTENSITY_MAX_EXP = 0xCB,
	TCVAR_BLUR_VIGNETTING_RADIUS = 0xCC,
	TCVAR_BLUR_VIGNETTING_INTENSITY = 0xCD,
	TCVAR_SCREEN_BLUR_INTENSITY = 0xCE,
	TCVAR_SKY_ZENITH_TRANSITION_POSITION = 0xCF,
	TCVAR_SKY_ZENITH_TRANSITION_EAST_BLEND = 0xD0,
	TCVAR_SKY_ZENITH_TRANSITION_WEST_BLEND = 0xD1,
	TCVAR_SKY_ZENITH_BLEND_START = 0xD2,
	TCVAR_SKY_ZENITH_COL_R = 0xD3,
	TCVAR_SKY_ZENITH_COL_G = 0xD4,
	TCVAR_SKY_ZENITH_COL_B = 0xD5,
	TCVAR_SKY_ZENITH_COL_INTEN = 0xD6,
	TCVAR_SKY_ZENITH_TRANSITION_COL_R = 0xD7,
	TCVAR_SKY_ZENITH_TRANSITION_COL_G = 0xD8,
	TCVAR_SKY_ZENITH_TRANSITION_COL_B = 0xD9,
	TCVAR_SKY_ZENITH_TRANSITION_COL_INTEN = 0xDA,
	TCVAR_SKY_AZIMUTH_TRANSITION_POSITION = 0xDB,
	TCVAR_SKY_AZIMUTH_EAST_COL_R = 0xDC,
	TCVAR_SKY_AZIMUTH_EAST_COL_G = 0xDD,
	TCVAR_SKY_AZIMUTH_EAST_COL_B = 0xDE,
	TCVAR_SKY_AZIMUTH_EAST_COL_INTEN = 0xDF,
	TCVAR_SKY_AZIMUTH_TRANSITION_COL_R = 0xE0,
	TCVAR_SKY_AZIMUTH_TRANSITION_COL_G = 0xE1,
	TCVAR_SKY_AZIMUTH_TRANSITION_COL_B = 0xE2,
	TCVAR_SKY_AZIMUTH_TRANSITION_COL_INTEN = 0xE3,
	TCVAR_SKY_AZIMUTH_WEST_COL_R = 0xE4,
	TCVAR_SKY_AZIMUTH_WEST_COL_G = 0xE5,
	TCVAR_SKY_AZIMUTH_WEST_COL_B = 0xE6,
	TCVAR_SKY_AZIMUTH_WEST_COL_INTEN = 0xE7,
	TCVAR_SKY_HDR = 0xE8,
	TCVAR_SKY_PLANE_COL_R = 0xE9,
	TCVAR_SKY_PLANE_COL_G = 0xEA,
	TCVAR_SKY_PLANE_COL_B = 0xEB,
	TCVAR_SKY_PLANE_INTEN = 0xEC,
	TCVAR_SKY_SUN_COL_R = 0xED,
	TCVAR_SKY_SUN_COL_G = 0xEE,
	TCVAR_SKY_SUN_COL_B = 0xEF,
	TCVAR_SKY_SUN_DISC_COL_R = 0xF0,
	TCVAR_SKY_SUN_DISC_COL_G = 0xF1,
	TCVAR_SKY_SUN_DISC_COL_B = 0xF2,
	TCVAR_SKY_SUN_DISC_SIZE = 0xF3,
	TCVAR_SKY_SUN_HDR = 0xF4,
	TCVAR_SKY_SUN_MIEPHASE = 0xF5,
	TCVAR_SKY_SUN_MIESCATTER = 0xF6,
	TCVAR_SKY_SUN_MIE_INTENSITY_MULT = 0xF7,
	TCVAR_SKY_SUN_INFLUENCE_RADIUS = 0xF8,
	TCVAR_SKY_SUN_SCATTER_INTEN = 0xF9,
	TCVAR_SKY_MOON_COL_R = 0xFA,
	TCVAR_SKY_MOON_COL_G = 0xFB,
	TCVAR_SKY_MOON_COL_B = 0xFC,
	TCVAR_SKY_MOON_DISC_SIZE = 0xFD,
	TCVAR_SKY_MOON_ITEN = 0xFE,
	TCVAR_SKY_STARS_ITEN = 0xFF,
	TCVAR_SKY_MOON_INFLUENCE_RADIUS = 0x100,
	TCVAR_SKY_MOON_SCATTER_INTEN = 0x101,
	TCVAR_SKY_CLOUD_GEN_FREQUENCY = 0x102,
	TCVAR_SKY_CLOUD_GEN_SCALE = 0x103,
	TCVAR_SKY_CLOUD_GEN_THRESHOLD = 0x104,
	TCVAR_SKY_CLOUD_GEN_SOFTNESS = 0x105,
	TCVAR_SKY_CLOUD_DENSITY_MULT = 0x106,
	TCVAR_SKY_CLOUD_DENSITY_BIAS = 0x107,
	TCVAR_SKY_CLOUD_MID_COL_R = 0x108,
	TCVAR_SKY_CLOUD_MID_COL_G = 0x109,
	TCVAR_SKY_CLOUD_MID_COL_B = 0x10A,
	TCVAR_SKY_CLOUD_BASE_COL_R = 0x10B,
	TCVAR_SKY_CLOUD_BASE_COL_G = 0x10C,
	TCVAR_SKY_CLOUD_BASE_COL_B = 0x10D,
	TCVAR_SKY_CLOUD_BASE_STRENGTH = 0x10E,
	TCVAR_SKY_CLOUD_SHADOW_COL_R = 0x10F,
	TCVAR_SKY_CLOUD_SHADOW_COL_G = 0x110,
	TCVAR_SKY_CLOUD_SHADOW_COL_B = 0x111,
	TCVAR_SKY_CLOUD_SHADOW_STRENGTH = 0x112,
	TCVAR_SKY_CLOUD_GEN_DENSITY_OFFSET = 0x113,
	TCVAR_SKY_CLOUD_OFFSET = 0x114,
	TCVAR_SKY_CLOUD_OVERALL_STRENGTH = 0x115,
	TCVAR_SKY_CLOUD_OVERALL_COLOR = 0x116,
	TCVAR_SKY_CLOUD_EDGE_STRENGTH = 0x117,
	TCVAR_SKY_CLOUD_FADEOUT = 0x118,
	TCVAR_SKY_CLOUD_HDR = 0x119,
	TCVAR_SKY_CLOUD_DITHER_STRENGTH = 0x11A,
	TCVAR_SKY_SMALL_CLOUD_COL_R = 0x11B,
	TCVAR_SKY_SMALL_CLOUD_COL_G = 0x11C,
	TCVAR_SKY_SMALL_CLOUD_COL_B = 0x11D,
	TCVAR_SKY_SMALL_CLOUD_DETAIL_STRENGTH = 0x11E,
	TCVAR_SKY_SMALL_CLOUD_DETAIL_SCALE = 0x11F,
	TCVAR_SKY_SMALL_CLOUD_DENSITY_MULT = 0x120,
	TCVAR_SKY_SMALL_CLOUD_DENSITY_BIAS = 0x121,
	TCVAR_CLOUD_SHADOW_DENSITY = 0x122,
	TCVAR_CLOUD_SHADOW_SOFTNESS = 0x123,
	TCVAR_CLOUD_SHADOW_OPACITY = 0x124,
	TCVAR_DIRECTIONAL_SHADOW_NUM_CASCADES = 0x125,
	TCVAR_DIRECTIONAL_SHADOW_DISTANCE_MULTIPLIER = 0x126,
	TCVAR_DIRECTIONAL_SHADOW_SOFTNESS = 0x127,
	TCVAR_DIRECTIONAL_SHADOW_CASCADE0_SCALE = 0x128,
	TCVAR_SPRITE_BRIGHTNESS = 0x129,
	TCVAR_SPRITE_SIZE = 0x12A,
	TCVAR_SPRITE_CORONA_SCREENSPACE_EXPANSION = 0x12B,
	TCVAR_LENSFLARE_VISIBILITY = 0x12C,
	TCVAR_SPRITE_DISTANT_LIGHT_TWINKLE = 0x12D,
	TCVAR_WATER_REFLECTION_FIRST = 0x12E,
	TCVAR_WATER_REFLECTION = 0x12E,
	TCVAR_WATER_REFLECTION_FAR_CLIP = 0x12F,
	TCVAR_WATER_REFLECTION_LOD = 0x130,
	TCVAR_WATER_REFLECTION_SKY_FLOD_RANGE = 0x131,
	TCVAR_WATER_REFLECTION_LOD_RANGE_ENABLED = 0x132,
	TCVAR_WATER_REFLECTION_LOD_RANGE_HD_START = 0x133,
	TCVAR_WATER_REFLECTION_LOD_RANGE_HD_END = 0x134,
	TCVAR_WATER_REFLECTION_LOD_RANGE_ORPHANHD_START = 0x135,
	TCVAR_WATER_REFLECTION_LOD_RANGE_ORPHANHD_END = 0x136,
	TCVAR_WATER_REFLECTION_LOD_RANGE_LOD_START = 0x137,
	TCVAR_WATER_REFLECTION_LOD_RANGE_LOD_END = 0x138,
	TCVAR_WATER_REFLECTION_LOD_RANGE_SLOD1_START = 0x139,
	TCVAR_WATER_REFLECTION_LOD_RANGE_SLOD1_END = 0x13A,
	TCVAR_WATER_REFLECTION_LOD_RANGE_SLOD2_START = 0x13B,
	TCVAR_WATER_REFLECTION_LOD_RANGE_SLOD2_END = 0x13C,
	TCVAR_WATER_REFLECTION_LOD_RANGE_SLOD3_START = 0x13D,
	TCVAR_WATER_REFLECTION_LOD_RANGE_SLOD3_END = 0x13E,
	TCVAR_WATER_REFLECTION_LOD_RANGE_SLOD4_START = 0x13F,
	TCVAR_WATER_REFLECTION_LOD_RANGE_SLOD4_END = 0x140,
	TCVAR_WATER_REFLECTION_HEIGHT_OFFSET = 0x141,
	TCVAR_WATER_REFLECTION_HEIGHT_OVERRIDE = 0x142,
	TCVAR_WATER_REFLECTION_HEIGHT_OVERRIDE_AMOUNT = 0x143,
	TCVAR_WATER_REFLECTION_DISTANT_LIGHT_INTENSITY = 0x144,
	TCVAR_WATER_REFLECTION_CORONA_INTENSITY = 0x145,
	TCVAR_WATER_REFLECTION_LAST = 0x145,
	TCVAR_WATER_FOGLIGHT = 0x146,
	TCVAR_WATER_INTERIOR = 0x147,
	TCVAR_WATER_ENABLEFOGSTREAMING = 0x148,
	TCVAR_WATER_FOAM_INTENSITY_MULT = 0x149,
	TCVAR_WATER_DRYING_SPEED_MULT = 0x14A,
	TCVAR_WATER_SPECULAR_INTENSITY = 0x14B,
	TCVAR_MIRROR_REFLECTION_LOCAL_LIGHT_INTENSITY = 0x14C,
	TCVAR_FOG_START = 0x14D,
	TCVAR_FOG_NEAR_COL_R = 0x14E,
	TCVAR_FOG_NEAR_COL_G = 0x14F,
	TCVAR_FOG_NEAR_COL_B = 0x150,
	TCVAR_FOG_NEAR_COL_A = 0x151,
	TCVAR_FOG_SUN_COL_R = 0x152,
	TCVAR_FOG_SUN_COL_G = 0x153,
	TCVAR_FOG_SUN_COL_B = 0x154,
	TCVAR_FOG_SUN_COL_A = 0x155,
	TCVAR_FOG_SUN_LIGHTING_CALC_POW = 0x156,
	TCVAR_FOG_MOON_COL_R = 0x157,
	TCVAR_FOG_MOON_COL_G = 0x158,
	TCVAR_FOG_MOON_COL_B = 0x159,
	TCVAR_FOG_MOON_COL_A = 0x15A,
	TCVAR_FOG_MOON_LIGHTING_CALC_POW = 0x15B,
	TCVAR_FOG_FAR_COL_R = 0x15C,
	TCVAR_FOG_FAR_COL_G = 0x15D,
	TCVAR_FOG_FAR_COL_B = 0x15E,
	TCVAR_FOG_FAR_COL_A = 0x15F,
	TCVAR_FOG_DENSITY = 0x160,
	TCVAR_FOG_HEIGHT_FALLOFF = 0x161,
	TCVAR_FOG_BASE_HEIGHT = 0x162,
	TCVAR_FOG_ALPHA = 0x163,
	TCVAR_FOG_HORIZON_TINT_SCALE = 0x164,
	TCVAR_FOG_HDR = 0x165,
	TCVAR_FOG_HAZE_COL_R = 0x166,
	TCVAR_FOG_HAZE_COL_G = 0x167,
	TCVAR_FOG_HAZE_COL_B = 0x168,
	TCVAR_FOG_HAZE_DENSITY = 0x169,
	TCVAR_FOG_HAZE_ALPHA = 0x16A,
	TCVAR_FOG_HAZE_HDR = 0x16B,
	TCVAR_FOG_HAZE_START = 0x16C,
	TCVAR_FOG_SHAPE_LOW = 0x16D,
	TCVAR_FOG_SHAPE_HIGH = 0x16E,
	TCVAR_FOG_LOG_10_MIN_VISIBILIY = 0x16F,
	TCVAR_FOG_SHAPE_WEIGHT_0 = 0x170,
	TCVAR_FOG_SHAPE_WEIGHT_1 = 0x171,
	TCVAR_FOG_SHAPE_WEIGHT_2 = 0x172,
	TCVAR_FOG_SHAPE_WEIGHT_3 = 0x173,
	TCVAR_FOG_SHADOW_AMOUNT = 0x174,
	TCVAR_FOG_SHADOW_FALLOFF = 0x175,
	TCVAR_FOG_SHADOW_BASE_HEIGHT = 0x176,
	TCVAR_FOG_VOLUME_RANGE = 0x177,
	TCVAR_FOG_VOLUME_FADE_RANGE = 0x178,
	TCVAR_FOG_VOLUME_INTENSITY_SCALE = 0x179,
	TCVAR_FOG_VOLUME_SIZE_SCALE = 0x17A,
	TCVAR_FOG_FOGRAY_CONTRAST = 0x17B,
	TCVAR_FOG_FOGRAY_INTENSITY = 0x17C,
	TCVAR_FOG_FOGRAY_DENSITY = 0x17D,
	TCVAR_FOG_FOGRAY_NEARFADE = 0x17E,
	TCVAR_FOG_FOGRAY_FARFADE = 0x17F,
	TCVAR_REFLECTION_LOD_RANGE_START = 0x180,
	TCVAR_REFLECTION_LOD_RANGE_END = 0x181,
	TCVAR_REFLECTION_SLOD_RANGE_START = 0x182,
	TCVAR_REFLECTION_SLOD_RANGE_END = 0x183,
	TCVAR_REFLECTION_INTERIOR_RANGE = 0x184,
	TCVAR_REFLECTION_TWEAK_INTERIOR_AMB = 0x185,
	TCVAR_REFLECTION_TWEAK_EXTERIOR_AMB = 0x186,
	TCVAR_REFLECTION_TWEAK_EMISSIVE = 0x187,
	TCVAR_REFLECTION_TWEAK_DIRECTIONAL = 0x188,
	TCVAR_REFLECTION_HDR_MULT = 0x189,
	TCVAR_FAR_CLIP = 0x18A,
	TCVAR_TEMPERAiTURE = 0x18B,
	TCVAR_PTFX_EMISSIVE_INTENSITY_MULT = 0x18C,
	TCVAR_VFXLIGHTNING_INTENSITY_MULT = 0x18D,
	TCVAR_VFXLIGHTNING_VISIBILITY = 0x18E,
	TCVAR_GPUPTFX_LIGHT_INTENSITY_MULT = 0x18F,
	TCVAR_NATURAL_AMBIENT_MULTIPLIER = 0x190,
	TCVAR_ARTIFICIAL_INT_AMBIENT_MULTIPLIER = 0x191,
	TCVAR_FOG_CUT_OFF = 0x192,
	TCVAR_NO_WEATHER_FX = 0x193,
	TCVAR_NO_GPU_FX = 0x194,
	TCVAR_NO_RAIN = 0x195,
	TCVAR_NO_RAIN_RIPPLES = 0x196,
	TCVAR_FOGVOLUME_DENSITY_SCALAR = 0x197,
	TCVAR_FOGVOLUME_DENSITY_SCALAR_INTERIOR = 0x198,
	TCVAR_FOGVOLUME_FOG_SCALER = 0x199,
	TCVAR_TIME_OFFSET = 0x19A,
	TCVAR_VEHICLE_DIRT_MOD = 0x19B,
	TCVAR_WIND_SPEED_MULT = 0x19C,
	TCVAR_ENTITY_REJECT = 0x19D,
	TCVAR_LOD_MULT = 0x19E,
	TCVAR_ENABLE_OCCLUSION = 0x19F,
	TCVAR_ENABLE_SHADOW_OCCLUSION = 0x1A0,
	TCVAR_ENABLE_EXTERIOR = 0x1A1,
	TCVAR_PORTAL_WEIGHT = 0x1A2,
	TCVAR_LIGHT_FALLOFF_MULT = 0x1A3,
	TCVAR_LODLIGHT_RANGE_MULT = 0x1A4,
	TCVAR_SHADOW_DISTANCE_MULT = 0x1A5,
	TCVAR_LOD_MULT_HD = 0x1A6,
	TCVAR_LOD_MULT_ORPHANHD = 0x1A7,
	TCVAR_LOD_MULT_LOD = 0x1A8,
	TCVAR_LOD_MULT_SLOD1 = 0x1A9,
	TCVAR_LOD_MULT_SLOD2 = 0x1AA,
	TCVAR_LOD_MULT_SLOD3 = 0x1AB,
	TCVAR_LOD_MULT_SLOD4 = 0x1AC,
	TCVAR_NUM = 0x1AD,
};

struct TCKeyFrameUncompressed {
    float m_Vars[(429 + 15) & ~15];

    __forceinline float* GetVarPtr(int index) {
        return &m_Vars[index];
    }

    __forceinline float GetVar(int index) const {
        return m_Vars[index];
    }

    __forceinline void SetVar(int index, float value) {
		m_Vars[index] = value;
    }
};

struct TCKeyFrame {
    uint16_t m_Vars[(429 + 15) & ~15];

    __forceinline float GetVar(int index) const {
        uint32_t e, z, s;

        e = (uint32_t)m_Vars[index] << 16;
        s = 0x80000000u & e;
        e = 0x7fff0000u & e;
        z = (uint32_t)(((int)(e) | -(int)(e)) >> 31);
        z = 0x38000000u & z;
        e = ((e >> 3) + z) | s;

        return *(float*)&e;
    }

    __forceinline void SetVar(int index, float value) {
        uint32_t i, e, l, o;
        i = *(uint32_t*)&value;
        e = 0x7fffffffu & i;
        e = 0x38000000u - e;
        l = (int)(0x00000000u + e) >> 31;
        o = (int)(0x0fffffffu + e) >> 31;
        e = (uint32_t)(-(int)e) >> 13;
        e = e | o;
        e = e & ((0x00007fffu & l));
        e = e | ((0x80000000u & i) >> 16);

		m_Vars[index] = (uint16_t)e;
    }
};

struct TCFrameInfo {
    TCKeyFrameUncompressed m_KeyFrame;
};

struct TCCycle {
    TCKeyFrame m_KeyFrame[2][13];
    uint32_t m_Name;
    char _0x57C4[0x1C];
};

struct TimeCycle {
    char _0x0000[0x68];
	TCCycle* m_Cycles;
    char _0x0070[0x8E0 - 0x70];
	TCKeyFrameUncompressed m_ReplayFrame;
	TCKeyFrameUncompressed m_DefaultFrame;
    char _0x1660[0x40];
	TCKeyFrameUncompressed m_JumpFrame;
    TCFrameInfo m_FrameInfo;

    __forceinline void SetVar(int index, float value) {
        for (int i = 0; i < 16; i++) {
			TCCycle& Cycle = m_Cycles[i];
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 13; k++) {
					Cycle.m_KeyFrame[j][k].SetVar(index, value);
                }
            }
        }

		m_ReplayFrame.SetVar(index, value);
		m_DefaultFrame.SetVar(index, value);
		m_JumpFrame.SetVar(index, value);
		m_FrameInfo.m_KeyFrame.SetVar(index, value);
    }
    __forceinline auto GetVar(int index) {
        for (int i = 0; i < 16; i++) {
            TCCycle& Cycle = m_Cycles[i];
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 13; k++) {
                    return Cycle.m_KeyFrame[j][k].GetVar(index);
                }
            }
        }
    }
};

template<typename T, typename T1>
T* Cast(T1 Type) {
	return (T*)(Type);
}


class CPlayerListMenu {
public:
	virtual ~CPlayerListMenu() {}
	virtual void unk_0008() {}

	char pad_0008[112]; //0x0008
	bool m_failed; //0x0076
	char pad_0079[21]; //0x0077
	uint32_t m_selected_friend; //0x008C
	char pad_0090[40]; //0x0090
	uint32_t m_online_bitset; //0x00B8
	char pad_00BC[28]; //0x00BC
	uint8_t m_is_joinable; //0x00D8
	char pad_00D9[3]; //0x00D9
	uint32_t unk_00DC; //0x00DC
	class unk* unk_00E0; //0x00E0
	char pad_00E8[80]; //0x00E8
	uint32_t unk_0138; //0x0138
};

struct DataNodeCommonOperations {
	virtual ~DataNodeCommonOperations();

	uint64_t m_Data;
};

struct NetObjectBase {
	uint64_t m_vtable;
	eNetObjectTypes m_type;
	int16_t m_network_id;
	char _0x000C[0x14];
	int8_t m_player_sync_intervals[0x20];
	char _0x0040[0x9];
	uint8_t m_owner; // 0x49
	char _0x004A[0x6];
	uint64_t m_entity;
	char _0x0058[0x4C0];
	int16_t m_spectating_network_id;
};

struct DataNode {
	virtual ~DataNode() = 0;
	virtual bool IsDataNode() = 0;
	virtual bool IsParentNode() = 0;

	DataNode* m_Next; //0x0008
	DataNode* m_Previous; //0x0010
	uint64_t m_SyncTree; //0x0018
	DataNode* m_Parent; //0x0020
	uint32_t m_Flag; //0x0028
	uint32_t m_Flag2; //0x002C
	uint32_t m_Flag3; //0x0030
	char _0x0034[0x4]; //0x0034
	DataNode* m_FirstChild; //0x0038
	DataNode* m_LastChild; //0x0040
	uint64_t m_UnkData; //0x0048
	uint32_t m_ChildCount; //0x0050
	char _0x0054[0x54];
	DataNodeCommonOperations* m_DataNodeCommonOperations; // 0xA8

	bool IsWriting() {
		return *(bool*)((uint64_t)this + 0x40);
	}
};

class ChatData
{
public:
	uint32_t m_timer_one; //0x0000
	uint32_t m_timer_two; //0x0004
	uint32_t m_char_count; //0x0008
	uint32_t m_key_held_time; //0x000C
	uint32_t m_team_label; //0x0010
	uint8_t m_chat_open; //0x0014
	uint8_t m_is_job; //0x0015
	uint8_t m_disabled; //0x0016
	uint8_t m_not_typing; //0x0017
	uint32_t m_focus_mode; //0x0018
	uint32_t m_chat_mode; //0x001C
	uint32_t m_scaleform; //0x0020
	char pad_0024[8]; //0x0024
	char16_t m_current_text[142]; //0x002C
	uint32_t m_hud_color; //0x0148
	uint8_t m_hud_color_override; // 0x014C
	char pad_014D[43]; // 0x014D
}; //Size: 0x0178
static_assert(sizeof(ChatData) == 0x178);

inline std::pair<float, std::string> g_SelectedMetricSystem = { 2.236936f, "MPH" };
inline bool g_EnablePlayerInfo;

struct QuadInfo
{
	std::uint64_t m_quadPool;
	short m_quadCount;
};

struct WaterQuads
{
	QuadInfo* m_oceanQuads;
};




inline bool g_ScriptVmRe = false;

#pragma pack(push, 1)
class ScInfo {
public:
	char m_ticket[208]; //0x0000
	char pad_00D0[304]; //0x00D0
	char m_session_ticket[88]; //0x0200
	char pad_0258[40]; //0x0258
	uint32_t m_nonce; //0x0280
	char pad_0284[4]; //0x0284
	uint32_t m_account_id; //0x0288
	char pad_028C[16]; //0x028C
	char m_profile_pic[128]; //0x029C
	char pad_031C[32]; //0x031C
	char m_country_code[4]; //0x033C
	char pad_0340[31]; //0x0340
	char m_email_address[96]; //0x035F
	char pad_03BF[6]; //0x03BF
	char m_language_subtag[8]; //0x03C5
	char pad_03CD[2]; //0x03CD
	char m_sc_name[20]; //0x03CF
	char pad_03E3[533]; //0x03E3
	char m_session_key[16]; //0x05F8
	char pad_0608[2296]; //0x0608
}; //Size: 0x0F00
static_assert(sizeof(ScInfo) == 0xF00);
#pragma pack(pop)
class ScGame {
public:
	char pad_0000[64]; //0x0000
	char* m_string; //0x0040
	char pad_0048[256]; //0x0048
};
static_assert(sizeof(ScGame) == 0x148);
class ScGameInfo {
public:
	virtual ~ScGameInfo() = default;
	virtual ScGame* GetGame() = 0;
	virtual void Unk0010() = 0;
	virtual void Unk0018() = 0;
	virtual int GetStringIndex(const char* StringId, u64 Unk, u32 GameId) = 0;
	char pad_0008[12]; //0x0008
	uint32_t m_id; //0x0014
	char pad_0018[8]; //0x0018
	ScGame m_games[10]; //0x0020
	u64 GetGamesAddress() {
		return (u64)this + offsetof(ScGameInfo, m_games);
	}
}; //Size: 0x0028
static_assert(sizeof(ScGameInfo) == 0xCF0);

inline std::unordered_map<uint64_t, bool> g_OnlineRockstarIds;
inline std::vector<uint64_t> g_OnlineRockstarIdQueue;

struct NetMsgIdentifier {
	uint64_t m_RockstarID;
	uint8_t m_Type;
	char _0x0009[0xB];
};


struct Friends {
	char _0x0000[128]; //0x0000
	char m_Name[32]; //0x0080
	char _0x00A0[24]; //0x00A0
	uint64_t m_RockstarID; //0x00B8
	char _0x00C0[4]; //0x00C0
	int m_State; //0x00C4
	char _0x00C8[312]; //0x00C8
};

class CFriend {
public:
	uint64_t m_rockstar_id; //0x0000
};

#pragma pack(push, 16)
class CFriendMenu {
public:
	virtual ~CFriendMenu() = default;
	CFriend get_friend(u8 index) {
		return *(CFriend*)((u64)this + 16ui64 * index);
	}
	u8 get_online_friend() {
		u8* data{ reinterpret_cast<u8*>((u64)this + 0x8) };
		for (u8 i{}; i != 20; ++i, data += 16) {
			if (data[i] == 3) {
				return i;
			}
		}
		return NULL;
	}
};
#pragma pack(pop)

namespace Rage {
	struct GenericPool {
		uint64_t m_Address;
		uint64_t m_MaskArray;
		uint32_t m_Count;
		uint32_t m_Size;

		inline uint64_t GetEntity(uint32_t Index) {
			if (IsValidPtr(m_MaskArray) && ~(*(uint8_t*)(m_MaskArray + Index) >> 7) & 1) {
				return (m_Address + (Index * m_Size));
			}

			return 0;
		}
	};

	struct VehicleEntityPool {
		uint64_t* m_Address;
		uint32_t m_Size;
		char _0x000C[0x4];
		uint32_t m_Count;

		inline uint64_t GetEntity(uint32_t Index) {
			if (!m_Address) return 0;
			return m_Address[Index];
		}
	};

	struct EntityPool {
		char _0x0000[0x10];
		uint32_t m_Num1;
		char _0x000C[0xC];
		uint32_t m_Num2;

		inline bool IsFull() {
			return m_Num1 - (m_Num2 & 0x3FFFFFFF) <= 256;
		}
	};
}