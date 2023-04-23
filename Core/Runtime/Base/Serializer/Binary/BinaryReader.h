#pragma once
#include <sstream>
#include "Core/Runtime/Base/Serializer/IOTransferer.h"
#include <crossguid/guid.hpp>
#include "Core/Runtime/Base/Data/Graphics/Color.h"
#include "Core/Runtime/Base/Data/Graphics/Rect.h"
#include "Core/Runtime/Base/Data/Math/Matrix/Matrix4x4.h"

namespace BDXKEngine
{
    class BinaryReader : public IOTransferer
    {
    public:
        bool IsImporter() override;
        void Reset(std::string& data) override;

        void TransferInt(int& value);
        void TransferFloat(float& value);
        void TransferBool(bool& value);
        void TransferString(std::string& value);
        void TransferBytes(std::vector<char>& value);
        void TransferGuid(xg::Guid& guid);
        void TransferVector2(Vector2& value);
        void TransferVector3(Vector3& value);
        void TransferVector4(Vector4& value);
        void TransferMatrix4x4(Matrix4x4& value);
        void TransferColor(Color& value);
        void TransferRect(Rect& value);

        BinaryReader();

    protected:
        void ReadStreamTo(char* value, int size);
        template <typename TValue>
        void ReadStreamTo(TValue& value)
        {
            ReadStreamTo(reinterpret_cast<char*>(&value), sizeof(TValue));
        }

    private:
        std::stringstream stream;
    };
}
