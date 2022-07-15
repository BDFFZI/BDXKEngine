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
		std::vector<Renderer*> renderers = FindObjectsOfType<Renderer>();
		//��Ⱦ���� TODO
		//�ڵ��޳� TODO

		//��ȡ���п���Ⱦ�ƹ�,(���ȼ����� TODO)
		std::vector<Light*> lights = FindObjectsOfType<Light>();
		int lightsCount = (int)lights.size();
		//�����ƹ⣬��ȡ����Ⱦ���ߴ��������
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

			//Ѱ�������ķ����ʼ���һ����Դ����ʼ��ʹ��ForwardBase
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
		else//û�ƹ�ʱ�ܲ��ܲ�������Ⱦ�ɣ��Ǿ͸���������ƽ�й�
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
		for (Renderer* renderer : renderers)
		{
			//��ȡ�������
			Transform* rendererTransform = renderer->GetGameObject()->GetTransform();
			worldInfo.localToWorld = rendererTransform->GetLocalToWorldMatrix();
			//������Ⱦ���ߵ���Ⱦ����
			Graphics::UpdateWorldInfo(worldInfo);

			//ÿһ�ι��ն���һ��Pass
			for (int i = 0; i < lightsCount; i++)
			{
				//��ȡPass����
				PassType& passType = passTypes[i];
				Material* material = renderer->GetMaterial();
				int passIndex = material->FindPass(passType);
				if (passIndex != -1)
				{
					//������Ⱦ���ߵĵƹ���Ϣ
					Graphics::UpdateLightInfo(lightInfos[i]);

					//����Pass��Ϣ
					material->SetPass(passIndex);

					//��Ⱦ
					RendererEditor::Render(renderer);
				}
			}
		}
	}
}