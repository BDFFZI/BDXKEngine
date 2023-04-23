#pragma once
#include "Texture.h"

namespace BDXKEngine
{
    /// <summary>
    /// 顺序为x,-x,y,-y,z,-z
    /// </summary>
    class TextureCube : public Texture
    {
    public:
        static ObjectPtr<TextureCube> Create(int width, int height, TextureFormat textureFormat);

        void SetRenderTarget(int index) const;
    private:
        std::vector<CComPtr<ID3D11RenderTargetView>> renderTextureRTV = {};

        void Awake() override;
    };

    CustomReflection(TextureCube)
}
