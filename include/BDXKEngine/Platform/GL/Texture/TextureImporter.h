#pragma once
#include "Texture2D.h"

namespace BDXKEngine
{
class TextureImporter
{
public:
    static ObjectPtr<Texture2D> Import(const std::string& fileName);
};
}
