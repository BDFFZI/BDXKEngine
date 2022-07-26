#include "Light.h"
using namespace BDXKEngine;

std::vector<ObjectPtr<Light>> Light::lights;

Light::Light() :Component(L"New Light")
{
}

LightType Light::GetType()
{
	return type;
}
Color Light::GetColor()
{
	return color;
}
float Light::GetIntensity()
{
	return intensity;
}
RenderMode Light::GetRenderMode()
{
	return renderMode;
}

void Light::SetLightType(LightType type) {
	this->type = type;
	switch (type)
	{
	case LightType::Directional:
		shadowMap = new Texture2D{ 1024,1024 };
		break;
	case LightType::Point:
		shadowMap = new Texture2D{ 512,512 };
		break;
	default:
		throw 0;
	}
}
void Light::SetColor(Color color)
{
	this->color = color;
}
void Light::SetIntensity(float intensity)
{
	this->intensity = intensity;
}

LightInfo Light::GetLightInfo(int order)
{
	ObjectPtr<Transform> lightTransform = GetGameObject()->GetTransform();
	LightInfo lightInfo{
		Vector4(lightTransform->GetPosition(),1),
		Vector4(lightTransform->GetFront().GetNormalized(),1),
		color * intensity,
		{},
		type,
		renderMode,
		order
	};

	return lightInfo;
}
ShadowInfo Light::GetShadowInfo()
{
	ObjectPtr<Transform> transform = GetTransform();

	ShadowInfo shadowInfo;
	shadowInfo.worldToLightView = transform->GetWorldToLocalMatrix();
	switch (type)
	{
	case BDXKEngine::LightType::Directional:
		shadowInfo.viewToLightClip = CameraInfo::Orthographic(
			transform->GetPosition(),
			transform->GetWorldToLocalMatrix(),
			1, -25, 25, 25
		).viewToClip;
		break;
	case BDXKEngine::LightType::Point:
		shadowInfo.viewToLightClip = CameraInfo::Perspective(
			transform->GetPosition(),
			transform->GetWorldToLocalMatrix(),
			1, 0.01f, 25, 90
		).viewToClip;
		break;
	default:
		throw 0;
	}

	return shadowInfo;
}

void BDXKEngine::Light::OnAwake()
{
	lights.push_back(this);
	SetLightType(type);

	Component::OnAwake();
}
void BDXKEngine::Light::OnRenderObject()
{
	//渲染阴影贴图
	std::vector<ObjectPtr<Renderer>> renderers = RendererEditor::GetRenderersQueue();

	GL::SetRenderTarget(shadowMap);

	//重置纹理
	GL::Clear(true, true);
	//设置深度绘制着色器
	GraphicsSettings::shadowMapMaterial->SetPass(0);
	//设置阴影信息
	Graphics::UpdateShadowInfo(GetShadowInfo(), nullptr);

	//渲染深度信息
	for (ObjectPtr<Renderer>& renderer : renderers)
	{
		ObjectPtr<Transform> rendererTransform = renderer->GetTransform();
		Graphics::UpdateObjectInfo({ rendererTransform->GetLocalToWorldMatrix() });
		Graphics::DrawMeshNow(renderer->GetMesh());
	}

	GL::SetRenderTarget(nullptr);
}
void BDXKEngine::Light::OnDestroy()
{
	lights.erase(std::find(
		lights.begin(),
		lights.end(),
		ObjectPtr<Light>{this}
	));

	Component::OnDestroy();
}

ObjectPtr<Texture2D> BDXKEngine::LightEditor::GetShadowMap(ObjectPtr<Light> light)
{
	return light->shadowMap;
}
LightInfo BDXKEngine::LightEditor::GetLightInfo(ObjectPtr<Light> light, int order)
{
	return light->GetLightInfo(order);
}
ShadowInfo BDXKEngine::LightEditor::GetShadowInfo(ObjectPtr<Light> light)
{
	return light->GetShadowInfo();
}
std::vector<ObjectPtr<Light>>& BDXKEngine::LightEditor::GetLights()
{
	return Light::lights;
}
