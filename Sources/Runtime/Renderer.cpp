#include "Renderer.h"
#include "RendererManager.h"

namespace BDXKEngine {
	ObjectPtr<Material> Renderer::GetMaterial()
	{
		return material;
	}
	ObjectPtr<Mesh> Renderer::GetMesh()
	{
		return mesh;
	}
	bool Renderer::GetCastShadows() {
		return castShadows;
	}
	bool Renderer::GetReceiveShadows()
	{
		return receiveShadows;
	}

	void Renderer::SetMaterial(ObjectPtr<Material> shader)
	{
		this->material = shader;
	}
	void Renderer::SetMesh(ObjectPtr<Mesh> mesh)
	{
		this->mesh = mesh;
	}
	void Renderer::Awake()
	{
		Component::Awake();

		RenderObjectHandler* renderObjectHandler = dynamic_cast<RenderObjectHandler*>(this);
		DrawGizmosHandler* drawGizmosHandler = dynamic_cast<DrawGizmosHandler*>(this);

		if (renderObjectHandler != nullptr) RendererManager::renderObjectEvents.insert(
			RendererManager::renderObjectEvents.begin(), renderObjectHandler);
		if (drawGizmosHandler != nullptr) RendererManager::drawGizmosEvents.insert(
			RendererManager::drawGizmosEvents.begin(), drawGizmosHandler);
		RendererManager::renderers.push_back(this);
	}
	void Renderer::Destroy() {
		RenderObjectHandler* renderObjectHandler = dynamic_cast<RenderObjectHandler*>(this);
		DrawGizmosHandler* drawGizmosHandler = dynamic_cast<DrawGizmosHandler*>(this);

		if (renderObjectHandler != nullptr)RendererManager::renderObjectEvents.erase(std::find_if(
			RendererManager::renderObjectEvents.begin(),
			RendererManager::renderObjectEvents.end(),
			[=](RenderObjectHandler*& item) {
				return item == renderObjectHandler;
			}
		));
		if (drawGizmosHandler != nullptr)RendererManager::drawGizmosEvents.erase(std::find_if(
			RendererManager::drawGizmosEvents.begin(),
			RendererManager::drawGizmosEvents.end(),
			[=](DrawGizmosHandler*& item) {
				return item == drawGizmosHandler;
			}
		));
		RendererManager::renderers.erase(std::find_if(
			RendererManager::renderers.begin(),
			RendererManager::renderers.end(),
			[=](ObjectPtr<Renderer>& item) {
				return item->GetInstanceID() == this->GetInstanceID();
			}
		));

		Component::Destroy();
	}
	void Renderer::SetCastShadows(bool castShadows)
	{
		this->castShadows = castShadows;
	}
	void Renderer::SetReceiveShadows(bool receiveShadows)
	{
		this->receiveShadows = receiveShadows;
	}



}

