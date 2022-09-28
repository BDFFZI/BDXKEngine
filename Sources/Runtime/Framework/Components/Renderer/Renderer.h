#pragma once
#include "Base/Object/ObjectPtr.h"
#include "Framework/Component.h"
#include "Function/Graphics.h"

namespace BDXKEngine {
	class RendererManager;
	class Renderer : public Component
	{
		friend RendererManager;
	public:
		ObjectPtr<Material> GetMaterial();
		ObjectPtr<Mesh> GetMesh();
		bool GetCastShadows() const;
		bool GetReceiveShadows() const;

		void SetMaterial(ObjectPtr<Material> material);
		void SetCastShadows(bool castShadows);
		void SetReceiveShadows(bool receiveShadows);
	protected:
		virtual void SetMesh(ObjectPtr<Mesh> mesh);
	private:
		ObjectPtr<Material> material = nullptr;
		ObjectPtr<Mesh> mesh = nullptr;
		bool castShadows = true;
		bool receiveShadows = true;

		void Enable() override;
		void Disable() override;
		void Transfer(Transferrer& transferrer) override;
	};
}