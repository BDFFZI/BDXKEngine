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

WorldInfo Graphics::worldInfo = {};
CameraInfo Graphics::cameraInfo = {};
LightInfo Graphics::lightInfo = {};
ObjectInfo Graphics::objectInfo = {};
ShadowInfo Graphics::shadowInfo = {};
ObjectPtr<Material> Graphics::blitMaterial = nullptr;

CComPtr<ID3D11Buffer> Graphics::worldInfoBuffer = nullptr;
CComPtr<ID3D11Buffer> Graphics::cameraInfoBuffer = nullptr;
CComPtr<ID3D11Buffer> Graphics::lightInfoBuffer = nullptr;
CComPtr<ID3D11Buffer> Graphics::objectInfoBuffer = nullptr;
CComPtr<ID3D11Buffer> Graphics::shadowInfoBuffer = nullptr;

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
void Graphics::UpdateShadowInfo(ShadowInfo shadowInfo, ObjectPtr<Texture2D> shadowMap)
{
	Graphics::shadowInfo = shadowInfo;
	GL::UpdateBuffer(shadowInfoBuffer, &shadowInfo);
	GL::SetTexture(4, shadowMap.As<Texture>());
}

void Graphics::DrawMeshNow(ObjectPtr<Mesh> mesh)
{
	GL::SetMesh(mesh);
	GL::Render(mesh->GetTrianglesCount());
}
void Graphics::DrawTexture(Rect screenRect, ObjectPtr<Texture2D> texture)
{
	ObjectPtr<Texture2D> canvas = GetRenderTarget();
	Vector2 screenSize = { (float)canvas->GetWidth(), (float)canvas->GetHeight() };
	Vector3 minPosition = ScreenToViewPos(screenSize, screenRect.GetMin());
	Vector3 maxPosition = ScreenToViewPos(screenSize, screenRect.GetMax());

	ObjectPtr<Mesh> mesh = new Mesh();
	mesh->SetPositions({
		minPosition,
		{maxPosition.x,minPosition.y,0},
		maxPosition,
		{minPosition.x,maxPosition.y,0}
		});
	mesh->SetTriangles({
		0,1,3,
		3,1,2
		});
	mesh->SetUVs({
		{0,1},
		{1,1},
		{1,0},
		{0,0},
		});
	mesh->UploadMeshData();

	UpdateObjectInfo({ Matrix4x4::identity });
	UpdateCameraInfo(CameraInfo::Orthographic(
		Vector3::zero,
		Matrix4x4::identity,
		screenSize.x / screenSize.y,
		0, 1, screenSize.y
	));
	DrawMeshNow(mesh);
}
void Graphics::Blit(ObjectPtr<Texture2D> source, ObjectPtr<Texture2D> dest, ObjectPtr<Material> blitMaterial)
{
	GL::SetRenderTarget(dest);
	GL::Begin();

	blitMaterial->SetPass(0);
	DrawTexture({ 0,0,(float)source->GetWidth(),(float)source->GetHeight() }, source);

	GL::End();
	GL::SetRenderTarget(nullptr);
}

Graphics* Graphics::Initialize(Window* window, GL* gl, ObjectPtr<Material> blitMaterial, GL2D** gl2d)
{
	//��ʼ��Direct2D������Direct3D����Ⱦ�����Ա�ʵ�����ߵĻ�������
	*gl2d = GL2D::Initialize(GL::GetRenderTarget());

	//������Ⱦ�õ�ͨ�ó���������
	GL::CreateBuffer(&worldInfo, sizeof(WorldInfo), D3D11_BIND_CONSTANT_BUFFER, &worldInfoBuffer);
	GL::SetConstantBuffer(0, &worldInfoBuffer.p);
	GL::CreateBuffer(&cameraInfo, sizeof(CameraInfo), D3D11_BIND_CONSTANT_BUFFER, &cameraInfoBuffer);
	GL::SetConstantBuffer(1, &cameraInfoBuffer.p);
	GL::CreateBuffer(&lightInfo, sizeof(LightInfo), D3D11_BIND_CONSTANT_BUFFER, &lightInfoBuffer);
	GL::SetConstantBuffer(2, &lightInfoBuffer.p);
	GL::CreateBuffer(&objectInfo, sizeof(ObjectInfo), D3D11_BIND_CONSTANT_BUFFER, &objectInfoBuffer);
	GL::SetConstantBuffer(3, &objectInfoBuffer.p);
	GL::CreateBuffer(&shadowInfo, sizeof(ShadowInfo), D3D11_BIND_CONSTANT_BUFFER, &shadowInfoBuffer);
	GL::SetConstantBuffer(5, &shadowInfoBuffer.p);
	Graphics::blitMaterial = blitMaterial;

	window->AddMessageListener(OnWindowMessage);
	return new Graphics{};
}

void Graphics::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_PAINT:
	{
		GL::Begin();

		//��ȡ��Ⱦ�¼�
		std::vector<RenderObjectEvent*>& renderObjectEvents = RenderObjectEvent::renderObjectEvents;
		std::vector<DrawGizmosEvent*>& drawGizmosEvents = DrawGizmosEvent::drawGizmosEvents;

		//������ͨ����
		std::for_each(
			renderObjectEvents.begin(),
			renderObjectEvents.end(),
			[](RenderObjectEvent* eventP) {
				eventP->OnRenderObject();
			}
		);

		//����UI�����������
		GL2D::BeginDraw();
		std::for_each(
			drawGizmosEvents.begin(),
			drawGizmosEvents.end(),
			[](DrawGizmosEvent* eventP) {
				eventP->OnDrawGizmos();
			}
		);
		GL2D::EndDraw();//˳�����Ҫ��Direct2D����Ⱦ���������ҪDirect3D����ʾ

		GL::End();

		//��������
		SetTexture(0, nullptr);
		SetTexture(1, nullptr);
		SetTexture(2, nullptr);
		SetTexture(3, nullptr);
		SetTexture(4, nullptr);
		SetTexture(5, nullptr);

		break;
	}
	case WM_SIZE:
	{
		GL2D::ReleaseResources();//�ͷŶ�Direct3D��Ⱦ��������ã��Ӷ�ʹDirect3D�������´�������

		//���µ��������С
		Rect rect = { Vector2::zero ,window->GetSize() };
		GL::ResetRenderTarget(rect);

		GL2D::CreateResources(GL::GetRenderTarget());//ʹ��Direct3D�´���������
		break;
	}
	case WM_DESTROY:
	{
		Object::Destroy(Graphics::blitMaterial.GetPtr());
		break;
	}
	}
}
