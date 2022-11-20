#include "ZTest.h"

namespace BDXKEngine
{
    ZTest ZTest::Default = {};

    void ZTest::Transfer(Transferer& transferer)
    {
        TransferFieldInfoOf(operation, int);
        TransferFieldInfo(factor);
        TransferFieldInfo(units);
        TransferFieldInfo(write);
        TransferFieldInfo(clip);
    }
}
