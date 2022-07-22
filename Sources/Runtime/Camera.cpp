#include "Camera.h"
#include <algorithm>

namespace BDXKEngine {
	Camera::Camera() :Component(L"New Camera")
	{
	}

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
		std::vector<ObjectPtr<Renderer>> renderers = RendererEditor::GetRenderersQueue();
		//渲染排序 TODO
		//遮挡剔除 TODO

		//获取所有可渲染灯光,(优先级排序 TODO)
		std::vector<ObjectPtr<Light>>& lights = LightEditor::GetLights();
		unsigned long lightsCount = (int)lights.size();
		//解析灯光，获取用来向渲染管线传输的数据
		std::vector<LightInfo> lightInfos{ lightsCount };
		std::vector<PassType> lightPasses{ lightsCount };
		if (lightsCount != 0)
		{
			for (int i = 0; i < lightsCount; i++)
			{
				lightInfos[i] = LightEditor::GetLightInfo(lights[i]);
				lightPasses[i] = PassType::ForwardAdd;
			}

			//寻找最亮的方向光始或第一个光源，其始终使用ForwardBase
			int mainLight = 0;
			int intensity = 0;
			for (int i = 0; i < lightsCount; i++)
			{
				ObjectPtr<Light> light = lights[i];
				if (light->GetType() == LightType::Directional && light->GetIntensity() > intensity)
				{
					mainLight = i;
					intensity = light->GetIntensity();
				}
			}
			lightPasses[mainLight] = PassType::ForwardBase;
		}
		else//没灯光时总不能不让他渲染吧，那就搞个假的的平行光
		{
			lightInfos.push_back({
				Vector4({},1),
				Vector4(Vector3::front,1),
				Color::clear,
				LightType::Directional,
				RenderMode::Important,
				0
				});
			lightPasses.push_back(PassType::ForwardBase);
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
		for (ObjectPtr<Renderer> renderer : renderers)
		{
			//获取该物体的渲染管线资源
			ObjectPtr<Mesh> mesh = renderer->GetMesh();//获取网格
			ObjectPtr<Material> material = renderer->GetMaterial();//获取材质
			ObjectPtr<Transform> rendererTransform = renderer->GetGameObject()->GetTransform();//获取物体矩阵
			worldInfo.localToWorld = rendererTransform->GetLocalToWorldMatrix();
			//上传渲染矩阵至渲染管线
			Graphics::UpdateWorldInfo(worldInfo);

			//每个灯光都需要单独渲染一遍
			for (int lightIndex = 0; lightIndex < lightsCount; lightIndex++)
			{
				Graphics::UpdateLightInfo(lightInfos[lightIndex]);//上传灯光信息至渲染管线
				PassType& lightPass = lightPasses[lightIndex];//获取Pass类型，每一次光照都是一次Pass

				//渲染符合该类型的Pass
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
	}
}