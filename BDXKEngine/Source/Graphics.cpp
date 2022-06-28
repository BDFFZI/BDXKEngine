#include "Graphics.h"
#include "Window.h"

CComPtr<ID3D11Buffer> Graphics::renderingMatrixBuffer = 0;
RenderingMatrix Graphics::renderingMatrix = {};


/// <summary>
/// Ĭ�������HLSL����������˳��ʽ�������ݣ������������������������
/// ��ᵼ����ɫ���õ��ľ���ᱻת�ã����Ǿͳ�������������
/// �������������ͨ����ɫ��������ĿǰBDXKEngine.hlsli�ڵľ����Ѿ�������ȷ�����о���
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

Graphics Graphics::Initialize(HWND window, std::function<void(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)>* windowEvent)
{
	//��ʼ��Direct3D
	GL::Initialize(window);
	GL::CreateBuffer(&renderingMatrix, sizeof(renderingMatrix), D3D11_BIND_CONSTANT_BUFFER, &renderingMatrixBuffer);
	GL::SetVertexConstantBuffer(&renderingMatrixBuffer.p);

	//����Direct3D����Ⱦ�����ʼ��Direct2D���Ա�ʵ�����ߵĻ�������
	CComPtr<IDXGISurface> dxgiSurface;
	assert(SUCCEEDED(GL::GetRenderTargetTexture()->QueryInterface(&dxgiSurface)));
	GL2D::Initialize(dxgiSurface);

	*windowEvent = OnWindowMessage;
	GL::Begin();
	GL2D::BeginDraw();
	return {};
}

void Graphics::OnWindowMessage(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_PAINT:
		//˳�����Ҫ��Direct2D����Ⱦ���������ҪDirect3D����ʾ
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
		Rect rect = { Vector2::zero ,Window::GetSize(window) };
		GL2D::ReleaseResources();//�ͷŶ�Direct3D��Ⱦ��������ã��Ӷ�ʹDirect3D�������´�������
		GL::Viewport(rect);

		CComPtr<IDXGISurface> dxgiSurface;
		assert(SUCCEEDED(GL::GetRenderTargetTexture()->QueryInterface(&dxgiSurface)));
		GL2D::CreateResources(dxgiSurface);//ʹ��Direct3D�´���������
		break;
	}
}
