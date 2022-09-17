#pragma once
#include <vector>
#include "Component.h"
#include "Transform.h"
#include "Vector3.h"
#include "Matrix4x4.h"

namespace BDXKEngine {
	class TransformEditor;
	class Transform :public Component
	{
		friend TransformEditor;
	public:
		ObjectPtr<Transform> GetParent();
		void SetParent(ObjectPtr<Transform> parent);
		ObjectPtr<Transform> GetChild(int index);
		int GetChildCount();

		Vector3 GetPosition();
		Vector3 GetEulerAngles();
		Vector3 GetScale();
		Vector3 GetLocalPosition();
		Vector3 GetLocalEulerAngles();
		Vector3 GetLocalScale();
		Matrix4x4 GetLocalToWorldMatrix();
		Matrix4x4 GetWorldToLocalMatrix();
		Vector3 GetRight();
		Vector3 GetUp();
		Vector3 GetFront();

		void SetLocalPosition(Vector3 value);
		void SetLocalEulerAngles(Vector3 value);
		void SetLocalScale(Vector3 value);

		std::wstring ToString()override;
	protected:
		//void Import(Reader* reader)override;
		void Destroy()override;
	private:
		static std::vector<ObjectPtr<Transform>> rootTransforms;

		Vector3 localPosition;
		Vector3 localEulerAngles;
		Vector3 localScale;

		Vector3 position;
		Vector3 eulerAngles;
		Vector3 scale;
		Matrix4x4 localToWorldMatrix;
		Matrix4x4 worldToLocalMatrix;

		ObjectPtr<Transform> parent;
		std::vector<ObjectPtr<Transform>> children;

		void RenewSelfMatrix();

		void RenewPosition();
		void RenewEulerAngles();
		void RenewScale();
		void RenewMatrix();
	};

	class TransformEditor
	{
	protected:
		static std::vector<ObjectPtr<Transform>> GetRootTransforms()
		{
			return Transform::rootTransforms;
		}
	};
}