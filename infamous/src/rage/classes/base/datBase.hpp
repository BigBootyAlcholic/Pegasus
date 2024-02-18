#pragma once

namespace Rage
{

    class datBase
    {
    public:
        virtual ~datBase() = default;
    }; //Size: 0x0008
    static_assert(sizeof(datBase) == 0x8);

}