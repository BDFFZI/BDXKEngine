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
	//��Ⱦ��Ӱ��ͼ
	std::vector<ObjectPtr<Renderer>> renderers = RendererEditor::GetRenderersQueue();

	Graphics::SetRenderTarget(shadowMap);
	GL::Begin();
	//��������
	GL::Clear(true, true);
	//���������Ϣ
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
