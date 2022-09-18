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

			//������ͨ����
			for (auto& renderObjectHandler : renderObjectEvents)
				renderObjectHandler->OnRenderObject();

			//��׼��Ⱦ���ߣ������Ⱦ��
			std::vector<Camera*> cameras = Object::FindObjectsOfType<Camera>();
			for (auto camera : cameras)
				camera->Render();

			//����UI�����������
			GL2D::BeginDraw();

			for (auto& drawGizmosHandler : drawGizmosEvents)
				drawGizmosHandler->OnDrawGizmos();

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