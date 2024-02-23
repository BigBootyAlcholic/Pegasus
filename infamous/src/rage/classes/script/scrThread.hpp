#pragma once
#include "scriptHandler.hpp"
#include "scriptHandlerNetComponent.hpp"
#include "scrThreadContext.hpp"
#include "../rage/scrValue.hpp"

namespace Rage
{
    class scrThread
    {
    public:
        virtual ~scrThread() = default;                                                                 // 0 (0x00)
        virtual void reset(std::uint32_t script_hash, void* args, std::uint32_t arg_count) = 0;         // 1 (0x08)
        virtual eThreadState run() = 0;                                                                 // 2 (0x10)
        virtual eThreadState tick(std::uint32_t ops_to_execute) = 0;                                    // 3 (0x18)
        virtual void kill() = 0;

    public:
        class Info {
        public:
            Info(scrValue* resultPtr, int parameterCount, scrValue* params) :
                ResultPtr(resultPtr), ParamCount(parameterCount), Params(params), BufferCount(0) { }
            //Return result, if applicable
            scrValue* ResultPtr;
            //Parameter count
            int ParamCount;
            //Pointer to parameter values
            scrValue* Params;
            int BufferCount;
            scrValue* Orig[4];
            Math::Vector3<float> Buffer[4];
            void CopyReferencedParametersOut() {
                int bc = BufferCount;
                while (bc--) {
                    Orig[bc][0].Float = Buffer[bc].m_X;
                    Orig[bc][1].Float = Buffer[bc].m_Y;
                    Orig[bc][2].Float = Buffer[bc].m_Z;
                }
            }
        };
        scrThreadContext m_context;                 // 0x08
        scrValue* m_stack;                          // 0xB0
        char m_padding[0x4];                        // 0xB8
        uint32_t m_arg_size;                        // 0xBC
        uint32_t m_arg_loc;                         // 0xC0
        char m_padding2[0x4];                       // 0xC4
        const char* m_exit_message;                 // 0xC8
        char m_pad[0x4];                            // 0xD0
        char m_name[0x40];                          // 0xD4
        scriptHandler* m_handler;                   // 0x118
        scriptHandlerNetComponent* m_net_component; // 0x120
    };
}