#include "RendererManager.h"
#include "Renderer.h"
#include "Camera.h"

namespace BDXKEngine {
	std::vector<ObjectPtr<Renderer>> RendererManager::renderers = {};

	RendererManager* RendererManager::Initialize(Window* window)
	{
		window->AddRenewEvent([]() {
			//��ȡ��Ⱦ�¼�
			std::vector<PreRenderHandler*> preRenderHandlers = Object::FindObjectsOfType<PreRenderHandler>();
			std::vector<PostRenderHandler*> postRenderHandlers = Object::FindObjectsOfType<PostRenderHandler>();

			GL::SetRenderTarget(nullptr);
			GL2D::BeginDraw();
	
			std::vector<Camera*> cameras = Object::FindObjectsOfType<Camera>();
			for (auto camera : cameras)
			{
				//�磺��Ⱦ��Ӱ��ͼ
				for (auto preRenderHandler : preRenderHandlers)
					preRenderHandler->OnPreRender();

				//��׼��Ⱦ���ߣ������Ⱦ��
				camera->Render();

				//�磺����UI�����������
				for (auto postRenderHandler : postRenderHandlers)
					postRenderHandler->OnPostRender();
			}

			GL2D::EndDraw();//˳�����Ҫ��Direct2D����Ⱦ���������ҪDirect3D����ʾ
			GL::Present();//��ʾ����Ļ��
			});
		window->AddResizeEvent([](Vector2 size) {
			GL2D::ReleaseResources();//�ͷŶ�Direct3D��Ⱦ��������ã��Ӷ�ʹDirect3D�������´�������

			//���µ��������С
			Rect rect = { Vector2::zero ,size };
			GL::ResizeDefaultRenderTarget(rect.GetSize());

			GL2D::CreateResources();//ʹ��Direct3D�´���������
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