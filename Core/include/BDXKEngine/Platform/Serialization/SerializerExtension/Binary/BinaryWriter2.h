#pragma once
#include "BDXKEngine/Base/Data/Graphics/Color.h"
#include "BDXKEngine/Base/Data/Graphics/Rect.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"
#include "BDXKEngine/Base/Data/Mathematics/Matrix/Matrix4x4.h"
#include "BDXKEngine/Base/Serializer/Binary/BinaryWriter.h"

namespace BDXKEngine
{
    class BinaryWriter2 : public BinaryWriter
    {
        CustomTransferFunc(Vector2, WriteStreamFrom)
        CustomTransferFunc(Vector3, WriteStreamFrom)
        CustomTransferFunc(Vector4, WriteStreamFrom)
        CustomTransferFunc(Matrix4x4, WriteStreamFrom)
        CustomTransferFunc(Color, WriteStreamFrom)
        CustomTransferFunc(Rect, WriteStreamFrom)
    };
}
