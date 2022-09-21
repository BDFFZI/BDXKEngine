#include "RendererManager.h"
#include "Renderer.h"
#include "Framework/Components/Renderer/Camera.h"

namespace BDXKEngine {
	std::vector<ObjectPtr<Renderer>> RendererManager::renderers = {};

	RendererManager* RendererManager::Initialize(Window* window)
	{
		window->AddRenewEvent([]() {
			//获取渲染事件
			std::vector<PreRenderHandler*> preRenderHandlers = Object::FindObjectsOfType<PreRenderHandler>();
			std::vector<PostRenderHandler*> postRenderHandlers = Object::FindObjectsOfType<PostRenderHandler>();

			GL::SetRenderTarget(nullptr);
			GL2D::BeginDraw();
	
			std::vector<Camera*> cameras = Object::FindObjectsOfType<Camera>();
			for (auto camera : cameras)
			{
				//如：渲染阴影贴图
				for (auto preRenderHandler : preRenderHandlers)
					preRenderHandler->OnPreRender();

				//标准渲染管线（相机渲染）
				camera->Render();

				//如：绘制UI，后期物体等
				for (auto postRenderHandler : postRenderHandlers)
					postRenderHandler->OnPostRender();
			}

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