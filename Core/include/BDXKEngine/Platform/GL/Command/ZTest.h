#pragma once
#include "BDXKEngine/Platform/GL/Core/GL.h"
#include "BDXKEngine/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    struct ZTest
    {
        enum class Operation
        {
            Less = D3D11_COMPARISON_LESS,
            LEqual = D3D11_COMPARISON_LESS_EQUAL,
            Equal = D3D11_COMPARISON_EQUAL,
            GEqual = D3D11_COMPARISON_GREATER_EQUAL,
            Greater = D3D11_COMPARISON_GREATER,
            NotEqual = D3D11_COMPARISON_NOT_EQUAL,
            Always = D3D11_COMPARISON_ALWAYS,
        };

        static ZTest Default;
        static ZTest Background;

        Operation operation = Operation::LEqual;
        float factor = 0; //缩放深度
        float units = 0; //偏移深度
        bool write = true;
        bool clip = true;

        ZTest();
        ZTest(Operation operation, float factor, float units, bool write, bool clip);

        void UpdateGL();
        void UploadRP() const;
        void Transfer(Transferer& transferer);
    private:
        CComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;
    };
}
