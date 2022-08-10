#include "Graphics.h"
#include <algorithm>
namespace BDXKEngine
{
	Vector3 ScreenToViewPos(Vector2 screenSize, Vector2 screenPos)
	{
		screenPos.y = screenSize.y - screenPos.y;
		screenPos = screenPos - screenSize / 2;
		return { screenPos.x,screenPos.y,0 };
	}

	ObjectPtr<Buffer> Graphics::worldInfoBuffer = nullptr;
	ObjectPtr<Buffer> Graphics::cameraInfoBuffer = nullptr;
	ObjectPtr<Buffer> Graphics::lightInfoBuffer = nullptr;
	ObjectPtr<Buffer> Graphics::objectInfoBuffer = nullptr;
	ObjectPtr<Buffer> Graphics::shadowInfoBuffer = nullptr;

	Window* Graphics::window = nullptr;
	ObjectPtr<Mesh> Graphics::drawTextureMesh = nullptr;
	ObjectPtr<Texture> Graphics::defaultTexture2D = nullptr;
	ObjectPtr<Texture> Graphics::defaultTextureCube = nullptr;

	/// <summary>
	/// 默认情况下HLSL以列向量的顺序方式接受数据，而矩阵传输是以行向量传入的
	/// 这会导致着色器得到的矩阵会被转置，于是就成了行向量矩阵
	/// 但这种情况可以通过着色器调整，目前BDXKEngine.hlsli内的矩阵已经可以正确接受行矩阵
	/// </summary>
	/// <param name="matrix"></param>
	void Graphics::UpdateWorldInfo(WorldInfo worldInfo) {
		worldInfoBuffer->SetData(&worldInfo);
	}
	void Graphics::UpdateCameraInfo(CameraInfo cameraInfo)
	{
		cameraInfoBuffer->SetData(&cameraInfo);
	}
	void Graphics::UpdateObjectInfo(ObjectInfo objectInfo)
	{
		objectInfoBuffer->SetData(&objectInfo);
	}
	void Graphics::UpdateLightInfo(LightInfo lightInfo, ShadowInfo shadowInfo, ObjectPtr<Texture> shadowMap)
	{
		lightInfoBuffer->SetData(&lightInfo);
		shadowInfoBuffer->SetData(&shadowInfo);
		if (lightInfo.lightType == LightType::Point)
		{
			GL::SetTexture(4, defaultTexture2D);
			GL::SetTexture(5, shadowMap);
		}
		else
		{
			GL::SetTexture(4, shadowMap);
			GL::SetTexture(5, defaultTextureCube);
		}
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

	void Graphics::Initialize(Window* window)
	{
		//创建渲染用的通用常量缓冲区
		worldInfoBuffer = new Buffer(BufferTarget::Constant, sizeof(WorldInfo));
		cameraInfoBuffer = new Buffer(BufferTarget::Constant, sizeof(CameraInfo));
		objectInfoBuffer = new Buffer(BufferTarget::Constant, sizeof(ObjectInfo));
		lightInfoBuffer = new Buffer(BufferTarget::Constant, sizeof(LightInfo));
		shadowInfoBuffer = new Buffer(BufferTarget::Constant, sizeof(ShadowInfo));
		//设置缓冲区
		GL::SetBuffer(1, worldInfoBuffer);
		GL::SetBuffer(2, cameraInfoBuffer);
		GL::SetBuffer(3, objectInfoBuffer);
		GL::SetBuffer(4, lightInfoBuffer);
		GL::SetBuffer(5, shadowInfoBuffer);

		defaultTexture2D = new Texture2D(1, 1);
		defaultTextureCube = new TextureCube(1, 1);
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

		Graphics::window = window;
		window->AddDestroyEvent([]() {
			drawTextureMesh = nullptr;
			defaultTexture2D = nullptr;
			defaultTextureCube = nullptr;
			});
	}
}