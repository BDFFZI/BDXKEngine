#include "Renderer.h"
using namespace BDXKEngine;

std::vector<ObjectPtr<Renderer>> Renderer::renderers;

Material* BDXKEngine::Renderer::GetMaterial()
{
	return material;
}
Mesh* BDXKEngine::Renderer::GetMesh()
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

void BDXKEngine::Renderer::SetMaterial(Material* shader)
{
	this->material = shader;
}
void BDXKEngine::Renderer::SetMesh(Mesh* mesh)
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

BDXKEngine::Renderer::Renderer()
{
	material = nullptr;
	mesh = nullptr;
	castShadows = true;
	receiveShadows = true;
	renderers.push_back({ this });
}


