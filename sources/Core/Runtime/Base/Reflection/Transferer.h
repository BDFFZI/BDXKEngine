#pragma once
#include <functional>
#include <unordered_map>
#include "Type.h"

namespace BDXKEngine
{
    //只专注于传送功能，能传送各种东西，允许添加自定义类型的传输方式
    //支持传输std::vector，但创建其元素时不会处理虚表的问题，需要自行解决，比如在传输方式内设置虚表
    //std::vector<std::string>和std::vector<bool>已经特殊处理
    class Transferer
    {
    public:
#define TransferFieldInfoRecursive(fieldName) transferer.TransferFieldRecursive(#fieldName, fieldName)
#define TransferFieldInfoOf(fieldName,transferType) transferer.TransferFieldOf<transferType>(#fieldName, fieldName)
#define TransferFieldInfo(fieldName) transferer.TransferField(#fieldName, fieldName)

        //设置传输函数(额外会记录字段大小，用于std::vector的序列化)
        template <typename TType, typename TFunc>
        void SetTransfer(TFunc func)
        {
            const Type type = GetTypeOf<TType>();
            transferFuncs[type] = [func](void* value, const Type&)
            {
                auto* target = static_cast<TType*>(value);
                func(*target);
            };
            transferSizes[type] = sizeof(TType);
        }
        bool HasTransfer(const Type& type) const;

        //传输字段（自动设置路径）
        template <typename TField>
        void TransferField(std::string key, TField& value)
        {
            PushPath(key);

            TransferValue(value);

            PopPath(key);
        }
        template <typename TTransfer, typename TField>
        void TransferFieldOf(const std::string key, TField& value)
        {
            TransferField<TTransfer>(key, *reinterpret_cast<TTransfer*>(&value));
        }
        template <typename TField>
        void TransferFieldRecursive(std::string key, TField& value)
        {
            PushPath(key);
            TransferValueRecursive(value);
            PopPath(key);
        }
        void TransferFieldNative(std::string key, void* value, const Type& type);

        //设置路径
        virtual void PushPath(const std::string& key);
        virtual void PopPath(std::string& key);

        //传输值
        template <typename TValue>
        void TransferValue(TValue& value)
        {
            TransferValueNative(&value, GetTypeOf<TValue>());
        }
        template <typename TTransfer, typename TSource>
        void TransferValueOf(TSource& value)
        {
            TransferValue(*reinterpret_cast<TTransfer*>(&value));
        }
        template <typename TField>
        void TransferValueRecursive(TField& value)
        {
            value.Transfer(*this);
        }
        void TransferValueNative(void* value, const Type& type);

        Transferer() = default;
        virtual ~Transferer() = default;

    protected:
        //获取传输信息前都会通过该函数过一遍，可以借此调整类型以实现泛型处理，甚至转换为某种特定类型以实现跳过传输。
        //总之这是传输前的最后一道关卡，通过调整类型来引导最终的传输过程，以实现一些自定义传输方式
        virtual std::function<void(void*, const Type&)> GetTransferFunc(const Type& type);
        virtual int GetTransferSize(const Type& type);

    private:
        std::unordered_map<Type, std::function<void(void*, const Type&)>> transferFuncs;
        std::unordered_map<Type, int> transferSizes;
    };
}
