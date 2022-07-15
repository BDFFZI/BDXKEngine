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
		virtual void OnRender() = 0;
	private:
		Material* material = nullptr;
	};

	class RendererEditor {
	protected:
		static void Render(Renderer* renderer)
		{
			renderer->OnRender();
		}
	};
}