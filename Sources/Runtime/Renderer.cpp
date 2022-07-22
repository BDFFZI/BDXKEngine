#include "Renderer.h"
using namespace BDXKEngine;

std::vector<ObjectPtr<Renderer>> Renderer::renderers;

BDXKEngine::Renderer::Renderer(std::wstring name) :Component(name)
{
}

ObjectPtr<Material> BDXKEngine::Renderer::GetMaterial()
{
	return material;
}
ObjectPtr<Mesh> BDXKEngine::Renderer::GetMesh()
{
	return mesh;
}
bool BDXKEngine::Renderer::GetCastShadows() {
	return castShadows;
}
bool BDXKEngine::Renderer::GetReceiveShadows()
{
	return receiveShadows;
}

void BDXKEngine::Renderer::SetMaterial(ObjectPtr<Material> shader)
{
	this->material = shader;
}
void BDXKEngine::Renderer::SetMesh(ObjectPtr<Mesh> mesh)
{
	this->mesh = mesh;
}
void BDXKEngine::Renderer::SetCastShadows(bool castShadows)
{
	this->castShadows = castShadows;
}
void BDXKEngine::Renderer::SetReceiveShadows(bool receiveShadows)
{
	this->receiveShadows = receiveShadows;
}


