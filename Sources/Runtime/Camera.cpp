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
		//������������ͶӰ�������������Ϣ
		WorldInfo worldInfo{};
		worldInfo.worldToCamera = transform->GetWorldToLocalMatrix();
		Vector3 viewSize = Screen::GetSize() / 1080 * 20;
		worldInfo.cameraToView = {
			//���ƻ������Ų������ܴ��ڴ�СӰ��
			3600 / fieldOfView / viewSize.x,0,0,0,
			0,3600 / fieldOfView / viewSize.y,0,0,
			//���òü����ܱ���z��С���� xy / 0 �����
			0,0,1,-clippingPlanes.x,
			//������������е�w����ʵ�ֽ���ԶС��ʽ xy / z
			0,0,1,0
		};
		worldInfo.cameraPosition = Vector4(transform->GetPosition(), 1);
		worldInfo.environment = background * 0.5f;

		//��ȡ���п���Ⱦ����
		std::vector<ObjectPtr<Renderer>> renderers = RendererEditor::GetRenderersQueue();
		//��Ⱦ���� TODO
		//�ڵ��޳� TODO

		//��ȡ���п���Ⱦ�ƹ�,(���ȼ����� TODO)
		std::vector<ObjectPtr<Light>>& lights = LightEditor::GetLights();
		unsigned long lightsCount = (int)lights.size();
		//�����ƹ⣬��ȡ��������Ⱦ���ߴ��������
		std::vector<LightInfo> lightInfos{ lightsCount };
		std::vector<PassType> lightPasses{ lightsCount };
		if (lightsCount != 0)
		{
			for (int i = 0; i < lightsCount; i++)
			{
				lightInfos[i] = LightEditor::GetLightInfo(lights[i]);
				lightPasses[i] = PassType::ForwardAdd;
			}

			//Ѱ�������ķ����ʼ���һ����Դ����ʼ��ʹ��ForwardBase
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
		else//û�ƹ�ʱ�ܲ��ܲ�������Ⱦ�ɣ��Ǿ͸���ٵĵ�ƽ�й�
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


		//��ʼ��Ⱦ����

		//��Ⱦ����
		switch (clearFlags)
		{
		case ClearFlags::Color:
			GL::Clear(true, true, background);
			break;
		case ClearFlags::Skybox://��պ� TODO
			break;
		case ClearFlags::Not:
			break;
		}

		//��Ⱦ����
		for (ObjectPtr<Renderer> renderer : renderers)
		{
			//��ȡ���������Ⱦ������Դ
			ObjectPtr<Mesh> mesh = renderer->GetMesh();//��ȡ����
			ObjectPtr<Material> material = renderer->GetMaterial();//��ȡ����
			ObjectPtr<Transform> rendererTransform = renderer->GetGameObject()->GetTransform();//��ȡ�������
			worldInfo.localToWorld = rendererTransform->GetLocalToWorldMatrix();
			//�ϴ���Ⱦ��������Ⱦ����
			Graphics::UpdateWorldInfo(worldInfo);

			//ÿ���ƹⶼ��Ҫ������Ⱦһ��
			for (int lightIndex = 0; lightIndex < lightsCount; lightIndex++)
			{
				Graphics::UpdateLightInfo(lightInfos[lightIndex]);//�ϴ��ƹ���Ϣ����Ⱦ����
				PassType& lightPass = lightPasses[lightIndex];//��ȡPass���ͣ�ÿһ�ι��ն���һ��Pass

				//��Ⱦ���ϸ����͵�Pass
				int passCount = material->GetPassCount();
				for (int passIndex = 0; passIndex < passCount; passIndex++)
				{
					if (material->GetPassType(passIndex) == lightPass)
					{
						material->SetPass(passIndex);//����Pass��Ϣ�ϴ���Ⱦ����
						Graphics::DrawMeshNow(mesh);//������Ⱦ���߽�����Ⱦ
					}
				}
			}
		}
	}
}