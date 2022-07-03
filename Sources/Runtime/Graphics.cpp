#include "Graphics.h"
#include "Window.h"

WorldInfo Graphics::worldInfo = {};
LightInfo Graphics::lightInfo = {};

CComPtr<ID3D11Buffer> Graphics::worldInfoBuffer = nullptr;
CComPtr<ID3D11Buffer> Graphics::lightInfoBuffer = nullptr;

/// <summary>
/// Ĭ�������HLSL����������˳��ʽ�������ݣ������������������������
/// ��ᵼ����ɫ���õ��ľ���ᱻת�ã����Ǿͳ�������������
/// �������������ͨ����ɫ��������ĿǰBDXKEngine.hlsli�ڵľ����Ѿ�������ȷ�����о���
/// </summary>
/// <param name="matrix"></param>
void Graphics::UpdateWorldInfo(WorldInfo worldInfo) {
	Graphics::worldInfo = worldInfo;
	GL::UpdateBuffer(worldInfoBuffer, &worldInfo);
}
void Graphics::UpdateLightInfo(LightInfo lightInfo)
{
	Graphics::lightInfo = lightInfo;
	GL::UpdateBuffer(lightInfoBuffer, &lightInfo);
}

void Graphics::DrawMeshNow(Mesh* mesh)
{
	GL::SetBuffer(GetVertexBuffer(mesh), sizeof(Vertex), GetTriangleBuffer(mesh), DXGI_FORMAT_R32_UINT);
	GL::Render(mesh->GetTrianglesCount());
}

void Graphics::SetRenderTarget(Texture2D* texture) {
	GL2D::ReleaseResources();

	GL::SetRenderTexture(texture == nullptr ? nullptr : GetGLTexture2D(texture));

	CComPtr<IDXGISurface> dxgiSurface;
	assert(SUCCEEDED(GL::GetRenderTargetTexture()->QueryInterface(&dxgiSurface)));
	GL2D::CreateResources(dxgiSurface);
}

void Graphics::Flush()
{
	GL::End();
}

Graphics* Graphics::Initialize(Window* window, GL** gl, GL2D** gl2d)
{
	//��ʼ��Direct3D
	*gl = GL::Initialize(window->GetHwnd());

	//����Direct3D����Ⱦ�����ʼ��Direct2D���Ա�ʵ�����ߵĻ�������
	CComPtr<IDXGISurface> dxgiSurface;
	assert(SUCCEEDED(GL::GetRenderTargetTexture()->QueryInterface(&dxgiSurface)));
	*gl2d = GL2D::Initialize(dxgiSurface);

	//������Ⱦ������
	GL::CreateBuffer(&worldInfo, sizeof(WorldInfo), D3D11_BIND_CONSTANT_BUFFER, &worldInfoBuffer);
	GL::SetConstantBuffer(0, &worldInfoBuffer.p);
	//����������Ϣ������
	GL::CreateBuffer(&lightInfo, sizeof(LightInfo), D3D11_BIND_CONSTANT_BUFFER, &lightInfoBuffer);
	GL::SetConstantBuffer(1, &lightInfoBuffer.p);

	window->AddMessageListener(OnWindowMessage);
	GL::Begin(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GL2D::BeginDraw();
	return new Graphics();
}

void Graphics::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_PAINT:
		//˳�����Ҫ��Direct2D����Ⱦ���������ҪDirect3D����ʾ
		GL2D::EndDraw();
		GL::End();
		GL::Begin(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GL2D::BeginDraw();
		break;
	case WM_DESTROY:
		GL2D::EndDraw();
		GL::End();
		break;
	case WM_SIZE:
		GL2D::ReleaseResources();//�ͷŶ�Direct3D��Ⱦ��������ã��Ӷ�ʹDirect3D�������´�������

		Rect rect = { Vector2::zero ,window->GetSize() };
		GL::Viewport(rect);

		CComPtr<IDXGISurface> dxgiSurface;
		assert(SUCCEEDED(GL::GetRenderTargetTexture()->QueryInterface(&dxgiSurface)));
		GL2D::CreateResources(dxgiSurface);//ʹ��Direct3D�´���������
		break;
	}
}
