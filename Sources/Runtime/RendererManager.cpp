#include "RendererManager.h"
#include "Renderer.h"
#include "Camera.h"

namespace BDXKEngine {
	std::vector<ObjectPtr<Renderer>> RendererManager::renderers = {};
	std::vector<RenderObjectHandler*> RendererManager::renderObjectEvents = {};
	std::vector<DrawGizmosHandler*> RendererManager::drawGizmosEvents = {};

	RendererManager* RendererManager::Initialize(Window* window)
	{
		window->AddRenewEvent([]() {
			GL::SetRenderTarget(nullptr);

			//绘制普通物体
			for (auto& renderObjectHandler : renderObjectEvents)
				renderObjectHandler->OnRenderObject();

			//标准渲染管线（相机渲染）
			std::vector<Camera*> cameras = Object::FindObjectsOfType<Camera>();
			for (auto camera : cameras)
				camera->Render();

			//绘制UI，后期物体等
			GL2D::BeginDraw();

			for (auto& drawGizmosHandler : drawGizmosEvents)
				drawGizmosHandler->OnDrawGizmos();

			GL2D::EndDraw();//顺序很重要，Direct2D的渲染结果最终需要Direct3D来显示

			GL::Present();//显示到屏幕上
			});
		window->AddResizeEvent([](Vector2 size) {
			GL2D::ReleaseResources();//释放对Direct3D渲染纹理的引用，从而使Direct3D可以重新创建纹理

			//重新调整纹理大小
			Rect rect = { Vector2::zero ,size };
			GL::ResizeDefaultRenderTarget(rect.GetSize());

			GL2D::CreateResources();//使用Direct3D新创建的纹理
			});

		return nullptr;
	}

	std::vector<ObjectPtr<Renderer>>& RendererManager::GetRenderers()
	{
		return RendererManager::renderers;
	}
	std::vector<ObjectPtr<Renderer>> RendererManager::GetRenderersQueue()
	{
		std::vector<ObjectPtr<Renderer>> queue = RendererManager::renderers;
		std::sort(
			queue.begin(),
			queue.end(),
			[](ObjectPtr<Renderer>& a, ObjectPtr<Renderer>& b) {
				return a->GetMaterial()->GetRenderQueue() < b->GetMaterial()->GetRenderQueue();
			}
		);
		return queue;
	}
}