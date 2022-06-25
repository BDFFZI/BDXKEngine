#pragma once
#include "Mesh.h"
#include "Shader.h"

class Graphics :GL, MeshEditor, ShaderEditor
{
public:
	/// <summary>
	/// Ĭ�������HLSL����������˳��ʽ�������ݣ������������������������
	/// ��ᵼ����ɫ���õ��ľ���ᱻת�ã����Ǿͳ�������������
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

