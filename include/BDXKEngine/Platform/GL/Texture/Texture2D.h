#pragma once
#include"Texture.h"

namespace BDXKEngine
{
    class Texture2D : public Texture
    {
    public:
        static ObjectPtr<Texture2D> Create(int width, int height);
        static ObjectPtr<Texture2D> Create(Color color);

        void SetRenderTarget() const;
    private:
        std::unique_ptr<Color[]> pixels;
        CComPtr<ID3D11RenderTargetView> renderTextureRTV = nullptr;

        void Awake() override;
    };
}
