#pragma once
#include "BDXKEngine/Base/Data/Graphics/Color.h"
#include "BDXKEngine/Base/Data/Graphics/Rect.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"
#include "BDXKEngine/Base/Serialization/Binary/BinaryImporter.h"

namespace BDXKEngine
{
    class BDXKBinaryImporter : public BinaryImporter
    {
        CustomTransferFunc(Vector2, ReadStreamTo)
        CustomTransferFunc(Vector3, ReadStreamTo)
        CustomTransferFunc(Vector4, ReadStreamTo)
        CustomTransferFunc(Color, ReadStreamTo)
        CustomTransferFunc(Rect, ReadStreamTo)
    };
}
