#include "Graphics.h"
#include "Window.h"

CComPtr<ID3D11Buffer> Graphics::renderingMatrixBuffer = 0;
RenderingMatrix Graphics::renderingMatrix = {};


/// <summary>
/// 默认情况下HLSL以列向量的顺序方式接受数据，而矩阵传输是以行向量传入的
/// 这会导致着色器得到的矩阵会被转置，于是就成了行向量矩阵
/// 但这种情况可以通过着色器调整，目前BDXKEngine.hlsli内的矩阵已经可以正确接受行矩阵
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

Graphics* Graphics::Initialize(Window* window, GL** gl, GL2D** gl2d)
{
	//初始化Direct3D
	*gl = GL::Initialize(window->GetHwnd());
	GL::CreateBuffer(&renderingMatrix, sizeof(renderingMatrix), D3D11_BIND_CONSTANT_BUFFER, &renderingMatrixBuffer);
	GL::SetVertexConstantBuffer(&renderingMatrixBuffer.p);

	//利用Direct3D的渲染纹理初始化Direct2D，以便实现两者的互操作性
	CComPtr<IDXGISurface> dxgiSurface;
	assert(SUCCEEDED(GL::GetRenderTargetTexture()->QueryInterface(&dxgiSurface)));
	*gl2d = GL2D::Initialize(dxgiSurface);

	window->AddMessageListener(OnWindowMessage);
	GL::Begin();
	GL2D::BeginDraw();
	return new Graphics();
}

void Graphics::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_PAINT:
		//顺序很重要，Direct2D的渲染结果最终需要Direct3D来显示
		GL2D::EndDraw();
		GL::End();
		GL::Begin();
		GL2D::BeginDraw();
		break;
	case WM_DESTROY:
		GL2D::EndDraw();
		GL::End();
		break;
	case WM_SIZE:
		Rect rect = { Vector2::zero ,window->GetSize() };
		GL2D::ReleaseResources();//释放对Direct3D渲染纹理的引用，从而使Direct3D可以重新创建纹理
		GL::Viewport(rect);

		CComPtr<IDXGISurface> dxgiSurface;
		assert(SUCCEEDED(GL::GetRenderTargetTexture()->QueryInterface(&dxgiSurface)));
		GL2D::CreateResources(dxgiSurface);//使用Direct3D新创建的纹理
		break;
	}
}
