#pragma once
#include "BDXKEngine/Base/Data/Graphics/Color.h"
#include "BDXKEngine/Base/Data/Graphics/Rect.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"
#include "BDXKEngine/Base/Data/Mathematics/Matrix/Matrix4x4.h"
#include "BDXKEngine/Base/Serializer/Binary/BinaryImporter.h"

namespace BDXKEngine
{
    class BinaryImporter2 : public BinaryImporter
    {
        CustomTransferFunc(Vector2, ReadStreamTo)
        CustomTransferFunc(Vector3, ReadStreamTo)
        CustomTransferFunc(Vector4, ReadStreamTo)
        CustomTransferFunc(Matrix4x4, ReadStreamTo)
        CustomTransferFunc(Color, ReadStreamTo)
        CustomTransferFunc(Rect, ReadStreamTo)
    };
}
