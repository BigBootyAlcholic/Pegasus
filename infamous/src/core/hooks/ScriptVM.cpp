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
#include "cheat/util/shop_manager.hpp"
#include "rage/classes/enums/enums.hpp"
#include "util/hooking/hooking.hpp"
namespace Hooks {

	class ScriptVMGuard {
	public:
		ScriptVMGuard(Rage::scrProgram* program, Rage::scrThreadContext* thread, u8** opcodeTable) :
			m_program(program), m_thread(thread), m_unmodifiedOpcodes(opcodeTable) {}
		~ScriptVMGuard() {}

		void setCallbackOnThread(std::function<void()> callback) {
			std::unique_lock lock(m_mutex);
			m_callbacks.push(callback);
		}
		void runCallbacks() {
			std::unique_lock lock(m_mutex);
			if (!m_callbacks.empty()) {
				auto fn{ std::move(m_callbacks.top()) };
				m_callbacks.pop();
				lock.unlock();
				Menu::NetShoppingManager::execute_as_script(m_thread->m_script_hash, fn);
			}
		}
	private:
		std::recursive_mutex m_mutex{};
		Rage::scrThreadContext* m_thread{};
		Rage::scrProgram* m_program{};
		u8** m_unmodifiedOpcodes{};
		std::stack<std::function<void()>> m_callbacks{};
	};

	class GlobalScriptVMGuard {
	public:
		GlobalScriptVMGuard() {}

		ScriptVMGuard* CreateGuardForThread(Rage::scrProgram* program, Rage::scrThreadContext* thread, u8** opcodeTable) {
			ScriptVMGuard* guard{ new ScriptVMGuard(program, thread, opcodeTable) };
			m_guards.insert({ program->m_name_hash, std::move(guard) });
			return m_guards[program->m_name_hash];
		}
		ScriptVMGuard* GetGuard(u32 hash) {
			if (auto it{ m_guards.find(hash) }; it != m_guards.end()) {
				return it->second;
			}
			return nullptr;
		}
		void Cleanup() {
			for (auto& entry : m_guards) {
				delete entry.second;
			}
		}
	private:
		std::map<u32, ScriptVMGuard*> m_guards{};
	};
	inline GlobalScriptVMGuard g_GlobalScriptVMGuard{};

	class ThreadStorageManager {
	public:
		void Tick() { //Handle cacher
			m_threadLocalStorage = Rage::tlsContext::get();
			m_thread = m_threadLocalStorage->m_script_thread;
			m_allocator = m_threadLocalStorage->m_allocator;
			if (!m_allocator) {
				m_allocator = m_threadLocalStorage->m_tls_entry;
			}
			if (!m_allocator) {
				m_allocator = m_threadLocalStorage->m_unk_allocator;
			}
		}
		void Access(std::function<void()> callback) {
			Rage::sysMemAllocator* allocatorStorage{ m_threadLocalStorage->m_allocator };
			Rage::scrThread* threadStorage{ m_threadLocalStorage->m_script_thread };
			m_threadLocalStorage->m_allocator = m_allocator;
			m_threadLocalStorage->m_script_thread = m_thread;
			m_threadLocalStorage->m_is_script_thread_active = true;
			std::invoke(callback);
			m_threadLocalStorage->m_allocator = allocatorStorage;
			m_threadLocalStorage->m_script_thread = threadStorage;
			m_threadLocalStorage->m_is_script_thread_active = threadStorage->m_context.m_state == Rage::eThreadState::Running;
		}
	private:
		Rage::tlsContext* m_threadLocalStorage{};
		Rage::scrThread* m_thread{};
		Rage::sysMemAllocator* m_allocator{};
	};
	inline ThreadStorageManager g_ThreadStorageManager{};

	inline std::optional<u8> charToHex(cc c) {
		if (c >= 'a' && c <= 'f')
			return static_cast<u8>(static_cast<int32_t>(c) - 87);
		if (c >= 'A' && c <= 'F')
			return static_cast<u8>(static_cast<int32_t>(c) - 55);
		if (c >= '0' && c <= '9')
			return static_cast<u8>(static_cast<int32_t>(c) - 48);
		return {};
	}
	inline std::vector<std::optional<u8>> CreateBytesFromString(std::string ptr) {
		std::vector<std::optional<u8>> bytes{};
		for (size_t i{}; i != ptr.size() - 1; ++i) {
			if (ptr[i] == ' ')
				continue;
			if (ptr[i] != '?') {
				if (auto c = charToHex(ptr[i]); c) {
					if (auto c2 = charToHex(ptr[i + 1]); c2)
						bytes.emplace_back(static_cast<uint8_t>((*c * 0x10) + *c2));
				}
			}
			else {
				bytes.push_back({});
			}
		}
		return bytes;
	}
	inline bool DoesMemoryMatch(u8* target, std::optional<u8> const* sig, u64 len) {
		for (u64 i{}; i != len; ++i) {
			if (sig[i] && *sig[i] != target[i]) {
				return false;
			}
		}
		return true;
	}

#define SIZEOF(a) sizeof(a) / sizeof(std::remove_pointer_t<decltype(a)>)
#define COUNT(a) ((sizeof(a)/sizeof(0[a])) / ((size_t)(!(sizeof(a) % sizeof(0[a])))))
#define ONCE_PER_FRAME(a) do a while (false)
#define ONCE(v, a) static bool v{ ([&] a (), true) };
#define LAZY_FIX(a) try a catch (...) {}
#define CASE(c) case c:
#define DEFAULT default:
#define FETCH_INSN {
#define NEXT_INSN break; }
#define SET_PC(_o) ONCE_PER_FRAME({ int64_t o{ _o }; pc = (opcodesTbl[o >> 14] + (o & 0x3FFF) - 1); opcodes = pc - o; })
#define ADD_PC(_r) SET_PC((pc - opcodes) + (_r))
#define CHECK_PC SET_PC(pc - opcodes)
#define FAULT(s, ...) ONCE_PER_FRAME({ LOG_ERROR(std::format(s {}, ...).c_str()) continue; })
#define LoadImm8 (*++pc)
#define LoadImm16 ((pc+=2), *(u16*)(pc-1))
#define LoadImmS16 ((pc+=2), *(i16*)(pc-1))
#define LoadImm24 ((pc+=3), *(u32*)(pc-3) >> 8)
#define LoadImm32 ((pc+=4), *(u32*)(pc-3))
#define HAS_ADDED_FUNCTIONALITY
//#ifdef HAS_ADDED_FUNCTIONALITY
	void PatchScriptItem(Rage::scrProgram* pt, Rage::scrThreadContext* ser, const u32 scriptHash, const std::string& ptr, const u32 offset, const std::vector<std::optional<u8>>& patch) {
		if (ser->m_script_hash == scriptHash) {
			std::vector<std::optional<u8>> bytes{ CreateBytesFromString(ptr) };
			for (u32 i{}; i != pt->m_code_size - bytes.size(); i++) {
				if (u8 * codeAddr{ pt->get_code_address(i) }) {
					if (DoesMemoryMatch(codeAddr, bytes.data(), bytes.size())) {
						u32 ip{ i + offset };
						memcpy(pt->get_code_address(ip), patch.data(), patch.size());
					

						LOG_WARN(std::format("{} {} {}", scriptHash, offset, ptr).c_str());

					}
					else {
						LOG_ERROR("ERR");
					}
				}
			}
		}
	}
/*#define PATCH_OPCODES(s, o, p, ...) \
	ONCE(s##PatchInitBool, { \
		patchOpcodes(Joaat(""), Context, #s Joaat(""), p, o, __VA_ARGS__); \
	})*/
//#endif

	inline void scr_assign_string(char* dst, u32 size, cc* src) {
		if (src) {
			while (*src && --size)
				*dst++ = *src++;
		}
		*dst = '\0';
	}
	inline void scr_append_string(char* dst, u32 size, cc* src) {
		while (*dst)
			dst++, --size;
		scr_assign_string(dst, size, src);
	}
	inline void scr_itoa(char* dest, int32_t value) {
		char stack[16]{}, * sp{ stack };
		if (value < 0) {
			*dest++ = '-';
			value = -value;
		}
		else if (!value) {
			dest[0] = '0';
			dest[1] = 0;
			return;
		}
		while (value) {
			*sp++ = (char)((value % 10) + '0');
			value /= 10;
		}
		while (sp != stack)
			*dest++ = *--sp;
		*dest = 0;
	}
	inline float scr_fmodf(float x, float y) {
		return y ? x - ((int)(x / y) * y) : 0;
	}

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

				Utils::GetFiberManager()->Add("MMU", [] {
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
			}
		}

		Native::StatGetInt(Context->GetArgument<uint32_t>(0), Context->GetArgument<int*>(1), Context->GetArgument<int>(2));
	}

	//TODO: MAKE THIS WORK
	Rage::eThreadState ScriptVmHook(Rage::scrValue* Stack, Rage::scrValue** Globals, Rage::scrProgram* Program, Rage::scrThreadContext* Context) {

		if (g_ScriptVmRe) {
			#ifdef HAS_ADDED_FUNCTIONALITY
			if (Context->m_script_hash == Joaat("valentinerpreward2")) {
				return Context->m_state = Rage::eThreadState::Aborted;
			}
			if (Context->m_state == Rage::eThreadState::Running) {
				g_ThreadStorageManager.Tick();


			}
#endif

			u8** opcodesTbl{ Program->m_code_blocks };
#ifdef HAS_ADDED_FUNCTIONALITY
			ScriptVMGuard* guard{ g_GlobalScriptVMGuard.CreateGuardForThread(Program, Context, opcodesTbl) };
#endif

			Rage::scrValue* sp{ Stack + Context->m_stack_pointer - 1 };
			Rage::scrValue* fp{ Stack + Context->m_frame_pointer };
			u8* pc{};
			u8* opcodes{};
			SET_PC(Context->m_instruction_pointer);
#ifdef HAS_ADDED_FUNCTIONALITY
			//PATCH_OPCODES("shop_controller", 5, "2D 01 04 00 00 2C ? ? ? 56 ? ? 71", {0x71, 0x2E, 0x01, 0x01});

			bool PatchOne = false;
			if (!PatchOne) {
				PatchScriptItem(Program, Context, Joaat("shop_controller"), "2D 01 04 00 00 2C ? ? ? 56 ? ? 71", 5, { 0x71, 0x2E, 0x01, 0x01 });//DESPWAN BYPASS
				PatchOne = true;
			}


#endif
			char buf[16]{};

			while (true) {
				u32 opcode{ LoadImm8 };
				switch (opcode) {
					CASE(OP_NOP) CHECK_PC; FETCH_INSN; NEXT_INSN;
					CASE(OP_IADD) FETCH_INSN; --sp; sp[0].Int += sp[1].Int; NEXT_INSN;
					CASE(OP_ISUB) FETCH_INSN; --sp; sp[0].Int -= sp[1].Int; NEXT_INSN;
					CASE(OP_IMUL) FETCH_INSN; --sp; sp[0].Int *= sp[1].Int; NEXT_INSN;
					CASE(OP_IDIV) FETCH_INSN; --sp; if (sp[1].Int) sp[0].Int /= sp[1].Int; NEXT_INSN;
					CASE(OP_IMOD) FETCH_INSN; --sp; if (sp[1].Int) sp[0].Int %= sp[1].Int; NEXT_INSN;
					CASE(OP_INOT) FETCH_INSN; sp[0].Int = !sp[0].Int; NEXT_INSN;
					CASE(OP_INEG) FETCH_INSN; sp[0].Int = -sp[0].Int; NEXT_INSN;

					CASE(OP_IEQ) FETCH_INSN; --sp; sp[0].Int = sp[0].Int == sp[1].Int; NEXT_INSN;
					CASE(OP_INE) FETCH_INSN; --sp; sp[0].Int = sp[0].Int != sp[1].Int; NEXT_INSN;
					CASE(OP_IGE) FETCH_INSN; --sp; sp[0].Int = sp[0].Int >= sp[1].Int; NEXT_INSN;
					CASE(OP_IGT) FETCH_INSN; --sp; sp[0].Int = sp[0].Int > sp[1].Int; NEXT_INSN;
					CASE(OP_ILE) FETCH_INSN; --sp; sp[0].Int = sp[0].Int <= sp[1].Int; NEXT_INSN;
					CASE(OP_ILT) FETCH_INSN; --sp; sp[0].Int = sp[0].Int < sp[1].Int; NEXT_INSN;

					CASE(OP_FADD) FETCH_INSN; --sp; sp[0].Float += sp[1].Float; NEXT_INSN;
					CASE(OP_FSUB) FETCH_INSN; --sp; sp[0].Float -= sp[1].Float; NEXT_INSN;
					CASE(OP_FMUL) FETCH_INSN; --sp; sp[0].Float *= sp[1].Float; NEXT_INSN;
					CASE(OP_FDIV) FETCH_INSN; --sp; if (sp[1].Int) sp[0].Float /= sp[1].Float; NEXT_INSN;
					CASE(OP_FMOD) FETCH_INSN; --sp; if (sp[1].Int) sp[0].Float = scr_fmodf(sp[0].Float, sp[1].Float); NEXT_INSN;
					CASE(OP_FNEG) FETCH_INSN; sp[0].Uns ^= static_cast<u32>(-1.f); NEXT_INSN;

					CASE(OP_FEQ) FETCH_INSN; --sp; sp[0].Int = sp[0].Float == sp[1].Float; NEXT_INSN;
					CASE(OP_FNE) FETCH_INSN; --sp; sp[0].Int = sp[0].Float != sp[1].Float; NEXT_INSN;
					CASE(OP_FGE) FETCH_INSN; --sp; sp[0].Int = sp[0].Float >= sp[1].Float; NEXT_INSN;
					CASE(OP_FGT) FETCH_INSN; --sp; sp[0].Int = sp[0].Float > sp[1].Float; NEXT_INSN;
					CASE(OP_FLE) FETCH_INSN; --sp; sp[0].Int = sp[0].Float <= sp[1].Float; NEXT_INSN;
					CASE(OP_FLT) FETCH_INSN; --sp; sp[0].Int = sp[0].Float < sp[1].Float; NEXT_INSN;

					CASE(OP_VADD) FETCH_INSN; sp -= 3; sp[-2].Float += sp[1].Float; sp[-1].Float += sp[2].Float; sp[0].Float += sp[3].Float; NEXT_INSN;
					CASE(OP_VSUB) FETCH_INSN; sp -= 3; sp[-2].Float -= sp[1].Float; sp[-1].Float -= sp[2].Float; sp[0].Float -= sp[3].Float; NEXT_INSN;
					CASE(OP_VMUL) FETCH_INSN; sp -= 3; sp[-2].Float *= sp[1].Float; sp[-1].Float *= sp[2].Float; sp[0].Float *= sp[3].Float; NEXT_INSN;
					CASE(OP_VDIV) FETCH_INSN; sp -= 3; if (sp[1].Int) sp[-2].Float /= sp[1].Float; if (sp[2].Int) sp[-1].Float /= sp[2].Float; if (sp[3].Int) sp[0].Float /= sp[3].Float; NEXT_INSN;
					CASE(OP_VNEG) FETCH_INSN; sp[-2].Uns ^= static_cast<u32>(-1.f); sp[-1].Uns ^= static_cast<u32>(-1.f); sp[0].Uns ^= static_cast<u32>(-1.f); NEXT_INSN;

					CASE(OP_IAND) FETCH_INSN; --sp; sp[0].Int &= sp[1].Int; NEXT_INSN;
					CASE(OP_IOR)  FETCH_INSN; --sp; sp[0].Int |= sp[1].Int; NEXT_INSN;
					CASE(OP_IXOR) FETCH_INSN; --sp; sp[0].Int ^= sp[1].Int; NEXT_INSN;

					CASE(OP_I2F) FETCH_INSN; sp[0].Float = static_cast<float>(sp[0].Int); NEXT_INSN;
					CASE(OP_F2I) FETCH_INSN; sp[0].Int = static_cast<int32_t>(sp[0].Float); NEXT_INSN;
					CASE(OP_F2V) FETCH_INSN; sp += 2; sp[-1].Int = sp[0].Int = sp[-2].Int; NEXT_INSN;

					CASE(OP_PUSH_CONST_U8) FETCH_INSN; ++sp; sp[0].Int = LoadImm8; NEXT_INSN;
					CASE(OP_PUSH_CONST_U8_U8) FETCH_INSN; sp += 2; sp[-1].Int = LoadImm8; sp[0].Int = LoadImm8; NEXT_INSN;
					CASE(OP_PUSH_CONST_U8_U8_U8) FETCH_INSN; sp += 3; sp[-2].Int = LoadImm8; sp[-1].Int = LoadImm8; sp[0].Int = LoadImm8; NEXT_INSN;

					CASE(OP_PUSH_CONST_U32) FETCH_INSN; ++sp; sp[0].Uns = LoadImm32; NEXT_INSN;
					CASE(OP_PUSH_CONST_F) FETCH_INSN; ++sp; sp[0].Uns = LoadImm32; NEXT_INSN;

					CASE(OP_DUP) FETCH_INSN; ++sp; sp[0].Any = sp[-1].Any; NEXT_INSN;
					CASE(OP_DROP) FETCH_INSN; --sp; NEXT_INSN;

					CASE(OP_NATIVE) FETCH_INSN;
					//OgScriptVmHook(Stack, Globals, Program, Context);
					i32 returnSize{ LoadImm8 };
					i32 paramCount{ (returnSize >> 2) & 0x3F };
					i32 imm{ (LoadImm8 << 8) };
					imm |= LoadImm8;
					returnSize &= 3;
					Rage::Invoker::NativeHandler cmd{ *Program->m_native_entrypoints[imm] };
					Context->m_instruction_pointer = (i32)(pc - opcodes - 4);
					Context->m_frame_pointer = (i32)(fp - Stack);
					Context->m_stack_pointer = (i32)(sp - Stack + 1);
					//Rage::Invoker::NativeContext curInfo; //(returnSize ? &Stack[Context->m_stack_pointer - paramCount] : 0, paramCount, &Stack[Context->m_stack_pointer - paramCount]);


					//cmd(&curInfo);
	#ifdef HAS_ADDED_FUNCTIONALITY
					guard->runCallbacks();
	#endif
					if (Context->m_state != Rage::eThreadState::Running)
						return Context->m_state;

					//curInfo.UpdateResultVectors(curInfo);
					sp -= paramCount - returnSize;
					NEXT_INSN;

					CASE(OP_ENTER) FETCH_INSN;
					u32 paramCount{ LoadImm8 };
					u32 localCount{ LoadImm16 };
					u32 nameCount{ LoadImm8 };
					if (Context->m_call_depth < 16) {
						Context->m_callstack[Context->m_call_depth] = (i32)(pc - opcodes + 2);
					}
					Context->m_call_depth += 1;
					pc += nameCount;
					if (sp - Stack >= (i32)(Context->m_stack_size - localCount)) {
						LOG_ERROR("Stack Overflow")
					}
					(++sp)->Int = (i32)(fp - Stack);
					fp = sp - paramCount - 1;
					while (localCount--)
						(++sp)->Any = 0;
					sp -= paramCount;
					NEXT_INSN;

					CASE(OP_LEAVE) FETCH_INSN;
					Context->m_call_depth -= 1;
					u32 paramCount{ LoadImm8 };
					u32 returnSize{ LoadImm8 };
					Rage::scrValue* result{ sp - returnSize };
					sp = fp + paramCount - 1;
					fp = Stack + sp[2].Uns;
					u32 newPc{ sp[1].Uns };
					SET_PC(newPc);
					sp -= paramCount;
					while (returnSize--) {
						*++sp = *++result;
					}
					if (!newPc) {
						return Context->m_state = Rage::eThreadState::Halted;
					}
					NEXT_INSN;

					CASE(OP_LOAD) FETCH_INSN; sp[0].Any = (sp[0].Reference)->Any; NEXT_INSN;
					CASE(OP_STORE) FETCH_INSN; sp -= 2; (sp[2].Reference)->Any = sp[1].Any; NEXT_INSN;
					CASE(OP_STORE_REV) FETCH_INSN; --sp; (sp[0].Reference)->Any = sp[1].Any; NEXT_INSN;

					CASE(OP_LOAD_N) FETCH_INSN;
					Rage::scrValue* addr{ ((sp--)->Reference) };
					u32 count{ (sp--)->Uns };
					for (u32 i{}; i < count; i++)
						(++sp)->Any = addr[i].Any;
					NEXT_INSN;

					CASE(OP_STORE_N) FETCH_INSN;
					Rage::scrValue* addr{ ((sp--)->Reference) };
					u32 count{ (sp--)->Uns };
					for (u32 i{}; i < count; i++)
						addr[count - 1 - i].Any = (sp--)->Any;
					NEXT_INSN;

					CASE(OP_ARRAY_U8) FETCH_INSN;
					--sp;
					Rage::scrValue* r{ sp[1].Reference };
					size_t idx{ sp[0].Uns };
#ifdef HAS_ADDED_FUNCTIONALITY
					if (idx >= r->Uns) {
						LOG_ERROR("Blocked Array Kick");
					}
#elif
					if (idx >= r->Uns) {
						FAULT("Array overrun, {} >= {}", idx, r->Uns);
					}
#endif
					r += 1U + idx * LoadImm8;
					sp[0].Reference = r;
					NEXT_INSN;

					CASE(OP_ARRAY_U8_LOAD) FETCH_INSN;
					--sp;
					Rage::scrValue* r{ sp[1].Reference };
					size_t idx{ sp[0].Uns };
#ifdef HAS_ADDED_FUNCTIONALITY
					if (idx >= r->Uns) {
						LOG_ERROR("Blocked Array Kick");
					}
#elif
					if (idx >= r->Uns) {
						FAULT("Array overrun, {} >= {}", idx, r->Uns);
					}
#endif
					r += 1U + idx * LoadImm8;
					sp[0].Any = r->Any;
					NEXT_INSN;

					CASE(OP_ARRAY_U8_STORE) FETCH_INSN;
					sp -= 3;
					Rage::scrValue* r{ sp[3].Reference };
					size_t idx{ sp[2].Uns };
#ifdef HAS_ADDED_FUNCTIONALITY
					if (idx >= r->Uns) {
						LOG_ERROR("Blocked Array Kick");
					}
#elif
					if (idx >= r->Uns) {
						FAULT("Array overrun, {} >= {}", idx, r->Uns);
					}
#endif
					r += 1U + idx * LoadImm8;
					r->Any = sp[1].Any;
					NEXT_INSN;

					CASE(OP_LOCAL_U8) FETCH_INSN; ++sp; sp[0].Reference = (fp + LoadImm8); NEXT_INSN;
					CASE(OP_LOCAL_U8_LOAD) FETCH_INSN; ++sp; sp[0].Any = fp[LoadImm8].Any; NEXT_INSN;
					CASE(OP_LOCAL_U8_STORE) FETCH_INSN; --sp; fp[LoadImm8].Any = sp[1].Any; NEXT_INSN;

					CASE(OP_STATIC_U8) FETCH_INSN; ++sp; sp[0].Reference = (Stack + LoadImm8); NEXT_INSN;
					CASE(OP_STATIC_U8_LOAD) FETCH_INSN; ++sp; sp[0].Any = Stack[LoadImm8].Any; NEXT_INSN;
					CASE(OP_STATIC_U8_STORE) FETCH_INSN; --sp; Stack[LoadImm8].Any = sp[1].Any; NEXT_INSN;

					CASE(OP_IADD_U8) FETCH_INSN; sp[0].Int += LoadImm8; NEXT_INSN;
					CASE(OP_IMUL_U8) FETCH_INSN; sp[0].Int *= LoadImm8; NEXT_INSN;

					CASE(OP_IOFFSET) FETCH_INSN; --sp; sp[0].Any += sp[1].Int * sizeof(Rage::scrValue); NEXT_INSN;
					CASE(OP_IOFFSET_U8) FETCH_INSN; sp[0].Any += LoadImm8 * sizeof(Rage::scrValue); NEXT_INSN;
					CASE(OP_IOFFSET_U8_LOAD) FETCH_INSN; sp[0].Any = (sp[0].Reference)[LoadImm8].Any; NEXT_INSN;
					CASE(OP_IOFFSET_U8_STORE) FETCH_INSN; sp -= 2; (sp[2].Reference)[LoadImm8].Any = sp[1].Any; NEXT_INSN;

					CASE(OP_PUSH_CONST_S16) FETCH_INSN; ++sp; sp[0].Int = LoadImmS16; NEXT_INSN;
					CASE(OP_IADD_S16) FETCH_INSN; sp[0].Int += LoadImmS16; NEXT_INSN;
					CASE(OP_IMUL_S16) FETCH_INSN; sp[0].Int *= LoadImmS16; NEXT_INSN;

					CASE(OP_IOFFSET_S16) FETCH_INSN; sp[0].Any += LoadImmS16 * sizeof(Rage::scrValue); NEXT_INSN;
					CASE(OP_IOFFSET_S16_LOAD) FETCH_INSN; sp[0].Any = (sp[0].Reference)[LoadImmS16].Any; NEXT_INSN;
					CASE(OP_IOFFSET_S16_STORE) FETCH_INSN; sp -= 2; (sp[2].Reference)[LoadImmS16].Any = sp[1].Any; NEXT_INSN;

					CASE(OP_ARRAY_U16) FETCH_INSN;
					--sp;
					Rage::scrValue* r{ sp[1].Reference };
					size_t idx{ sp[0].Uns };
#ifdef HAS_ADDED_FUNCTIONALITY
					if (idx >= r->Uns) {
						LOG_ERROR("Blocked Array Kick");
					}
#elif
					if (idx >= r->Uns) {
						FAULT("Array overrun, {} >= {}", idx, r->Uns);
					}
#endif
					r += 1U + idx * LoadImm16;
					sp[0].Reference = r;
					NEXT_INSN;

					CASE(OP_ARRAY_U16_LOAD) FETCH_INSN;
					--sp;
					Rage::scrValue* r{ sp[1].Reference };
					size_t idx{ sp[0].Uns };
#ifdef HAS_ADDED_FUNCTIONALITY
					if (idx >= r->Uns) {
						LOG_ERROR("Blocked Array Kick");
					}
#elif
					if (idx >= r->Uns) {
						FAULT("Array overrun, {} >= {}", idx, r->Uns);
					}
#endif
					r += 1U + idx * LoadImm16;
					sp[0].Any = r->Any;
					NEXT_INSN;

					CASE(OP_ARRAY_U16_STORE) FETCH_INSN;
					sp -= 3;
					Rage::scrValue* r{ sp[3].Reference };
					u32 idx{ sp[2].Uns };
#ifdef HAS_ADDED_FUNCTIONALITY
					if (idx >= r->Uns) {
						LOG_ERROR("Blocked Array Kick");
					}
#elif
					if (idx >= r->Uns) {
						FAULT("Array overrun, {} >= {}", idx, r->Uns);
					}
#endif
					r += 1U + idx * LoadImm16;
					r->Any = sp[1].Any;
					NEXT_INSN;

					CASE(OP_LOCAL_U16) FETCH_INSN; ++sp; sp[0].Reference = (fp + LoadImm16); NEXT_INSN;
					CASE(OP_LOCAL_U16_LOAD) FETCH_INSN; ++sp; sp[0].Any = fp[LoadImm16].Any; NEXT_INSN;
					CASE(OP_LOCAL_U16_STORE) FETCH_INSN; --sp; fp[LoadImm16].Any = sp[1].Any; NEXT_INSN;

					CASE(OP_STATIC_U16) FETCH_INSN; ++sp; sp[0].Reference = (Stack + LoadImm16); NEXT_INSN;
					CASE(OP_STATIC_U16_LOAD) FETCH_INSN; ++sp; sp[0].Any = Stack[LoadImm16].Any; NEXT_INSN;
					CASE(OP_STATIC_U16_STORE) FETCH_INSN; --sp; Stack[LoadImm16].Any = sp[1].Any; NEXT_INSN;

					CASE(OP_GLOBAL_U16) FETCH_INSN; ++sp; sp[0].Reference = (Globals[0] + LoadImm16); NEXT_INSN;
					CASE(OP_GLOBAL_U16_LOAD) FETCH_INSN; ++sp; sp[0].Any = Globals[0][LoadImm16].Any; NEXT_INSN;
					CASE(OP_GLOBAL_U16_STORE) FETCH_INSN; --sp; Globals[0][LoadImm16].Any = sp[1].Any; NEXT_INSN;

					CASE(OP_STATIC_U24) FETCH_INSN; ++sp; sp[0].Reference = (Stack + LoadImm24); NEXT_INSN;
					CASE(OP_STATIC_U24_LOAD) FETCH_INSN; ++sp; sp[0].Any = Stack[LoadImm24].Any; NEXT_INSN;
					CASE(OP_STATIC_U24_STORE) FETCH_INSN; --sp; Stack[LoadImm24].Any = sp[1].Any; NEXT_INSN;

					CASE(OP_GLOBAL_U24) FETCH_INSN;
					u32 imm{ LoadImm24 }, block{ imm >> 18 }, ofs{ imm & 0x3FFFF };
					++sp;
					if (!Globals[block]) {
						//FAULT("Global block {}/{} not resident", block, ofs);
						LOG_ERROR(std::format("Global block {}/{} not resident", block, ofs).c_str());
						return Context->m_state = Rage::eThreadState::Halted;
					}
					else {
						sp[0].Reference = (&Globals[block][ofs]);
					}
					NEXT_INSN;

					CASE(OP_GLOBAL_U24_LOAD) FETCH_INSN;
					u32 imm{ LoadImm24 }, block{ imm >> 18 }, ofs{ imm & 0x3FFFF };
					++sp;
					if (!Globals[block]) {
						LOG_ERROR(std::format("Global block {}/{} not resident", block, ofs).c_str());
						return Context->m_state = Rage::eThreadState::Halted;
					}
					else {
						sp[0].Any = Globals[block][ofs].Any;
					}
					NEXT_INSN;

					CASE(OP_GLOBAL_U24_STORE) FETCH_INSN;
					u32 imm{ LoadImm24 }, block{ imm >> 18 }, ofs{ imm & 0x3FFFF };
					--sp;
					if (!Globals[block]) {
						LOG_ERROR(std::format("Global block {}/{} not resident", block, ofs).c_str());
						return Context->m_state = Rage::eThreadState::Halted;
					}
					else {
						Globals[block][ofs].Any = sp[1].Any;
					}
					NEXT_INSN;

					CASE(OP_PUSH_CONST_U24) FETCH_INSN; ++sp; sp[0].Int = LoadImm24; NEXT_INSN;

					CASE(OP_CALL) FETCH_INSN; u32 imm = LoadImm24; ++sp; sp[0].Uns = (i32)(pc - opcodes); SET_PC(imm); NEXT_INSN;

					CASE(OP_J) FETCH_INSN; i32 imm{ LoadImmS16 }; ADD_PC(imm); NEXT_INSN;
					CASE(OP_JZ) FETCH_INSN; i32 imm{ LoadImmS16 }; --sp; if (sp[1].Int == 0) ADD_PC(imm); else CHECK_PC; NEXT_INSN;
					CASE(OP_IEQ_JZ) FETCH_INSN; i32 imm{ LoadImmS16 }; sp -= 2; if (!(sp[1].Int == sp[2].Int)) ADD_PC(imm); else CHECK_PC; NEXT_INSN;
					CASE(OP_INE_JZ) FETCH_INSN; i32 imm{ LoadImmS16 }; sp -= 2; if (!(sp[1].Int != sp[2].Int)) ADD_PC(imm); else CHECK_PC; NEXT_INSN;
					CASE(OP_IGE_JZ) FETCH_INSN; i32 imm{ LoadImmS16 }; sp -= 2; if (!(sp[1].Int >= sp[2].Int)) ADD_PC(imm); else CHECK_PC; NEXT_INSN;
					CASE(OP_IGT_JZ) FETCH_INSN; i32 imm{ LoadImmS16 }; sp -= 2; if (!(sp[1].Int > sp[2].Int)) ADD_PC(imm); else CHECK_PC; NEXT_INSN;
					CASE(OP_ILE_JZ) FETCH_INSN; i32 imm{ LoadImmS16 }; sp -= 2; if (!(sp[1].Int <= sp[2].Int)) ADD_PC(imm); else CHECK_PC; NEXT_INSN;
					CASE(OP_ILT_JZ) FETCH_INSN; i32 imm{ LoadImmS16 }; sp -= 2; if (!(sp[1].Int < sp[2].Int)) ADD_PC(imm); else CHECK_PC; NEXT_INSN;

					CASE(OP_SWITCH) FETCH_INSN;
					--sp;
					u32 label{ sp[1].Uns };
					u32 count{ LoadImm8 };
					CHECK_PC;
					for (u32 i{}; i != count; ++i) {
						u32 match{ LoadImm32 };
						u32 target{ LoadImm16 };
						if (label == match) {
							ADD_PC(target);
							break;
						}
					}
					CHECK_PC;
					NEXT_INSN;

					CASE(OP_STRING) FETCH_INSN;
					u32 offset{ sp[0].Uns };
					sp[0].String = (Program->m_strings_data[offset >> 14] + (offset & 0x3FFF));
					NEXT_INSN;

					CASE(OP_TEXT_LABEL_ASSIGN_STRING) FETCH_INSN;
					sp -= 2;
					char* dest{ const_cast<char*>(sp[2].String) };
					const char* src{ sp[1].String };
					scr_assign_string(dest, LoadImm8, src);
					NEXT_INSN;

					CASE(OP_TEXT_LABEL_ASSIGN_INT) FETCH_INSN;
					sp -= 2;
					char* dest{ const_cast<char*>(sp[2].String) };
					i32 value{ sp[1].Int };
					scr_itoa(buf, value);
					scr_assign_string(dest, LoadImm8, buf);
					NEXT_INSN;

					CASE(OP_TEXT_LABEL_APPEND_STRING) FETCH_INSN;
					sp -= 2;
					char* dest{ const_cast<char*>(sp[2].String) };
					const char* src{ sp[1].String };
					scr_append_string(dest, LoadImm8, src);
					NEXT_INSN;

					CASE(OP_TEXT_LABEL_APPEND_INT) FETCH_INSN;
					sp -= 2;
					char* dest{ const_cast<char*>(sp[2].String) };
					i32 value{ sp[1].Int };
					scr_itoa(buf, value);
					scr_append_string(dest, LoadImm8, buf);
					NEXT_INSN;

					CASE(OP_TEXT_LABEL_COPY) FETCH_INSN;
					sp -= 3;
					Rage::scrValue* dest{ sp[3].Reference };
					i32 destSize{ sp[2].Int };
					i32 srcSize{ sp[1].Int };
					//Remove excess
					while (srcSize > destSize) {
						--srcSize;
						--sp;
					}
					//Do the bulk of the copy
					for (i32 i{}; i != srcSize; ++i)
						dest[srcSize - 1 - i].Reference = (sp--)->Reference;
					//Make sure it's still NUL-terminated
					char* cDest{ (char*)dest };
					cDest[(srcSize * sizeof(Rage::scrValue)) - 1] = '\0';
					NEXT_INSN;

					CASE(OP_CATCH) FETCH_INSN;
					Context->m_catch_pointer_count = (i32)(pc - opcodes);
					Context->m_catch_frame_pointer = (i32)(fp - Stack);
					Context->m_catch_stack_pointer = (i32)(sp - Stack + 1);
					++sp;
					sp[0].Int = -1;
					NEXT_INSN;

					CASE(OP_THROW) FETCH_INSN;
					int32_t imm{ sp[0].Int };
					if (!Context->m_catch_pointer_count) {
						LOG_ERROR("THROW with no CATCH");
					}
					else {
						SET_PC(Context->m_catch_pointer_count);
						fp = Stack + Context->m_catch_frame_pointer;
						sp = Stack + Context->m_catch_stack_pointer;
						sp[0].Int = imm;
					}
					NEXT_INSN;

					CASE(OP_CALLINDIRECT) FETCH_INSN;
					u32 imm{ sp[0].Uns };
					if (!imm)
						LOG_ERROR("Attempted to call through uninitialized (zero) function pointer");
					sp[0].Uns = (int32_t)(pc - opcodes);
					SET_PC(imm);
					NEXT_INSN;

					CASE(OP_PUSH_CONST_M1) FETCH_INSN; ++sp; sp[0].Int = -1; NEXT_INSN;
					CASE(OP_PUSH_CONST_0) FETCH_INSN; ++sp; sp[0].Any = 0; NEXT_INSN;
					CASE(OP_PUSH_CONST_1) FETCH_INSN; ++sp; sp[0].Int = 1; NEXT_INSN;
					CASE(OP_PUSH_CONST_2) FETCH_INSN; ++sp; sp[0].Int = 2; NEXT_INSN;
					CASE(OP_PUSH_CONST_3) FETCH_INSN; ++sp; sp[0].Int = 3; NEXT_INSN;
					CASE(OP_PUSH_CONST_4) FETCH_INSN; ++sp; sp[0].Int = 4; NEXT_INSN;
					CASE(OP_PUSH_CONST_5) FETCH_INSN; ++sp; sp[0].Int = 5; NEXT_INSN;
					CASE(OP_PUSH_CONST_6) FETCH_INSN; ++sp; sp[0].Int = 6; NEXT_INSN;
					CASE(OP_PUSH_CONST_7) FETCH_INSN; ++sp; sp[0].Int = 7; NEXT_INSN;

					CASE(OP_PUSH_CONST_FM1) FETCH_INSN; ++sp; sp[0].Float = -1.f; NEXT_INSN;
					CASE(OP_PUSH_CONST_F0) FETCH_INSN; ++sp; sp[0].Float = 0.f; NEXT_INSN;
					CASE(OP_PUSH_CONST_F1) FETCH_INSN; ++sp; sp[0].Float = 1.f; NEXT_INSN;
					CASE(OP_PUSH_CONST_F2) FETCH_INSN; ++sp; sp[0].Float = 2.f; NEXT_INSN;
					CASE(OP_PUSH_CONST_F3) FETCH_INSN; ++sp; sp[0].Float = 3.f; NEXT_INSN;
					CASE(OP_PUSH_CONST_F4) FETCH_INSN; ++sp; sp[0].Float = 4.f; NEXT_INSN;
					CASE(OP_PUSH_CONST_F5) FETCH_INSN; ++sp; sp[0].Float = 5.f; NEXT_INSN;
					CASE(OP_PUSH_CONST_F6) FETCH_INSN; ++sp; sp[0].Float = 6.f; NEXT_INSN;
					CASE(OP_PUSH_CONST_F7) FETCH_INSN; ++sp; sp[0].Float = 7.f; NEXT_INSN;

					CASE(OP_STRINGHASH) FETCH_INSN; sp[0].Uns = Rage::LowerCaseJoaat(sp[0].String); NEXT_INSN;

					CASE(OP_IS_BIT_SET) FETCH_INSN;
					--sp;
					sp[0].Int = ((sp[0].Int & (1 << sp[1].Int)) != 0);
					NEXT_INSN;
					DEFAULT LOG_ERROR(std::format("Invalid opcode! ({} on thread {}, 0x{:X})", opcode, Program->m_name, Program->m_name_hash).c_str());
				}
			}
		
		}
		else {
			bool PatchOne = false;
			if (!PatchOne) {
				//PatchScriptItem(Program, Context, Joaat("shop_controller"), "2D 01 04 00 00 2C ? ? ? 56 ? ? 71", 5, { 0x71, 0x2E, 0x01, 0x01 });
			//	PatchScriptItem(Program, Context, Joaat("freemode"),"2D 01 04 00 ? 2C ? ? ? 5D ? ? ? 71 57 ? ? 2C", 5, { 0x2E, 0x01, 0x00 });
				//PatchScriptItem(Program, Context, Joaat("freemode"),  "5D ? ? ? 76 57 ? ? 5D ? ? ? 76", 0, { 0x2E, 0x00, 0x00 });
				//PatchScriptItem(Program, Context, Joaat("freemode"),  "2D 01 09 00 00 5D ? ? ? 56 ? ? 3A", 5, { 0x2E, 0x01, 0x00 });
				//PatchScriptItem(Program, Context, Joaat("freemode"),  "71 2E ? ? 55 ? ? 61 ? ? ? 47 ? ? 63", 0, { 0x72 });
				//PatchScriptItem(Program, Context, Joaat("freemode"),  "2D 00 07 00 00 7B", 5, { 0x2E, 0x00, 0x00 });
				//PatchScriptItem(Program, Context, Joaat("shop_controller"),  "2D 01 03 00 00 5D ? ? ? 06 56 ? ? 2E ? ? 2C", 5, { 0x2E, 0x01, 0x00 });
				PatchOne = true;
			}
			return OgScriptVmHook(Stack, Globals, Program, Context);
		}

	}
}