#include "Transferer.h"
#include <regex>

namespace BDXKEngine
{
    std::function<void(void*, const Type&)> Transferer::GetTransferFunc(const Type& type)
    {
        const auto transferFunc = transferFuncs[type];
        return transferFunc != nullptr ? transferFunc : GetTransferFuncFallback(type);
    }
    int Transferer::GetTypeSize(const Type& type)
    {
        const int size = typeSizes[type];
        return size != 0 ? size : GetTypeSizeFallback(type);
    }

    void Transferer::PushPath(const std::string& key)
    {
    }
    void Transferer::PopPath(std::string& key)
    {
    }
    void Transferer::TransferNested(std::string key, Transferable& value)
    {
        PushPath(key);
        value.Transfer(*this);
        PopPath(key);
    }

    void Transferer::TransferField(std::string key, void* value, const Type& type)
    {
        PushPath(key);
        TransferValue(value, type);
        PopPath(key);
    }

    void Transferer::TransferValue(void* value, const Type& type)
    {
        const auto transferFunc = GetTransferFunc(type);
        if (transferFunc != nullptr)transferFunc(value, type);
        else TransferValueFallback(value, type);
    }


    void Transferer::TransferValueFallback(void* value, const Type& type)
    {
        if (type.find("class std::vector") != std::string::npos)
        {
            //解析模板类型
            std::smatch matchResult;
            std::regex_search(type, matchResult, std::regex{"class std::vector<(.*)>"}); //(?<=<).*(?=,)
            const std::string matchString = matchResult[1];

            Type itemType;
            size_t itemSize;
            if (std::regex_search(
                    matchString, matchResult,
                    std::regex{"(class std::vector<.*,class std::allocator<.*> >),class std::allocator"}) == true
            )
            {
                itemType = matchResult[1];
                itemSize = sizeof std::vector<void*>;
            }
            else
            {
                std::regex_search(matchString, matchResult, std::regex{"(.*?),"});

                itemType = matchResult[1];
                itemSize = GetTypeSize(itemType);
            }

            if (itemSize != 0)
            {
                std::vector<char>& vector = *static_cast<std::vector<char>*>(value);
                int count = static_cast<int>(vector.size() / itemSize);

                TransferField("count", count);
                vector.resize(count * itemSize);
                for (int index = 0; index < count; index++)
                {
                    TransferField("item" + std::to_string(index), vector.data() + index * itemSize, itemType);
                }
                return;
            }
        }

        throw std::exception((type + "无法找到可用的传输方式").c_str());
    }
    std::function<void(void*, const Type&)> Transferer::GetTransferFuncFallback(const Type& type)
    {
        return nullptr;
    }
    int Transferer::GetTypeSizeFallback(const Type& type)
    {
        return 0;
    }
}
