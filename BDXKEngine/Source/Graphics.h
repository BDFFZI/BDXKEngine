#pragma once
#include <functional>
#include "GL.h"
#include "GL2D.h"
#include "Mesh.h"
#include "Shader.h"

class Graphics :GL, GL2D, MeshEditor, ShaderEditor
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
	static Graphics Initialize(HWND window, std::function<void(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)>* windowEvent);
private:
	Graphics() {};

	static CComPtr<ID3D11Buffer> renderingMatrixBuffer;
	static RenderingMatrix renderingMatrix;

	static void OnWindowMessage(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
};

