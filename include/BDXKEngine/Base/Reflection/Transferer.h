#pragma once
#include <functional>
#include <unordered_map>
#include "Type.h"

namespace BDXKEngine
{
    class Transferer
    {
    public:
        bool GetIsReadOnly() const;

        template <typename TValue>
        void SetTransferFunc(std::function<void(TValue& value)> func)
        {
            transferFuncs[GetTypeID<TValue>()] = [=](void* value, const Type& type)
            {
                TValue* target = static_cast<TValue*>(value);
                func(*target);
            };
        }

        template <typename TTransfer, typename TField>
        void TransferFieldOf(std::string key, TField& value)
        {
            PushPath(key);

            TransferValueOf<TTransfer>(value);

            PopPath(key);
        }
        template <typename TField>
        void TransferField(std::string key, TField& value)
        {
            PushPath(key);

            TransferValue(value);

            PopPath(key);
        }

#define TransferFieldInfo(fieldName) transferer.TransferField(#fieldName, fieldName)
#define TransferFieldInfoOf(fieldName,transferType) transferer.TransferFieldOf<transferType>(#fieldName, fieldName)
        //下方这些宏和反射功能不兼容，不建议使用，请尝试使用IsInput()来实现类似功能
#define TransferProperty(propertyName) auto (propertyName) = Get##propertyName();\
                transferer.TransferField(#propertyName,propertyName);\
                Set##propertyName(propertyName)
#define TransferPropertyReadOnly(propertyName) auto (propertyName) = Get##propertyName();\
transferer.TransferField(#propertyName,propertyName)

        virtual ~Transferer() = default;
    protected:
        Transferer(bool isReadOnly = true);

        template <typename TTransfer, typename TSource>
        void TransferValueOf(TSource& value)
        {
            auto typeId = GetTypeID<TTransfer>();
            auto transferFunc = transferFuncs[typeId];

            if (transferFunc != nullptr)transferFunc(&value, typeId);
            else TransferValue(&value, typeId);
        }
        template <typename TSource>
        void TransferValue(TSource& value)
        {
            TransferValueOf<TSource>(value);
        }

        virtual void PushPath(const std::string& key);
        virtual void PopPath(std::string& key);
        virtual void TransferValue(void* value, const Type& type);
    private:
        std::unordered_map<Type, std::function<void(void*, const Type&)>> transferFuncs;
        bool isReadOnly;
    };

    class TransferFunc
    {
    public:
        template <typename TTransferer, typename TValue>
        TransferFunc(TTransferer* transferer, std::function<void(TValue&)> func)
        {
            transferer->template SetTransferFunc<TValue>(func);
        }
    };

#define CustomTransferFunc(Type,FuncName,ID) TransferFunc CustomTransferFunc##FuncName##ID = {this,std::function([this](Type##& value){FuncName(value);})};
}
