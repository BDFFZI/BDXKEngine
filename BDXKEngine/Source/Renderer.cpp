#include "Renderer.h"
#include "GameObject.h"
#include "Graphics.h"

void Renderer::Start()
{
	transform = GetGameObject()->GetTransform();
}

void Renderer::OnRenderObject() {
	Graphics::SetMatrix(transform->GetLocalToWorldMatrix().ToMatrix3x2());
	Graphics::SetBrushColor(color);
	if (render != NULL)render();
}
