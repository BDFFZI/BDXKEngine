#pragma once
#include <string>
#include <vector>
#include <any>
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector2.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector3.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"
#include "BDXKEngine/Base/Color.h"
#include "BDXKEngine/Base/Rect.h"


namespace BDXKEngine
{
    class ObjectPtrBase;
    class Serialization;

    enum class TransferDirection
    {
        None,
        Input,
        Output,
    };

    class Transferer
    {
#define TransferProperty(propertyName) auto (propertyName) = Get##propertyName();\
transferer.TransferField(#propertyName,propertyName);\
Set##propertyName(propertyName)
#define TransferPropertyReadOnly(propertyName) auto (propertyName) = Get##propertyName();\
transferer.TransferField(#propertyName,propertyName)

    public:
        virtual TransferDirection GetTransferDirection() = 0;
        virtual void PushPath(const std::string& key) = 0;
        virtual void PopPath(std::string& key) =0;

        template <typename TTransfer, typename TSource>
        void TransferFieldOf(std::string key, TSource& value)
        {
            PushPath(key);
            TransferValue(*reinterpret_cast<TTransfer*>(&value));
            PopPath(key);
        }
        template <typename TSource>
        void TransferField(std::string key, TSource& value)
        {
            PushPath(key);
            TransferValue(value);
            PopPath(key);
        }
        void TransferField(std::string key, char* source, int size)
        {
            PushPath(key);
            TransferValue(source, size);
            PopPath(key);
        }


        virtual ~Transferer() = default;
    protected:
        virtual void TransferValue(char* source, int size) = 0;
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
        virtual void TransferValue(Serialization& value) = 0;
        virtual bool TransferValue(std::vector<std::any>& value)
        {
            return false;
        }
        template <typename TSource>
        void TransferValue(std::vector<TSource>& vector)
        {
            std::vector<std::any> vectorAny{};

            const int originalSize = static_cast<int>(vector.size());
            vectorAny.resize(originalSize);
            for (auto index = originalSize - 1; index >= 0; --index)
                vectorAny[index] = vector[index];

            if (TransferValue(vectorAny))
            {
                const int currentSize = static_cast<int>(vectorAny.size());
                vector.resize(currentSize);
                for (auto index = currentSize - 1; index >= 0; --index)
                    vector[index] = *vectorAny[index]._Cast<TSource>();
            }
            else
            {
                //默认处理方式
                int count = static_cast<int>(vector.size());
                TransferField("size", count);
                vector.resize(count);
                for (int i = 0; i < count; i++)
                    TransferField("item" + std::to_string(i), vector[i]);
            }
        }
    };
}
