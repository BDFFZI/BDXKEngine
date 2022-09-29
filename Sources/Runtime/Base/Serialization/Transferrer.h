#pragma once
#include <string>
#include <utility>
#include "Base/Vector/Vector2.h"
#include "Base/Vector/Vector3.h"
#include "Base/Vector/Vector4.h"
#include "Base/Color.h"
#include "Base/Rect.h"

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
        virtual void TransferInt(std::wstring key, int& value) = 0;
        virtual void TransferFloat(std::wstring key, float& value) = 0;
        virtual void TransferBool(std::wstring key, bool& value) = 0;
        virtual void TransferVector2(std::wstring key, Vector2& value) = 0;
        virtual void TransferVector3(std::wstring key, Vector3& value) = 0;
        virtual void TransferVector4(std::wstring key, Vector4& value) = 0;
        virtual void TransferColor(std::wstring key, Color& value) = 0;
        virtual void TransferRect(std::wstring key, Rect& value) = 0;
        virtual void TransferString(std::wstring key, std::wstring& value) = 0;
        virtual void TransferObjectPtr(std::wstring key, ObjectPtrBase& value) = 0;
        virtual void TransferBytes(std::wstring key, void* source, int size) = 0;

        void TransferInt(std::wstring key, void* value)
        {
            TransferInt(std::move(key), *static_cast<int*>(value));
        }

        virtual ~Transferrer() = default;
    };
}
