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
		//����Ϊ0,Ϊ0�Ļ�ClipPlaneParameter�����ֳ�0���޷�����
		nearClipPlane = std::fmax(0.01f, distance);
	}
	void Camera::SetFarClipPlane(float distance)
	{
		//�����nearClipPlane�в��죬��Ȼ���������������ֳ�0
		farClipPlane = std::fmax(nearClipPlane + 0.01f, distance);
	}

	void Camera::OnRenderObject()
	{
		//�ϴ������Ϣ��������Ϣ
		Graphics::UpdateWorldInfo(WorldInfo{ { background * 0.5f } });
		Graphics::UpdateCameraInfo(CameraInfo{ transform->GetWorldToLocalMatrix() ,fieldOfView,GetAspectRatio(),nearClipPlane,farClipPlane });


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
				{},
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
			//�ϴ�������Ϣ����Ⱦ����
			ObjectPtr<Transform> rendererTransform = renderer->GetGameObject()->GetTransform();//��ȡ�������
			Graphics::UpdateObjectInfo(ObjectInfo{ rendererTransform->GetLocalToWorldMatrix() });

			//��ȡ���������Ⱦ������Դ
			ObjectPtr<Mesh> mesh = renderer->GetMesh();//��ȡ����
			ObjectPtr<Material> material = renderer->GetMaterial();//��ȡ����

			//ÿ���ƹⶼ��Ҫ������Ⱦһ��
			for (int lightIndex = 0; lightIndex < lightsCount; lightIndex++)
			{
				PassType& lightPass = lightPasses[lightIndex];//��ȡPass���ͣ�ÿһ�ι��ն���һ��Pass
				Graphics::UpdateLightInfo(lightInfos[lightIndex]);//�ϴ��ƹ���Ϣ����Ⱦ����

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