#pragma once
#include "BDXKEngine/Base/Object/ObjectPtr.h"
#include "BDXKEngine/Framework/Component/Component.h"
#include "BDXKEngine/Function/Graphics.h"

namespace BDXKEngine {
	class RenderManager;
	class Renderer : public Component
	{
		friend RenderManager;
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
		void Transfer(transferer& transferer) override;
	};
}