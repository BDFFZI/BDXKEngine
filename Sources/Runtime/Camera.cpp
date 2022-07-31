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
		case BDXKEngine::Projection::Perspective://����Ϊ0,Ϊ0�Ļ�ClipPlaneParameter�����ֳ�0���޷�����
			nearClipPlane = std::fmax(0.01f, distance);
			break;
		}

	}
	void Camera::SetFarClipPlane(float distance)
	{
		//�����nearClipPlane�в��죬��Ȼ���������������ֳ�0
		farClipPlane = std::fmax(nearClipPlane + 0.01f, distance);
	}

	void Camera::OnAwake()
	{
		Component::OnAwake();

		transform = GetGameObject()->GetTransform();
	}
	void Camera::OnRenderObject()
	{
		//��ȡ���п���Ⱦ����
		std::vector<ObjectPtr<Renderer>> renderers = RendererEditor::GetRenderersQueue();
		//��Ⱦ���� TODO
		//�ڵ��޳� TODO

		//��ȡ���п���Ⱦ�ƹ�,(���ȼ����� TODO)
		std::vector<ObjectPtr<Light>>& lights = LightEditor::GetLights();
		unsigned long lightsCount = (int)lights.size();
		//�����ƹ��Pass����
		std::vector<PassType> lightPasses{ lightsCount };
		if (lightsCount != 0)
		{
			//Ѱ�������ķ����ʼ���һ����Դ����ʼ��ʹ��ForwardBase
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

			//���õƹ���ڵ�һ����Ⱦ
			std::swap(lights[mainLight], lights[0]);
			lightPasses[0] = PassType::ForwardBase;
		}


		//��ʼ��Ⱦ����

		//��Ⱦ����,���ϴ�����������ݵ�֮ǰʹ�ã�����Ḳ������
		switch (clearFlags)
		{
		case ClearFlags::Color:
			GL::Clear(true, true, background);
			break;
		case ClearFlags::Skybox://��պ� TODO
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

		//�ϴ�������Ϣ
		Graphics::UpdateWorldInfo(WorldInfo{
			{ background * 0.5f },
			{{Time::GetRealtimeSinceStartup(),0,0},0}
			});

		//�ϴ������Ϣ
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

		//��Ⱦ���壬ע��û�ƹ�ʱ���ᴥ����Ⱦ
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
				//�ϴ��ƹ����Ӱ��Ϣ����Ⱦ����
				Graphics::UpdateLightInfo(
					LightEditor::GetLightInfo(lights[lightIndex], lightIndex),
					LightEditor::GetShadowInfo(lights[lightIndex]),
					LightEditor::GetShadowMap(lights[lightIndex])
				);

				//��Ⱦ���ϸ����͵�Pass
				PassType& lightPass = lightPasses[lightIndex];//��ȡPass���ͣ�ÿһ�ι��ն���һ��Pass
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

		//������Ⱦ����
		GL::SetTexture(0, nullptr);
		GL::SetTexture(1, nullptr);
		GL::SetTexture(2, nullptr);
		GL::SetTexture(3, nullptr);
		//��Ӱ��ͼ
		GL::SetTexture(4, nullptr);
		GL::SetTexture(5, nullptr);
		//��պ�
		GL::SetTexture(6, nullptr);
	}
}