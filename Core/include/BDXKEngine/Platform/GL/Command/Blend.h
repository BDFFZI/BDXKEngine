#pragma once
#include "BDXKEngine/Platform/GL/Core/GL.h"
#include "BDXKEngine/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    struct Blend
    {
        enum class Factor
        {
            Zero = D3D11_BLEND_ZERO,
            One = D3D11_BLEND_ONE,
            SrcColor = D3D11_BLEND_SRC_COLOR,
            SrcAlpha = D3D11_BLEND_SRC_ALPHA,
            DstColor = D3D11_BLEND_DEST_COLOR,
            DstAlpha = D3D11_BLEND_DEST_ALPHA,
            OneMinusSrcColor = D3D11_BLEND_INV_SRC_COLOR,
            OneMinusSrcAlpha = D3D11_BLEND_INV_SRC_ALPHA,
            OneMinusDstColor = D3D11_BLEND_INV_DEST_COLOR,
            OneMinusDstAlpha = D3D11_BLEND_INV_DEST_ALPHA,
        };

        enum class Operation
        {
            Add = D3D11_BLEND_OP_ADD,
        };

        static Blend Opaque;
        static Blend Multiply;
        static Blend Additive;

        bool state = false;
        Factor sourceFactor = Factor::Zero;
        Factor destinationFactor = Factor::Zero;
        Operation operation = Operation::Add;

        Blend();
        Blend(bool state, Factor sourceFactor, Factor destinationFactor, Operation operation);

        void UpdateGL();
        void UploadRP() const;
        void Transfer(Transferer& transferer);
    private:
        CComPtr<ID3D11BlendState> blendState;
    };
}
