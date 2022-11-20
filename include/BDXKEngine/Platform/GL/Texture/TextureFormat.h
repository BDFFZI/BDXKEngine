#pragma once
#include <dxgiformat.h>

namespace BDXKEngine
{
    enum class TextureFormat
    {
        None = DXGI_FORMAT_UNKNOWN,
        R32G32B32A32_FLOAT = DXGI_FORMAT_R32G32B32A32_FLOAT,
        B8G8R8A8_UNORM = DXGI_FORMAT_B8G8R8A8_UNORM,
    };

    int GetPixelSize(TextureFormat textureFormat);
}
