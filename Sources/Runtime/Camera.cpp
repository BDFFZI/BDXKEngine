#include "Camera.h"
#include <algorithm>
#include <cmath>
#include "Resources.h"
#include "Time.h"
#include "Debug.h"
#include "Screen.h"

namespace BDXKEngine {
	Camera::Camera() :Component(L"New Camera")
	{
	}

	float Camera::GetAspectRatio()
	{
		Vector3 viewSize = Screen::GetSize();
		return viewSize.x / viewSize.y;
	}

	void Camera::SetClearFlags(ClearFlags clearFlags)
	{
		this->clearFlags = clearFlags;
	}
	void Camera::SetBackground(Color color)
	{
		this->background = color;
	}
	void Camera::SetNearClipPlane(float distance)
	{

		switch (projection)
		{
		case BDXKEngine::Projection::Orthographic:
			nearClipPlane = distance;
			break;
		case BDXKEngine::Projection::Perspective://不能为0,为0的话ClipPlaneParameter将出现除0而无法计算
			nearClipPlane = std::fmax(0.01f, distance);
			break;
		}

	}
	void Camera::SetFarClipPlane(float distance)
	{
		//必须和nearClipPlane有差异，不然后续参数计算会出现除0
		farClipPlane = std::fmax(nearClipPlane + 0.01f, distance);
	}

	void Camera::OnAwake()
	{
		Component::OnAwake();

		transform = GetGameObject()->GetTransform();
	}
	void Camera::OnRenderObject()
	{
		//获取所有可渲染物体
		std::vector<ObjectPtr<Renderer>> renderers = RendererEditor::GetRenderersQueue();
		//渲染排序 TODO
		//遮挡剔除 TODO

		//获取所有可渲染灯光,(优先级排序 TODO)
		std::vector<ObjectPtr<Light>>& lights = LightEditor::GetLights();
		unsigned long lightsCount = (int)lights.size();
		//解析灯光的Pass类型
		std::vector<PassType> lightPasses{ lightsCount };
		if (lightsCount != 0)
		{
			//寻找最亮的方向光始或第一个光源，其始终使用ForwardBase
			int mainLight = 0;
			float intensity = 0;
			for (int i = 0; i < lightsCount; i++)
			{
				lightPasses[i] = PassType::ForwardAdd;
				ObjectPtr<Light> light = lights[i];
				if (light->GetType() == LightType::Directional && light->GetIntensity() > intensity)
				{
					mainLight = i;
					intensity = light->GetIntensity();
				}
			}

			//将该灯光放在第一个渲染
			std::swap(lights[mainLight], lights[0]);
			lightPasses[0] = PassType::ForwardBase;
		}


		//开始渲染场景

		//渲染背景,在上传相机世界数据等之前使用，否则会覆盖数据
		switch (clearFlags)
		{
		case ClearFlags::Color:
			GL::Clear(true, true, background);
			break;
		case ClearFlags::Skybox://天空盒 TODO
			GL::Clear(true, true);
			Resources::GetSkyboxMaterial()->SetMatrix(0, transform->GetLocalToWorldMatrix());
			Resources::GetSkyboxMaterial()->SetVector(0, Vector4{ transform->GetPosition(),1 });
			Resources::GetSkyboxMaterial()->SetPass(0);
			GL::SetTexture(6, GraphicsSettings::skybox.As<Texture>());
			Graphics::DrawTexture(Resources::GetWhiteTexture(), { Vector2::zero,Screen::GetSize() });
			break;
		case ClearFlags::Not:
			break;
		}

		//上传世界信息
		Graphics::UpdateWorldInfo(WorldInfo{
			{ background * 0.5f },
			{{Time::GetRealtimeSinceStartup(),0,0},0}
			});

		//上传相机信息
		switch (projection)
		{
		case Projection::Orthographic:
			Graphics::UpdateCameraInfo(CameraInfo::Orthographic(
				transform->GetPosition(),
				transform->GetWorldToLocalMatrix(),
				GetAspectRatio(),
				nearClipPlane, farClipPlane,
				size));
			break;
		case Projection::Perspective:
			Graphics::UpdateCameraInfo(CameraInfo::Perspective(
				transform->GetPosition(),
				transform->GetWorldToLocalMatrix(),
				GetAspectRatio(),
				nearClipPlane, farClipPlane,
				fieldOfView));
			break;
		}

		//渲染物体，注意没灯光时不会触发渲染
		for (ObjectPtr<Renderer> renderer : renderers)
		{
			//上传物体信息至渲染管线
			ObjectPtr<Transform> rendererTransform = renderer->GetGameObject()->GetTransform();//获取物体矩阵
			Graphics::UpdateObjectInfo(ObjectInfo{ rendererTransform->GetLocalToWorldMatrix() });

			//获取该物体的渲染管线资源
			ObjectPtr<Mesh> mesh = renderer->GetMesh();//获取网格
			ObjectPtr<Material> material = renderer->GetMaterial();//获取材质

			//每个灯光都需要单独渲染一遍
			for (int lightIndex = 0; lightIndex < lightsCount; lightIndex++)
			{
				//上传灯光和阴影信息至渲染管线
				Graphics::UpdateLightInfo(
					LightEditor::GetLightInfo(lights[lightIndex], lightIndex),
					LightEditor::GetShadowInfo(lights[lightIndex]),
					LightEditor::GetShadowMap(lights[lightIndex])
				);

				//渲染符合该类型的Pass
				PassType& lightPass = lightPasses[lightIndex];//获取Pass类型，每一次光照都是一次Pass
				int passCount = material->GetPassCount();
				for (int passIndex = 0; passIndex < passCount; passIndex++)
				{
					if (material->GetPassType(passIndex) == lightPass)
					{
						material->SetPass(passIndex);//将该Pass信息上传渲染管线
						Graphics::DrawMeshNow(mesh);//启动渲染管线进行渲染
					}
				}
			}
		}

		//清理渲染管线
		GL::SetTexture(0, nullptr);
		GL::SetTexture(1, nullptr);
		GL::SetTexture(2, nullptr);
		GL::SetTexture(3, nullptr);
		//阴影贴图
		GL::SetTexture(4, nullptr);
		GL::SetTexture(5, nullptr);
		//天空盒
		GL::SetTexture(6, nullptr);
	}
}