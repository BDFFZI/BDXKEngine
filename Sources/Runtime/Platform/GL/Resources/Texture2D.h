#pragma once
#include"Texture.h"
#include"Base/Color.h"

namespace BDXKEngine
{
    class GL2D;

    class Texture2D : public Texture
    {
        friend GL;
        friend GL2D;
    public:
        static ObjectPtr<Texture2D> Create(int width, int height);
        static ObjectPtr<Texture2D> Create(Color color);
    private:
        std::unique_ptr<Color[]> pixels;
        CComPtr<ID3D11RenderTargetView> renderTextureRTV = nullptr;

        void Transfer(Transferrer& transferrer) override;
        void Awake() override;
    };
}
