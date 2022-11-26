#pragma once
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector3.h"

namespace BDXKEngine
{
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector2 uv;
        Color color;
    };

    constexpr D3D11_INPUT_ELEMENT_DESC VertexDescription[]{
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
}
