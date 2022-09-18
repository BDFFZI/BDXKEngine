#pragma once
#include <queue>
#include "ObjectPtr.h"
#include "Component.h"
#include "Graphics.h"

namespace BDXKEngine {
	class RendererManager;
	class Renderer : public Component
	{
		friend RendererManager;
	public:
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
		ObjectPtr<Material> material = nullptr;
		ObjectPtr<Mesh> mesh = nullptr;
		bool castShadows = true;
		bool receiveShadows = true;

		void Awake()override;
		void Destroy()override;
	};
}