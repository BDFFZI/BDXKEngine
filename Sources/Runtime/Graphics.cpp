#include "Graphics.h"
#include <algorithm>
#include "Window.h"

namespace BDXKEngine {
	WorldInfo Graphics::worldInfo = {};
	LightInfo Graphics::lightInfo = {};

	CComPtr<ID3D11Buffer> Graphics::worldInfoBuffer = nullptr;
	CComPtr<ID3D11Buffer> Graphics::lightInfoBuffer = nullptr;

	/// <summary>
	/// 默认情况下HLSL以列向量的顺序方式接受数据，而矩阵传输是以行向量传入的
	/// 这会导致着色器得到的矩阵会被转置，于是就成了行向量矩阵
	/// 但这种情况可以通过着色器调整，目前BDXKEngine.hlsli内的矩阵已经可以正确接受行矩阵
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

	void Graphics::Flush()
	{
		GL::End();
	}

	Graphics* Graphics::Initialize(Window* window, GL** gl, GL2D** gl2d)
	{
		//初始化Direct3D
		*gl = GL::Initialize(window->GetHwnd());
		//初始化Direct2D，利用Direct3D的渲染纹理以便实现两者的互操作性
		*gl2d = GL2D::Initialize(GetIDXGISurface());

		//创建渲染缓冲区
		GL::CreateBuffer(&worldInfo, sizeof(WorldInfo), D3D11_BIND_CONSTANT_BUFFER, &worldInfoBuffer);
		GL::SetConstantBuffer(0, &worldInfoBuffer.p);
		//创建光照信息缓冲区
		GL::CreateBuffer(&lightInfo, sizeof(LightInfo), D3D11_BIND_CONSTANT_BUFFER, &lightInfoBuffer);
		GL::SetConstantBuffer(1, &lightInfoBuffer.p);

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

			//获取渲染事件
			std::vector<RenderObjectEvent*>& renderObjectEvents = RenderEventEditor::GetRenderObjectEvents();
			std::vector<DrawGizmosEvent*>& drawGizmosEvents = RenderEventEditor::GetDrawGizmosEvents();

			//绘制普通物体
			std::for_each(
				renderObjectEvents.begin(),
				renderObjectEvents.end(),
				[](RenderObjectEvent* eventP) {
					RenderObject(eventP);
				}
			);

			//绘制UI，后期物体等
			GL2D::BeginDraw();
			std::for_each(
				drawGizmosEvents.begin(),
				drawGizmosEvents.end(),
				[](DrawGizmosEvent* eventP) {
					DrawGizmos(eventP);
				}
			);
			GL2D::EndDraw();//顺序很重要，Direct2D的渲染结果最终需要Direct3D来显示

			GL::End();
			break;
		}
		case WM_SIZE:
		{
			GL2D::ReleaseResources();//释放对Direct3D渲染纹理的引用，从而使Direct3D可以重新创建纹理

//重新调整纹理大小
			Rect rect = { Vector2::zero ,window->GetSize() };
			GL::Viewport(rect);

			GL2D::CreateResources(GetIDXGISurface());//使用Direct3D新创建的纹理
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