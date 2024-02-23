#pragma once
#include <cstdint>
#include "stdafx.hpp"
#include "rage/classes/rage/joaat.hpp"
namespace Rage
{

    enum class eThreadState : uint32_t {
        Running,
        Blocked,
        Aborted,
        Halted,
        ResetInstructionCount
    };

    class scrThreadContext
    {
    public:
        std::uint32_t m_thread_id;           // 0x00
        joaat_t m_script_hash;               // 0x04
        eThreadState m_state;                // 0x08
        std::uint32_t m_instruction_pointer; // 0x0C
        std::uint32_t m_frame_pointer;       // 0x10
        std::uint32_t m_stack_pointer;       // 0x14
        float m_timer_a;                     // 0x18
        float m_timer_b;                     // 0x1C
        float m_wait_timer;                  // 0x20
        char m_padding1[0x2C];               // 0x24
        std::uint32_t m_stack_size;          // 0x50
        uint32_t m_catch_pointer_count; //0x0054
        uint32_t m_catch_frame_pointer; //0x0058
        uint32_t m_catch_stack_pointer; //0x005C
        uint32_t m_priority; //0x0060
        uint8_t m_call_depth; //0x0060
        uint8_t unk_0061; //0x0061
        uint16_t unk_0062; //0x0062
        char m_callstack[16]; //0x0068
    };
    static_assert(sizeof(scrThreadContext) == 0x78);
}