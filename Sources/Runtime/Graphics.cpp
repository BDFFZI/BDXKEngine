#include "Graphics.h"
#include <algorithm>
#include "Window.h"
using namespace BDXKEngine;

Vector3 ScreenToViewPos(Vector2 screenSize, Vector2 screenPos)
{
	screenPos.y = screenSize.y - screenPos.y;
	screenPos = screenPos - screenSize / 2;
	return { screenPos.x,screenPos.y,0 };
}

Window* Graphics::window = nullptr;
WorldInfo Graphics::worldInfo = {};
CameraInfo Graphics::cameraInfo = {};
LightInfo Graphics::lightInfo = {};
ObjectInfo Graphics::objectInfo = {};
ShadowInfo Graphics::shadowInfo = {};

CComPtr<ID3D11Buffer> Graphics::worldInfoBuffer = nullptr;
CComPtr<ID3D11Buffer> Graphics::cameraInfoBuffer = nullptr;
CComPtr<ID3D11Buffer> Graphics::lightInfoBuffer = nullptr;
CComPtr<ID3D11Buffer> Graphics::objectInfoBuffer = nullptr;
CComPtr<ID3D11Buffer> Graphics::shadowInfoBuffer = nullptr;
ObjectPtr<Mesh> Graphics::drawTextureMesh = nullptr;

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
void Graphics::UpdateCameraInfo(CameraInfo cameraInfo)
{
	Graphics::cameraInfo = cameraInfo;
	GL::UpdateBuffer(cameraInfoBuffer, &cameraInfo);
}
void Graphics::UpdateObjectInfo(ObjectInfo objectInfo)
{
	Graphics::objectInfo = objectInfo;
	GL::UpdateBuffer(objectInfoBuffer, &objectInfo);
}
void Graphics::UpdateLightInfo(LightInfo lightInfo, ShadowInfo shadowInfo, ObjectPtr<Texture> shadowMap)
{
	Graphics::lightInfo = lightInfo;
	GL::UpdateBuffer(lightInfoBuffer, &lightInfo);
	UpdateShadowInfo(shadowInfo);
	if (lightInfo.lightType == LightType::Point)
		GL::SetTexture(5, shadowMap);
	else
		GL::SetTexture(4, shadowMap);
}
void BDXKEngine::Graphics::UpdateShadowInfo(ShadowInfo shadowInfo)
{
	Graphics::shadowInfo = shadowInfo;
	GL::UpdateBuffer(shadowInfoBuffer, &shadowInfo);
}


void Graphics::DrawMeshNow(ObjectPtr<Mesh> mesh)
{
	GL::SetMesh(mesh);
	GL::Render(mesh->GetTrianglesCount());
}
void Graphics::DrawTexture(ObjectPtr<Texture2D> texture, Rect screenRect)
{
	ObjectPtr<Texture2D> renderTarget = GL::GetRenderTarget();
	Vector2 screenSize;
	if (renderTarget != nullptr)
		screenSize = { (float)renderTarget->GetWidth(), (float)renderTarget->GetHeight() };
	else
		screenSize = window->GetSize();
	Vector3 minPosition = ScreenToViewPos(screenSize, screenRect.GetMin());
	Vector3 maxPosition = ScreenToViewPos(screenSize, screenRect.GetMax());

	drawTextureMesh->SetPositions({
		minPosition,
		{maxPosition.x,minPosition.y,0},
		maxPosition,
		{minPosition.x,maxPosition.y,0}
		});
	drawTextureMesh->UploadMeshData();

	UpdateObjectInfo({ Matrix4x4::identity });
	UpdateCameraInfo(CameraInfo::Orthographic(
		Vector3::zero,
		Matrix4x4::identity,
		screenSize.x / screenSize.y,
		0, 1, screenSize.y
	));
	GL::SetTexture(0, texture.As<Texture>());

	DrawMeshNow(drawTextureMesh);
}
void Graphics::Blit(ObjectPtr<Texture2D> source, ObjectPtr<Texture2D> dest, ObjectPtr<Material> blitMaterial)
{
	GL::SetRenderTarget(dest);
	blitMaterial->SetPass(0);
	DrawTexture(source, { 0,0,(float)source->GetWidth(),(float)source->GetHeight() });
	GL::SetRenderTarget(nullptr);
}

Graphics* Graphics::Initialize(Window* window)
{
	//创建渲染用的通用常量缓冲区
	GL::CreateBuffer(&worldInfo, sizeof(WorldInfo), D3D11_BIND_CONSTANT_BUFFER, &worldInfoBuffer);
	GL::SetConstantBuffer(1, &worldInfoBuffer.p);
	GL::CreateBuffer(&cameraInfo, sizeof(CameraInfo), D3D11_BIND_CONSTANT_BUFFER, &cameraInfoBuffer);
	GL::SetConstantBuffer(2, &cameraInfoBuffer.p);
	GL::CreateBuffer(&objectInfo, sizeof(ObjectInfo), D3D11_BIND_CONSTANT_BUFFER, &objectInfoBuffer);
	GL::SetConstantBuffer(3, &objectInfoBuffer.p);
	GL::CreateBuffer(&lightInfo, sizeof(LightInfo), D3D11_BIND_CONSTANT_BUFFER, &lightInfoBuffer);
	GL::SetConstantBuffer(4, &lightInfoBuffer.p);
	GL::CreateBuffer(&shadowInfo, sizeof(ShadowInfo), D3D11_BIND_CONSTANT_BUFFER, &shadowInfoBuffer);
	GL::SetConstantBuffer(5, &shadowInfoBuffer.p);

	Graphics::window = window;
	window->AddMessageListener(OnWindowMessage);

	drawTextureMesh = new Mesh();
	drawTextureMesh->SetTriangles({
		0,1,3,
		3,1,2
		});
	drawTextureMesh->SetUVs({
		{0,1},
		{1,1},
		{1,0},
		{0,0},
		});

	return new Graphics{};
}

void Graphics::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_PAINT:
	{
		//获取渲染事件
		std::vector<RenderObjectEvent*>& renderObjectEvents = RenderObjectEvent::renderObjectEvents;
		std::vector<DrawGizmosEvent*>& drawGizmosEvents = DrawGizmosEvent::drawGizmosEvents;

		GL::SetRenderTarget(nullptr);

		//绘制普通物体
		std::for_each(
			renderObjectEvents.begin(),
			renderObjectEvents.end(),
			[](RenderObjectEvent* eventP) {
				eventP->OnRenderObject();
			}
		);

		//绘制UI，后期物体等
		GL2D::BeginDraw();
		std::for_each(
			drawGizmosEvents.begin(),
			drawGizmosEvents.end(),
			[](DrawGizmosEvent* eventP) {
				eventP->OnDrawGizmos();
			}
		);
		GL2D::EndDraw();//顺序很重要，Direct2D的渲染结果最终需要Direct3D来显示

		GL::Present();//显示到屏幕上

		break;
	}
	case WM_SIZE:
	{
		GL2D::ReleaseResources();//释放对Direct3D渲染纹理的引用，从而使Direct3D可以重新创建纹理

		//重新调整纹理大小
		Rect rect = { Vector2::zero ,window->GetSize() };
		GL::ResizeDefaultRenderTarget(rect.GetSize());

		GL2D::CreateResources();//使用Direct3D新创建的纹理
		break;
	}
	case WM_DESTROY:
		return;
		drawTextureMesh = nullptr;
		break;
	}
}
