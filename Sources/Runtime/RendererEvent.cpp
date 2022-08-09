#include "RendererEvent.h"

namespace BDXKEngine {
	inline std::vector<RenderObjectHandler*> renderObjectEvents;
	inline std::vector<DrawGizmosHandler*> drawGizmosEvents;

	RenderObjectHandler::RenderObjectHandler()
	{
		renderObjectEvents.insert(renderObjectEvents.begin(), this);
	}
	DrawGizmosHandler::DrawGizmosHandler()
	{
		drawGizmosEvents.insert(drawGizmosEvents.begin(), this);
	}

	RendererManager* RendererManager::Initialize(Window* window)
	{
		window->AddRenewEvent([]() {
			GL::SetRenderTarget(nullptr);

			//������ͨ����
			for (auto& renderObjectHandler : renderObjectEvents)
				renderObjectHandler->OnRenderObject();

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
}

