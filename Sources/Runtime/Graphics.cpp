#include "Graphics.h"
#include <algorithm>
#include "Window.h"

namespace BDXKEngine {
	WorldInfo Graphics::worldInfo = {};
	CameraInfo Graphics::cameraInfo = {};
	LightInfo Graphics::lightInfo = {};
	ObjectInfo Graphics::objectInfo = {};

	CComPtr<ID3D11Buffer> Graphics::worldInfoBuffer = nullptr;
	CComPtr<ID3D11Buffer> Graphics::cameraInfoBuffer = nullptr;
	CComPtr<ID3D11Buffer> Graphics::lightInfoBuffer = nullptr;
	CComPtr<ID3D11Buffer> Graphics::objectInfoBuffer = nullptr;

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
	void Graphics::UpdateCameraInfo(CameraInfo cameraInfo)
	{
		Graphics::cameraInfo = cameraInfo;
		GL::UpdateBuffer(cameraInfoBuffer, &cameraInfo);
	}
	void Graphics::UpdateLightInfo(LightInfo lightInfo)
	{
		Graphics::lightInfo = lightInfo;
		GL::UpdateBuffer(lightInfoBuffer, &lightInfo);
	}

	void Graphics::UpdateObjectInfo(ObjectInfo objectInfo)
	{
		Graphics::objectInfo = objectInfo;
		GL::UpdateBuffer(objectInfoBuffer, &objectInfo);
	}

	void Graphics::DrawMeshNow(ObjectPtr<Mesh> mesh)
	{
		GL::SetBuffer(GetVertexBuffer(mesh), sizeof(Vertex), GetTriangleBuffer(mesh), DXGI_FORMAT_R32_UINT);
		GL::Render(mesh->GetTrianglesCount());
	}

	void Graphics::SetRenderTarget(ObjectPtr<Texture2D> texture) {
		GL2D::ReleaseResources();
		GL::SetRenderTexture(texture == nullptr ? nullptr : Texture2DEditor::GetGLTexture2D(texture));
		GL2D::CreateResources(GetIDXGISurface());
	}

	Graphics* Graphics::Initialize(Window* window, GL** gl, GL2D** gl2d)
	{
		//��ʼ��Direct3D
		*gl = GL::Initialize(window->GetHwnd());
		//��ʼ��Direct2D������Direct3D����Ⱦ�����Ա�ʵ�����ߵĻ�������
		*gl2d = GL2D::Initialize(GetIDXGISurface());

		//������Ⱦ�õ�ͨ�ó���������
		GL::CreateBuffer(&worldInfo, sizeof(WorldInfo), D3D11_BIND_CONSTANT_BUFFER, &worldInfoBuffer);
		GL::SetConstantBuffer(0, &worldInfoBuffer.p);
		GL::CreateBuffer(&cameraInfo, sizeof(CameraInfo), D3D11_BIND_CONSTANT_BUFFER, &cameraInfoBuffer);
		GL::SetConstantBuffer(1, &cameraInfoBuffer.p);
		GL::CreateBuffer(&lightInfo, sizeof(LightInfo), D3D11_BIND_CONSTANT_BUFFER, &lightInfoBuffer);
		GL::SetConstantBuffer(2, &lightInfoBuffer.p);
		GL::CreateBuffer(&objectInfo, sizeof(ObjectInfo), D3D11_BIND_CONSTANT_BUFFER, &objectInfoBuffer);
		GL::SetConstantBuffer(3, &objectInfoBuffer.p);

		window->AddMessageListener(OnWindowMessage);

		return new Graphics();
	}

	void Graphics::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
	{
		switch (messageSign)
		{
		case WM_PAINT:
		{
			GL::Begin(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//��ȡ��Ⱦ�¼�
			std::vector<RenderObjectEvent*>& renderObjectEvents = RenderEventEditor::GetRenderObjectEvents();
			std::vector<DrawGizmosEvent*>& drawGizmosEvents = RenderEventEditor::GetDrawGizmosEvents();

			//������ͨ����
			std::for_each(
				renderObjectEvents.begin(),
				renderObjectEvents.end(),
				[](RenderObjectEvent* eventP) {
					RenderObject(eventP);
				}
			);

			//����UI�����������
			GL2D::BeginDraw();
			std::for_each(
				drawGizmosEvents.begin(),
				drawGizmosEvents.end(),
				[](DrawGizmosEvent* eventP) {
					DrawGizmos(eventP);
				}
			);
			GL2D::EndDraw();//˳�����Ҫ��Direct2D����Ⱦ���������ҪDirect3D����ʾ

			GL::End();
			break;
		}
		case WM_SIZE:
		{
			GL2D::ReleaseResources();//�ͷŶ�Direct3D��Ⱦ��������ã��Ӷ�ʹDirect3D�������´�������

//���µ��������С
			Rect rect = { Vector2::zero ,window->GetSize() };
			GL::Viewport(rect);

			GL2D::CreateResources(GetIDXGISurface());//ʹ��Direct3D�´���������
			break;
		}
		}
	}

	CComPtr<IDXGISurface> Graphics::GetIDXGISurface()
	{
		CComPtr<IDXGISurface> dxgiSurface;
		HRESULT result = GL::GetRenderTargetTexture()->QueryInterface(&dxgiSurface);
		assert(SUCCEEDED(result));

		return dxgiSurface;
	}
}