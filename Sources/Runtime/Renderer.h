#pragma once
#include "Component.h"
#include "Graphics.h"
#include <queue>

namespace BDXKEngine {
	class RendererEditor;
	class Renderer : public Component
	{
		friend RendererEditor;
	public:
		Material* GetMaterial();
		Mesh* GetMesh();
		bool GetCastShadows();
		bool GetReceiveShadows();

		void SetMaterial(Material* shader);
		void SetCastShadows(bool castShadows);
		void SetReceiveShadows(bool receiveShadows);
	protected:
		Renderer();

		void SetMesh(Mesh* mesh);
	private:
		static std::vector<ObjectPtr<Renderer>> renderers;

		Material* material;
		Mesh* mesh;
		bool castShadows;
		bool receiveShadows;
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
					return a->GetMaterial()->GetRenderQueue() > b->GetMaterial()->GetRenderQueue();
				}
			);
			return queue;
		}

		static Mesh* GetMesh(Renderer* renderer)
		{
			return renderer->GetMesh();
		}
	};
}