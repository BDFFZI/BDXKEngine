#include "Renderer.h"


void Renderer::Start()
{
	transform = GetGameObject()->GetTransform();
}

void Renderer::OnRenderObject() {
	Graphics2D::SetMatrix(transform->GetLocalToWorldMatrix().ToMatrix3x2());
	Graphics2D::SetBrushColor(color);
	if (render != NULL)render();
}
