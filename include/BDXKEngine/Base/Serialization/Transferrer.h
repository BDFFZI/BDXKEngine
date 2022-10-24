#pragma once
#include <string>
#include <vector>
#include "BDXKEngine/Base/Vector/Vector2.h"
#include "BDXKEngine/Base/Vector/Vector3.h"
#include "BDXKEngine/Base/Vector/Vector4.h"
#include "BDXKEngine/Base/Color.h"
#include "BDXKEngine/Base/Rect.h"

namespace BDXKEngine
{
    class ISerializable;
    class ObjectPtrBase;

    enum class TransferDirection
    {
        Input,
        Output,
        Inspect
    };

    class Transferrer
    {
    public:
#define TransferProperty(propertyName) auto (propertyName) = Get##propertyName();\
    transferrer.TransferField(#propertyName,propertyName);\
    Set##propertyName(propertyName);

        virtual TransferDirection GetTransferDirection() = 0;

        template <typename TSource>
        void TransferField(std::string key, TSource& value)
        {
            PushPath(key);
            TransferValue(value);
            PopPath(key);
        }
        template <typename TTransfer, typename TSource>
        void TransferFieldOf(std::string key, TSource& value)
        {
            PushPath(key);
            TransferValue(*reinterpret_cast<TTransfer*>(&value));
            PopPath(key);
        }
        void TransferField(std::string key, char* source, int size)
        {
            PushPath(key);
            TransferValue(source, size);
            PopPath(key);
        }

        virtual ~Transferrer() = default;
    protected:
        virtual void PushPath(const std::string& key) = 0;
        virtual void PopPath(std::string& key) =0;

        virtual void TransferValue(int& value) = 0;
        virtual void TransferValue(float& value) = 0;
        virtual void TransferValue(bool& value) = 0;
        virtual void TransferValue(Vector2& value) = 0;
        virtual void TransferValue(Vector3& value) = 0;
        virtual void TransferValue(Vector4& value) = 0;
        virtual void TransferValue(Color& value) = 0;
        virtual void TransferValue(Rect& value) = 0;
        virtual void TransferValue(std::string& value) = 0;
        virtual void TransferValue(ObjectPtrBase& value) = 0;
        virtual void TransferValue(ISerializable& value) = 0;
        virtual void TransferValue(std::vector<ObjectPtrBase>& vector) = 0;
        //virtual void TransferVector(std::string key, std::vector<ISerializable>& vector) = 0;
        virtual void TransferValue(char* source, int size) = 0;
    };
}
