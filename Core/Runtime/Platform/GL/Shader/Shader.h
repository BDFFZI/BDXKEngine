#pragma once
#include <filesystem>
#include "../Core/GL.h"
#include "Core/Runtime/Base/Object/Object.h"
#include "Core/Runtime/Base/Reflection/Reflection.h"

namespace BDXKEngine
{
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector4 tangent;
        Vector2 uv;
        Color color;
    };

    constexpr D3D11_INPUT_ELEMENT_DESC VertexDescription[]{
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    class Shader : public Object
    {
    public:
        static ObjectPtr<Shader> Create(const std::string& hlsl, bool instantiate = true);
        static ObjectPtr<Shader> CreateFromFile(const std::filesystem::path& path, bool instantiate = true);

        void UploadRP() const; // 设置当前渲染管线中用于的着色器
    private:
        std::vector<char> vertexPass;
        std::vector<char> pixelPass;

        CComPtr<ID3D11VertexShader> vertexShader;
        CComPtr<ID3D11PixelShader> pixelShader;
        CComPtr<ID3D11InputLayout> inputLayout;

        void Transfer(Transferer& transferer) override;
        void Awake() override;
    };

    CustomReflection(Shader)
}
