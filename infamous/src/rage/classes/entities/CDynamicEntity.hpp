#pragma once

#include "CEntity.hpp"
#include "../network/netObject.hpp"

#include <cstdint>

namespace Rage
{
    class CDynamicEntity : public CEntity
    {
    public:
        class Rage::netObject* m_net_object; //0x00D0
        char gapD8[16];
        uint64_t qwordE8;
    };
    static_assert(sizeof(CDynamicEntity) == 0xF0);
}