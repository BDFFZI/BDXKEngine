#include "TextureFormat.h"

#include <exception>

namespace BDXKEngine
{
    int GetPixelSize(const TextureFormat textureFormat)
    {
        switch (textureFormat)
        {
        case TextureFormat::None: return 0;
        case TextureFormat::R32G32B32A32_FLOAT:
            return 16;
        case TextureFormat::R8G8B8A8_UNORM:
        case TextureFormat::B8G8R8A8_UNORM:
            return 4;
        case TextureFormat::R8_UNORM:
            return 1;
        }

        throw std::exception("无法识别的格式");
    }
}
