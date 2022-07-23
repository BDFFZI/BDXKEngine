#pragma once
#include <queue>
#include "ObjectPtr.h"
#include "Component.h"
#include "Graphics.h"

namespace BDXKEngine {
	class RendererEditor;
	class Renderer : public Component
	{
		friend RendererEditor;
	public:
		Renderer(std::wstring name = L"New Renderer");

		ObjectPtr<Material> GetMaterial();
		ObjectPtr<Mesh> GetMesh();
		bool GetCastShadows();
		bool GetReceiveShadows();

		void SetMaterial(ObjectPtr<Material> shader);
		void SetCastShadows(bool castShadows);
		void SetReceiveShadows(bool receiveShadows);
	protected:
		void SetMesh(ObjectPtr<Mesh> mesh);
	private:
		static std::vector<ObjectPtr<Renderer>> renderers;//”…Rendererπ‹¿Ì

		ObjectPtr<Material> material;
		ObjectPtr<Mesh> mesh;
		bool castShadows;
		bool receiveShadows;

		void OnAwake()override
		{
			Component::OnAwake();

			material = nullptr;
			mesh = nullptr;
			castShadows = true;
			receiveShadows = true;
			renderers.push_back(this);
		}

		void OnDestroy()override {
			renderers.erase(std::find_if(
				renderers.begin(),
				renderers.end(),
				[=](ObjectPtr<Renderer>& item) {
					return item->GetInstanceID() == this->GetInstanceID();
				}
			));

			Component::OnDestroy();
		}
	};

	class RendererEditor {
	protected:
		static std::vector<ObjectPtr<Renderer>>& GetRenderers()
		{
			return Renderer::renderers;
		}

		static std::vector<ObjectPtr<Renderer>> GetRenderersQueue()
		{
			std::vector<ObjectPtr<Renderer>> queue = Renderer::renderers;
			std::sort(
				queue.begin(),
				queue.end(),
				[](ObjectPtr<Renderer>& a, ObjectPtr<Renderer>& b) {
					return a->GetMaterial()->GetRenderQueue() < b->GetMaterial()->GetRenderQueue();
				}
			);
			return queue;
		}

		static ObjectPtr<Mesh> GetMesh(ObjectPtr<Renderer> renderer)
		{
			return renderer->GetMesh();
		}
	};
}