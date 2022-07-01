#pragma once
#include <functional>
#include "GL.h"
#include "GL2D.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

class Graphics :GL, GL2D, MeshEditor, ShaderEditor
{
public:
	/// <summary>
	/// 默认情况下HLSL以列向量的顺序方式接受数据，而矩阵传输是以行向量传入的
	/// 这会导致着色器得到的矩阵会被转置，于是就成了行向量矩阵
	/// </summary>
	/// <param name="matrix"></param>
	static void UpdateMatrix(RenderingMatrix matrix);
	static void RenderMesh(Mesh* mesh, Shader* shader);
	static void Clear(Color color = Color::clear);
protected:
	static Graphics* Initialize(Window* window, GL** gl, GL2D** gl2d);
private:

	static CComPtr<ID3D11Buffer> renderingMatrixBuffer;
	static RenderingMatrix renderingMatrix;

	static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
};


