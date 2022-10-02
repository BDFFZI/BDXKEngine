#pragma once
#include "BDXKEngine/Base/Serialization/Transferrer.h"

namespace BDXKEngine
{
    class BinaryTransferrer : public Transferrer
    {
    protected:
        BinaryTransferrer(std::iostream& stream);
        void PushPath(const std::string& key) override;
        void PopPath(std::string& key) override;
        std::iostream& stream;
    };
}
