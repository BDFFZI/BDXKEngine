#include "Light.h"
using namespace BDXKEngine;

std::vector<ObjectPtr<Light>> Light::lights;

BDXKEngine::Light::Light() :Component(L"New Light")
{
	lights.push_back(this);
	shadowMap = new Texture2D{ 1024,1024 };
}

inline void BDXKEngine::Light::OnLateUpdate()
{
	//渲染阴影贴图
	std::vector<ObjectPtr<Renderer>> renderers = RendererEditor::GetRenderersQueue();

	Graphics::SetRenderTarget(shadowMap);
	GL::Begin();
	//重置纹理
	GL::Clear(true, true);
	//绘制深度信息
	ShaderEditor::SetPass(GraphicsSettings::shadowShader);
	for (ObjectPtr<Renderer>& renderer : renderers)
	{
		ObjectPtr<Transform> rendererTransform = renderer->GetTransform();
		Graphics::UpdateObjectInfo({ rendererTransform->GetLocalToWorldMatrix() });
		Graphics::UpdateCameraInfo({ GetTransform()->GetWorldToLocalMatrix(),90,1,0.01,100 });
		Graphics::DrawMeshNow(renderer->GetMesh());
	}

	GL::End();
	Graphics::SetRenderTarget(nullptr);
}
