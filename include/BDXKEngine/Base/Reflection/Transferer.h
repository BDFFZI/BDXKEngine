#pragma once
#include <functional>
#include <unordered_map>
#include "Type.h"

namespace BDXKEngine
{
    class Transferer;

    class Transferable
    {
    public:
        virtual ~Transferable() = default;
        virtual void Transfer(Transferer& transferer) = 0;
    };

    class Transferer
    {
    public:
        virtual ~Transferer() = default;
        virtual void Reset(std::string& data);

        template <typename TValue>
        void SetTransferFunc(std::function<void(TValue& value)> func)
        {
            const Type type = GetTypeID<TValue>();
            transferFuncs[type] = [=](void* value, const Type&)
            {
                TValue* target = static_cast<TValue*>(value);
                func(*target);
            };
            typeSizes[type] = sizeof TValue;
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
        
        void TransferNested(std::string key, Transferable& value);


#define TransferFieldInfo(fieldName) transferer.TransferField(#fieldName, fieldName)
#define TransferFieldInfoOf(fieldName,transferType) transferer.TransferFieldOf<transferType>(#fieldName, fieldName)
        //下方这些宏和反射功能不兼容，不建议使用，请尝试使用IsInput()来实现类似功能
#define TransferProperty(propertyName) auto (propertyName) = Get##propertyName();\
                transferer.TransferField(#propertyName,propertyName);\
                Set##propertyName(propertyName)
#define TransferPropertyReadOnly(propertyName) auto (propertyName) = Get##propertyName();\
transferer.TransferField(#propertyName,propertyName)

        template <typename TTransfer, typename TSource>
        void TransferValueOf(TSource& value)
        {
            TransferValue(&value, GetTypeID<TTransfer>());
        }
        template <typename TSource>
        void TransferValue(TSource& value)
        {
            TransferValueOf<TSource>(value);
        }

        virtual void PushPath(const std::string& key);
        virtual void PopPath(std::string& key);
        virtual void TransferValue(void* value, const Type& type);
    protected:
        virtual void TransferValueFallback(void* value, const Type& type);
    private:
        std::unordered_map<Type, std::function<void(void*, const Type&)>> transferFuncs;
        std::unordered_map<Type, int> typeSizes;
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

#define CustomTransferFuncInternal(Type,FuncName,ID) TransferFunc CustomTransferFunc##FuncName##ID = {this,std::function([this](Type##& value){FuncName(value);})};
#define CustomTransferFuncInternal2(Type,FuncName,ID) CustomTransferFuncInternal(Type,FuncName,ID)
#define CustomTransferFunc(Type,FuncName) CustomTransferFuncInternal2(Type,FuncName,__LINE__)
}
