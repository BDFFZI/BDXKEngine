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
	static void UpdateMatrix(RenderingMatrix matrix);
	static void RenderMesh(Mesh* mesh, Shader* shader);
	static void Clear(Color color = Color::clear);

protected:
	static void Initialize(HWND window);
private:
	static CComPtr<ID3D11Buffer> renderingMatrixBuffer;
	static RenderingMatrix renderingMatrix;
};

