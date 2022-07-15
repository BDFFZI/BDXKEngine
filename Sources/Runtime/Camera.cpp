#include "Camera.h"
#include <algorithm>

namespace BDXKEngine {
	void Camera::SetClearFlags(ClearFlags clearFlags)
	{
		this->clearFlags = clearFlags;
	}

	void Camera::SetBackground(Color color)
	{
		this->background = color;
	}

	void Camera::OnRenderObject()
	{
		//计算相机矩阵和投影矩阵，填充世界信息
		WorldInfo worldInfo{};
		worldInfo.worldToCamera = transform->GetWorldToLocalMatrix();
		Vector3 viewSize = Screen::GetSize() / 1080 * 20;
		worldInfo.cameraToView = {
			//控制画面缩放并避免受窗口大小影响
			3600 / fieldOfView / viewSize.x,0,0,0,
			0,3600 / fieldOfView / viewSize.y,0,0,
			//利用裁剪功能避免z过小导致 xy / 0 的情况
			0,0,1,-clippingPlanes.x,
			//利用齐次坐标中的w分量实现近大远小公式 xy / z
			0,0,1,0
		};
		worldInfo.cameraPosition = Vector4(transform->GetPosition(), 1);
		worldInfo.environment = background * 0.5f;

		//获取所有可渲染物体
		std::vector<Renderer*> renderers = FindObjectsOfType<Renderer>();
		//渲染排序 TODO
		//遮挡剔除 TODO

		//获取所有可渲染灯光,(优先级排序 TODO)
		std::vector<Light*> lights = FindObjectsOfType<Light>();
		int lightsCount = (int)lights.size();
		//解析灯光，获取向渲染管线传输的数据
		std::vector<LightInfo> lightInfos{ lights.size() };
		std::vector<PassType> passTypes{ lights.size() };
		if (lightsCount != 0)
		{
			for (int i = 0; i < lightsCount; i++)
			{
				Light* light = lights[i];
				Transform* lightTransform = light->GetGameObject()->GetTransform();
				LightInfo lightInfo{
					Vector4(lightTransform->GetPosition(),1),
					Vector4(lightTransform->GetFront().GetNormalized(),1),
					light->GetColor() * light->GetIntensity(),
					light->GetType(),
					light->GetRenderMode(),
					i
				};

				lightInfos[i] = lightInfo;
				passTypes[i] = PassType::ForwardAdd;
			}

			//寻找最亮的方向光始或第一个光源，其始终使用ForwardBase
			int mainLight = 0;
			int intensity = 0;
			for (int i = 0; i < lightsCount; i++)
			{
				if (lights[i]->GetType() == LightType::Directional && lights[i]->GetIntensity() > intensity)
				{
					mainLight = i;
					intensity = lights[i]->GetIntensity();
				}
			}
			passTypes[mainLight] = PassType::ForwardBase;
		}
		else//没灯光时总不能不让他渲染吧，那就搞个不发光的平行光
		{
			lightInfos.push_back({
				Vector4({},1),
				Vector4(Vector3::front,1),
				Color::clear,
				LightType::Directional,
				RenderMode::Important,
				0
				});
			passTypes.push_back(PassType::ForwardBase);
			lightsCount++;
		}


		//开始渲染场景

		//渲染背景
		switch (clearFlags)
		{
		case ClearFlags::Color:
			GL::Clear(true, true, background);
			break;
		case ClearFlags::Skybox://天空盒 TODO
			break;
		case ClearFlags::Not:
			break;
		}

		//渲染物体
		for (Renderer* renderer : renderers)
		{
			//获取物体矩阵
			Transform* rendererTransform = renderer->GetGameObject()->GetTransform();
			worldInfo.localToWorld = rendererTransform->GetLocalToWorldMatrix();
			//设置渲染管线的渲染矩阵
			Graphics::UpdateWorldInfo(worldInfo);

			//每一次光照都是一次Pass
			for (int i = 0; i < lightsCount; i++)
			{
				//获取Pass数据
				PassType& passType = passTypes[i];
				Material* material = renderer->GetMaterial();
				int passIndex = material->FindPass(passType);
				if (passIndex != -1)
				{
					//设置渲染管线的灯光信息
					Graphics::UpdateLightInfo(lightInfos[i]);

					//设置Pass信息
					material->SetPass(passIndex);

					//渲染
					RendererEditor::Render(renderer);
				}
			}
		}
	}
}