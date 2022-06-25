#pragma once
#include "Mesh.h"
#include "Shader.h"

class Graphics :GL, MeshEditor, ShaderEditor
{
public:
	/// <summary>
	/// 默认情况下HLSL以列向量的顺序方式接受数据，而矩阵传输是以行向量传入的
	/// 这会导致着色器得到的矩阵会被转置，于是就成了行向量矩阵
	/// </summary>
	/// <param name="matrix"></param>
	static void UpdateMatrix(RenderingMatrix matrix) {
		renderingMatrix = matrix;
		GL::UpdateBuffer(renderingMatrixBuffer, &renderingMatrix);
	}
	static void RenderMesh(Mesh* mesh, Shader* shader)
	{
		GL::Render(
			MeshEditor::GetVertexBuffer(mesh), ShaderEditor::GetInputLayout(shader), sizeof(Vertex),
			MeshEditor::GetTriangleBuffer(mesh), DXGI_FORMAT_R32_SINT, mesh->GetTrianglesCount(),
			ShaderEditor::GetVertexShader(shader), ShaderEditor::GetPixelShader(shader), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		);
	}
	static void Clear(Color color = Color::clear)
	{
		GL::Clear(color);
	}

protected:
	static void Initialize(HWND window)
	{
		GL::Initialize(window);
		GL::CreateBuffer(&renderingMatrix, sizeof(renderingMatrix), D3D11_BIND_CONSTANT_BUFFER, &renderingMatrixBuffer);
		GL::SetVertexConstantBuffer(&renderingMatrixBuffer.p);
	}
private:
	static CComPtr<ID3D11Buffer> renderingMatrixBuffer;
	static RenderingMatrix renderingMatrix;
};

