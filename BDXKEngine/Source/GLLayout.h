#pragma once
#include "Vector3.h"
#include "Color.h"
#include "Matrix4x4.h"
#include "GL.h"

struct Vertex {
	Vector3 position;
	Vector3 normal;
	Color color;
};

const D3D11_INPUT_ELEMENT_DESC VertexDescription[]{
   { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
   { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
   { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

struct RenderingMatrix {
	Matrix4x4 localToWorld;
	Matrix4x4 worldToCamera;
	Matrix4x4 cameraToView;
};
static_assert((sizeof(RenderingMatrix) % 16) == 0, "常量缓冲区大小必须是16字节的倍数");