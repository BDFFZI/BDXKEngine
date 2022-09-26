﻿#include "Light.h"

namespace BDXKEngine {
	std::vector<ObjectPtr<Light>> Light::lights;

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

		ShadowInfo shadowInfo{
			transform->GetWorldToLocalMatrix(),
			CameraInfo::Orthographic(
				transform->GetPosition(),
				transform->GetWorldToLocalMatrix(),
				1, -25, 25, 25
			).viewToClip
		};

		return shadowInfo;
	}
	ObjectPtr<Texture> Light::GetShadowMap()
	{
		if (type == LightType::Point)
			return shadowMapCube.ToObjectPtr<Texture>();
		else
			return shadowMap.ToObjectPtr<Texture>();
	}

	void Light::Awake()
	{
		lights.emplace_back(this);

		shadowMap = Texture2D::Create(1024, 1024);
		shadowMapCube = TextureCube::Create(512, 512);

		Component::Awake();
	}
	void Light::OnPreRender()
	{
		//获取投射阴影的物体
		std::vector<ObjectPtr<Renderer>> renderers = RendererManager::GetRenderersQueue();
		//设置深度绘制着色器
		Resources::GetShadowMapMaterial()->SetPass(0);

		//渲染深度信息
		if (type == LightType::Point)
		{
			ObjectPtr<Transform> transform = GetTransform();
			Vector3 rotations[] = {
				{0,90,0},{0,-90,0},
				{-90,0,0},{90,0,0},
				{0,0,0},{0,180,0},
			};

			for (int index = 0; index < 6; index++)
			{
				Graphics::UpdateLightInfo(
					GetLightInfo(),
					ShadowInfo{
						Matrix4x4::TRS(transform->GetPosition(),rotations[index],Vector3::one).GetInverse(),
						Matrix4x4::Perspective(90,1,0.01f,25)
					},
					nullptr
				);

				GL::SetRenderTarget(shadowMapCube, index);
				GL::Clear(true, true);

				for (ObjectPtr<Renderer>& renderer : renderers)
				{
					ObjectPtr<Transform> rendererTransform = renderer->GetTransform();
					Graphics::UpdateObjectInfo({ rendererTransform->GetLocalToWorldMatrix() });
					Graphics::DrawMeshNow(renderer->GetMesh());
				}
			}
		}
		else
		{
			//设置灯光阴影信息
			Graphics::UpdateLightInfo(
				GetLightInfo(),
				GetShadowInfo(),
				nullptr
			);

			//设置阴影贴图
			GL::SetRenderTarget(shadowMap);
			GL::Clear(true, true);

			for (ObjectPtr<Renderer>& renderer : renderers)
			{
				ObjectPtr<Transform> rendererTransform = renderer->GetTransform();
				Graphics::UpdateObjectInfo({ rendererTransform->GetLocalToWorldMatrix() });
				Graphics::DrawMeshNow(renderer->GetMesh());
			}
		}

		GL::SetRenderTarget(nullptr);
	}
	void Light::Destroy()
	{
		lights.erase(std::find(
			lights.begin(),
			lights.end(),
			ObjectPtr<Light>{this}
		));
		Object::Destroy(shadowMap.ToObjectBase());
		Object::Destroy(shadowMapCube.ToObjectBase());

		Component::Destroy();
	}


	LightInfo LightEditor::GetLightInfo(ObjectPtr<Light> light, int order)
	{
		return light->GetLightInfo(order);
	}
	ShadowInfo LightEditor::GetShadowInfo(ObjectPtr<Light> light)
	{
		return light->GetShadowInfo();
	}
	ObjectPtr<Texture> LightEditor::GetShadowMap(ObjectPtr<Light> light)
	{
		return light->GetShadowMap();
	}
	std::vector<ObjectPtr<Light>>& LightEditor::GetLights()
	{
		return Light::lights;
	}
}