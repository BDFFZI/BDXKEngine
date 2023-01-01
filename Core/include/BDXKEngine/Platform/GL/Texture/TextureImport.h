#pragma once
#include "Texture2D.h"

namespace BDXKEngine
{
class TextureImport
{
public:
    static ObjectPtr<Texture2D> Png(const std::string& fileName);
};
}
