#include "Camera.h"
#include <algorithm>
#include <cmath>

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
		//不能为0,为0的话ClipPlaneParameter将出现除0而无法计算
		nearClipPlane = std::fmax(0.01f, distance);
	}
	void Camera::SetFarClipPlane(float distance)
	{
		//必须和nearClipPlane有差异，不然后续参数计算会出现除0
		farClipPlane = std::fmax(nearClipPlane + 0.01f, distance);
	}

	void Camera::OnRenderObject()
	{
		//上传相机信息和世界信息
		Graphics::UpdateWorldInfo(WorldInfo{ { background * 0.5f } });
		Graphics::UpdateCameraInfo(CameraInfo{ transform->GetWorldToLocalMatrix() ,fieldOfView,GetAspectRatio(),nearClipPlane,farClipPlane });


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
				{},
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
			//上传物体信息至渲染管线
			ObjectPtr<Transform> rendererTransform = renderer->GetGameObject()->GetTransform();//获取物体矩阵
			Graphics::UpdateObjectInfo(ObjectInfo{ rendererTransform->GetLocalToWorldMatrix() });

			//获取该物体的渲染管线资源
			ObjectPtr<Mesh> mesh = renderer->GetMesh();//获取网格
			ObjectPtr<Material> material = renderer->GetMaterial();//获取材质

			//每个灯光都需要单独渲染一遍
			for (int lightIndex = 0; lightIndex < lightsCount; lightIndex++)
			{
				PassType& lightPass = lightPasses[lightIndex];//获取Pass类型，每一次光照都是一次Pass
				Graphics::UpdateLightInfo(lightInfos[lightIndex]);//上传灯光信息至渲染管线

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