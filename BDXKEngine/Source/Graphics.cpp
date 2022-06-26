#include "Graphics.h"

CComPtr<ID3D11Buffer> Graphics::renderingMatrixBuffer = 0;
RenderingMatrix Graphics::renderingMatrix = {};


/// <summary>
/// 默认情况下HLSL以列向量的顺序方式接受数据，而矩阵传输是以行向量传入的
/// 这会导致着色器得到的矩阵会被转置，于是就成了行向量矩阵
/// </summary>
/// <param name="matrix"></param>
void Graphics::UpdateMatrix(RenderingMatrix matrix) {
	renderingMatrix = matrix;
	GL::UpdateBuffer(renderingMatrixBuffer, &renderingMatrix);
}

void Graphics::RenderMesh(Mesh* mesh, Shader* shader)
{
	GL::Render(
		MeshEditor::GetVertexBuffer(mesh), ShaderEditor::GetInputLayout(shader), sizeof(Vertex),
		MeshEditor::GetTriangleBuffer(mesh), DXGI_FORMAT_R32_UINT, mesh->GetTrianglesCount(),
		ShaderEditor::GetVertexShader(shader), ShaderEditor::GetPixelShader(shader), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
}

void Graphics::Clear(Color color)
{
	GL::Clear(color, 1, 0);
}

void Graphics::Initialize(HWND window)
{
	GL::Initialize(window);
	GL::CreateBuffer(&renderingMatrix, sizeof(renderingMatrix), D3D11_BIND_CONSTANT_BUFFER, &renderingMatrixBuffer);
	GL::SetVertexConstantBuffer(&renderingMatrixBuffer.p);
}
