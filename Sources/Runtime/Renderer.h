#pragma once
#include "Component.h"
#include "Graphics.h"

namespace BDXKEngine {
	class RendererEditor;
	class Renderer : public Component
	{
		friend RendererEditor;
	public:
		Material* GetMaterial()
		{
			return material;
		}
		void SetMaterial(Material* shader)
		{
			this->material = shader;
		}
	protected:
		Renderer()
		{
			renderers.push_back(this);
		}

		virtual void OnRender() = 0;
	private:
		static std::vector<Renderer*> renderers;

		Material* material = nullptr;
	};

	class RendererEditor {
	protected:
		static std::vector<Renderer*>& GetRenderers()
		{
			return Renderer::renderers;
		}

		static void Render(Renderer* renderer)
		{
			renderer->OnRender();
		}
	};
}