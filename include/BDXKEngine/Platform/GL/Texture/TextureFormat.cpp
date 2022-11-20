#include "TextureFormat.h"

#include <exception>

namespace BDXKEngine
{
    int GetPixelSize(TextureFormat textureFormat)
    {
        switch (textureFormat)
        {
        case TextureFormat::None: return 0;
        case TextureFormat::R32G32B32A32_FLOAT: return 16;
        case TextureFormat::B8G8R8A8_UNORM: return 4;
        }

        throw std::exception("无法识别的格式");
    }
}
