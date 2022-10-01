#pragma once
#include <string>
#include <utility>
#include "BDXKEngine/Base/Vector/Vector2.h"
#include "BDXKEngine/Base/Vector/Vector3.h"
#include "BDXKEngine/Base/Vector/Vector4.h"
#include "BDXKEngine/Base/Color.h"
#include "BDXKEngine/Base/Rect.h"

namespace BDXKEngine
{
    class ObjectPtrBase;

    enum class TransferDirection
    {
        Input,
        Output,
    };

    class Transferrer
    {
    public:
        virtual TransferDirection GetTransferDirection() = 0;
        virtual void TransferInt(std::string key, int& value) = 0;
        virtual void TransferFloat(std::string key, float& value) = 0;
        virtual void TransferBool(std::string key, bool& value) = 0;
        virtual void TransferVector2(std::string key, Vector2& value) = 0;
        virtual void TransferVector3(std::string key, Vector3& value) = 0;
        virtual void TransferVector4(std::string key, Vector4& value) = 0;
        virtual void TransferColor(std::string key, Color& value) = 0;
        virtual void TransferRect(std::string key, Rect& value) = 0;
        virtual void TransferString(std::string key, std::string& value) = 0;
        virtual void TransferObjectPtr(std::string key, ObjectPtrBase& value) = 0;
        virtual void TransferBytes(std::string key, void* source, int size) = 0;

        void TransferInt(std::string key, void* value)
        {
            TransferInt(std::move(key), *static_cast<int*>(value));
        }

        virtual ~Transferrer() = default;
    };
}
