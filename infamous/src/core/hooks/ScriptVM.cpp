#include "stdafx.hpp"
#include "../hooks.hpp"
#include "util/fiber.hpp"
#include "util/fiber_pool.hpp"
#include "cheat/menu/main.hpp"
#include "rage/invoker/natives.hpp"
#include "core/core.hpp"
#include "rage/classes/script/scrProgram.hpp"
#include "rage/classes/rage/scrValue.hpp"
#include "cheat/util/control_manager.hpp"
#include "cheat/util/notify.hpp"
#include "cheat/util/script/script_patcher_service.hpp"
#include "cheat/util/texture.hpp"
#include "cheat/util/script/script_patcher_service.hpp"
#include "cheat/util/player_manager.hpp"

const u32 scrPageShift = 14;
const u32 scrPageSize = (1 << scrPageShift);
const u32 scrPageMask = scrPageSize - 1;
#define CASE(x) case x:
#define DEFAULT default:
#define FETCH_INSN
#define NEXT_INSN goto DISPATCH

#define VALIDATE_PC(target) do { if (target < 4) FAULT("Impossible jump target caught"); } while (0)
#define VALIDATE_PTR(value)	do { if (value.Uns < 16384) FAULT("Invalid reference caught"); } while (0)

#define SET_PC(_o) do { u32 o = _o; pc = (opcodesTbl[o>>scrPageShift] + (o & scrPageMask) - 1); opcodes = pc - o; } while (0)
#define ADD_PC(_r) SET_PC((pc - opcodes) + (_r))
#define CHECK_PC SET_PC(pc - opcodes)
#define DEC_CALLDEPTH --(ser->m_call_depth)
#define SET_STATE_AND_RET(state) ser->m_state = state; return ser->m_state;

#define FAULT(s, ...) do { LOG(Fatal, s, __VA_ARGS__); NEXT_INSN; } while(0)

#define LoadImm32	((pc+=4), *(u32*)(pc-3))
#if defined(__SNC__) || defined(_XBOX_VER)
#define LoadImm24	((pc+=3), *(u32*)(pc-3) & 0xFFFFFF)
#else
#define LoadImm24	((pc+=3), *(u32*)(pc-3) >> 8)
#endif
#define LoadImm16	((pc+=2), *(u16*)(pc-1))
#define LoadImmS16	((pc+=2), *(s16*)(pc-1))
#define LoadImm8	(*++pc)

static void scr_assign_string(char* dst, unsigned siz, const char* src)
{
	if (src)
	{
		while (*src && --siz)
			*dst++ = *src++;
	}
	*dst = '\0';
}

static void scr_append_string(char* dst, unsigned siz, const char* src)
{
	while (*dst)
		dst++, --siz;
	scr_assign_string(dst, siz, src);
}

static void scr_itoa(char* dest, int value)
{
	char stack[16], * sp = stack;
	if (value < 0)
	{
		*dest++ = '-';
		value = -value;
	}
	else if (!value)
	{
		dest[0] = '0';
		dest[1] = 0;
		return;
	}
	while (value)
	{
		*sp++ = (char)((value % 10) + '0');
		value /= 10;
	}
	while (sp != stack)
	{
		*dest++ = *--sp;
	}
	*dest = 0;
}

inline float scr_fmodf(float x, float y) { return y ? x - ((int)(x / y) * y) : 0; }
enum { c_NativeInsnLength = 4 };
static const int MAX_CALLSTACK = 16;


namespace Hooks {


	void StatGetIntHook(Rage::Invoker::NativeContext* Context) {
		//if (Context->m_script_hash == joaat("main_persistent")) {
	
		//}

		static int FrameCache = 0;
		static uint32_t this_frame = 0;
		static uint32_t last_frame = 0;
		static bool InitFlag = false;

		if (FrameCache < Native::GetFrameCount()) {
			FrameCache = Native::GetFrameCount();

			this_frame = timeGetTime();
			Framework::GetFrameWork()->m_delta = (float)(this_frame - last_frame) / 1000;
			last_frame = this_frame;

			if (!InitFlag) {
				InitFlag = true;
				Utils::GetFiberManager()->Initialize();
				Utils::GetFiberPool()->Initialize();
				Menu::textures::initialize();
				//Cheat::Menu::Textures::initialize();

				Utils::GetFiberManager()->Add("MMU", [] {
					//	RegisterScriptPatches();
					Menu::GetPlayerManager()->Update();
					GetMainMenu()->Update();
					});

				Utils::GetFiberManager()->Add("CMU", [] {
					Menu::GetControlManager()->Update();
					});

				Utils::GetFiberManager()->Add("F_BASE", [] {
					Menu::Notify::get_notify()->update();
					});
			}
			if (Core::g_Running) {
				Utils::GetFiberManager()->Update();
				//	Utils::GetScriptHookFiberManager()->Update();
			}
		}

		Native::StatGetInt(Context->GetArgument<uint32_t>(0), Context->GetArgument<int*>(1), Context->GetArgument<int>(2));
	}

	uint64_t ScriptVmHook(Rage::scrValue* Stack, uint64_t** Globals, Rage::scrProgram* Program, Rage::scrThreadContext* Context) {

		/*//const u8** opcodesTbl = Program->m_code_blocks;
		Rage::scrValue* sp = Stack + Context->m_stack_pointer - 1;
		Rage::scrValue* fp = Stack + Context->m_frame_pointer;


*/

	//	std::uint8_t** OriginalByteCode = Program->m_code_blocks;

	//	if (auto ByteCode = Menu::g_ScriptPatcher->GetScriptBytecode(Program->m_name_hash))
		//	Program->m_code_blocks = ByteCode;

		const auto _Return = OgScriptVmHook(Stack, Globals, Program, Context);


	//	Program->m_code_blocks = OriginalByteCode;
		return _Return;
	}
}