#pragma once
#include"Texture.h"
#include "TextureFormat.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"

namespace BDXKEngine
{
    class Texture2D : public Texture
    {
    public:
        static ObjectPtr<Texture2D> Create(Color color);
        static ObjectPtr<Texture2D> Create(int width, int height, TextureFormat textureFormat);
        static ObjectPtr<Texture2D> Create(int widthf, int heightf, TextureFormat textureFormat, const char* pixels);

        void SetRenderTarget() const;
    private:
        std::vector<char> pixels;
        CComPtr<ID3D11RenderTargetView> renderTextureRTV = nullptr;

        void Transfer(Transferer& transferer) override;
        void Awake() override;
    };

    CustomReflection(Texture2D);
}
