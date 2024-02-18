#pragma once

#include "datBase.hpp"
#include "fwRefAwareBaseImpl.hpp"

namespace Rage
{

    class fwRefAwareBase : public fwRefAwareBaseImpl<datBase>
    {
    };
    static_assert(sizeof(fwRefAwareBase) == 0x10);

}