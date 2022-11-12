#include "Transferer.h"

namespace BDXKEngine
{
    Transferer::Transferer(bool isReadOnly)
    {
        this->isReadOnly = isReadOnly;
    }
    bool Transferer::GetIsReadOnly() const
    {
        return isReadOnly;
    }

    void Transferer::PushPath(const std::string& key)
    {
    }
    void Transferer::PopPath(std::string& key)
    {
    }
    void Transferer::TransferValue(void* value, const Type& type)
    {
        throw std::exception((type + "的传输方式未定义").c_str());
    }
}
