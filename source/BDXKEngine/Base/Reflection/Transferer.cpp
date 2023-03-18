#include "Transferer.h"
#include <regex>

namespace BDXKEngine
{
    std::function<void(void*, const Type&)> Transferer::GetTransferFunc(const Type& type)
    {
        const auto transferFunc = transferFuncs.find(type);
        return transferFunc != transferFuncs.end() ? transferFunc->second : GetTransferFuncFallback(type);
    }
    int Transferer::GetTypeSize(const Type& type)
    {
        const int size = typeSizes[type];
        return size != 0 ? size : GetTypeSizeFallback(type);
    }
    bool Transferer::IsContainTransferFunc(const Type& type) const
    {
        return transferFuncs.contains(type);
    }

    void Transferer::PushPath(const std::string& key)
    {
    }
    void Transferer::PopPath(std::string& key)
    {
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

            Type itemType = {};
            size_t itemSize = 0;
            if (std::regex_search( //判断是否是vector嵌套
                    matchString, matchResult,
                    std::regex{"(class std::vector<.*,class std::allocator<.*> >),class std::allocator"}) == true
            )
            {
                itemType = matchResult[1];
                itemSize = sizeof std::vector<void*>;
            }
            else //是基本元素
            {
                //优先显示定义的类型
                for (const auto& item : typeSizes)
                {
                    if (matchString.starts_with(item.first + ","))
                    {
                        itemType = item.first;
                        itemSize = item.second;
                        break;
                    }
                }

                //无法确定类型，最后放手一搏
                if (itemSize == 0)
                {
                    std::regex_search(matchString, matchResult, std::regex{"(.*?),"});
                    itemType = matchResult[1];
                    itemSize = GetTypeSize(itemType);
                }
            }

            if (itemSize != 0)
            {
                if (itemType == GetTypeOf<bool>()) //虽然bool以byte为单位，但vector<bool>似乎以bit的方式储存，所以得特殊处理
                {
                    std::vector<bool>& vector = *static_cast<std::vector<bool>*>(value);
                    int count = static_cast<int>(vector.size() / itemSize);

                    TransferField("count", count);
                    vector.resize(count * itemSize);
                    for (int index = 0; index < count; index++)
                    {
                        bool element = vector[index];
                        TransferField("item" + std::to_string(index), element);
                        vector[index] = element;
                    }
                }
                else if (itemType == GetTypeOf<std::string>())
                {
                    std::vector<std::string>& vector = *static_cast<std::vector<std::string>*>(value);

                    int count = static_cast<int>(vector.size());
                    TransferField("count", count);
                    vector.resize(count);
                    for (int index = 0; index < count; index++)
                    {
                        TransferField("item" + std::to_string(index), vector[index]);
                    }
                }
                else //注意以这种方式创建的对象没有虚表信息，可能会出问题，需自行额外处理(可以参照ObjectTransferer)
                {
                    std::vector<char>& vector = *static_cast<std::vector<char>*>(value);
                    int count = static_cast<int>(vector.size() / itemSize);

                    TransferField("count", count);
                    vector.resize(count * itemSize);
                    for (int index = 0; index < count; index++)
                    {
                        TransferField("item" + std::to_string(index), vector.data() + index * itemSize, itemType);
                    }
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
