#include "Transform.h"
#include<cmath>
#include<exception>
#include "GameObject.h"

namespace BDXKEngine {
	std::vector<ObjectPtr<Transform>> Transform::rootTransforms{};

	ObjectPtr<Transform> Transform::GetParent()
	{
		return parent;
	}
	void Transform::SetParent(ObjectPtr<Transform> newparent)
	{
		if (newparent.IsNull() == false)
		{
			ObjectPtr<Transform> current = this;
			ObjectPtr<Transform> newUpLayer = newparent;
			do {
				if (newUpLayer == current)
				{
					throw std::exception("������ͼ��һ��������������Ϊ��ĺ��ӣ���ᵼ��Ƕ��ѭ�����ǲ�����ġ�");
				}
				newUpLayer = newUpLayer->parent;
			} while (newUpLayer.IsNull() == false);
		}

		//���ɸ�����
		if (parent.IsNull() == false)parent->children.erase(std::find(parent->children.begin(), parent->children.end(), this));
		else rootTransforms.erase(std::find(rootTransforms.begin(), rootTransforms.end(), this));
		//�����¸�����
		this->parent = newparent;
		//���¸�����
		if (parent.IsNull() == false)parent->children.push_back(this);
		else rootTransforms.push_back(this);

		RenewScale();
		RenewEulerAngles();
		RenewPosition();
	}
	ObjectPtr<Transform> Transform::GetChild(int index)
	{
		return children[index];
	}
	int Transform::GetChildCount()
	{
		return (int)children.size();
	}

	Vector3 Transform::GetPosition()
	{
		return position;
	}
	Vector3 Transform::GetEulerAngles()
	{
		return eulerAngles;
	}
	Vector3 Transform::GetScale()
	{
		return scale;
	}
	Vector3 Transform::GetLocalPosition()
	{
		return localPosition;
	}
	Vector3 Transform::GetLocalEulerAngles()
	{
		return localEulerAngles;
	}
	Vector3 Transform::GetLocalScale()
	{
		return localScale;
	}
	Matrix4x4 Transform::GetLocalToWorldMatrix()
	{
		return localToWorldMatrix;
	}
	Matrix4x4 Transform::GetWorldToLocalMatrix()
	{
		return worldToLocalMatrix;
	}
	Vector3 Transform::GetRight()
	{
		return localToWorldMatrix.MultiplyVector(Vector3::right);
	}
	Vector3 Transform::GetUp()
	{
		return localToWorldMatrix.MultiplyVector(Vector3::up);
	}
	Vector3 Transform::GetFront()
	{
		return localToWorldMatrix.MultiplyVector(Vector3::front);
	}

	void Transform::SetLocalPosition(Vector3 value)
	{
		localPosition = value;
		RenewPosition();
	}
	void Transform::SetLocalEulerAngles(Vector3 value)
	{
		value.x = (float)std::fmod(value.x, 360);
		value.y = (float)std::fmod(value.y, 360);
		value.z = (float)std::fmod(value.z, 360);
		localEulerAngles = value;
		RenewEulerAngles();
	}
	void Transform::SetLocalScale(Vector3 value)
	{
		localScale = value;
		RenewScale();
	}

	std::wstring Transform::ToString() {
		std::wstringstream stream;
		stream << Object::ToString() << std::endl;
		stream << L"λ�ã�" << localPosition.ToString() << std::endl;
		stream << L"��ת��" << localEulerAngles.ToString() << std::endl;
		stream << L"���ţ�" << localScale.ToString() << std::endl;
		stream << L"���ף�" << (parent.IsNull() == false ? parent->GetGameObject()->GetName() : L"nullptr") << std::endl;
		stream << L"����������" << std::to_wstring(GetChildCount()) << std::endl;
		return stream.str();
	}

	//void Transform::Import(Reader* reader)
	//{
	//	parent = nullptr;
	//	rootTransforms.push_back(this);

	//	localPosition = Vector3::zero;
	//	localEulerAngles = Vector3::zero;
	//	localScale = Vector3::one;

	//	position = Vector3::zero;
	//	eulerAngles = Vector3::zero;
	//	scale = Vector3::one;

	//	localToWorldMatrix = Matrix4x4::identity;
	//	worldToLocalMatrix = Matrix4x4::identity;
	//}

	void Transform::Destroy()
	{
		for (ObjectPtr<Transform> child : children)
			Object::Destroy(child->GetGameObject().GetPtr());
		children.clear();

		if (parent.IsNull())
		{
			rootTransforms.erase(std::find_if(
				rootTransforms.begin(),
				rootTransforms.end(),
				[=](ObjectPtr<Transform>& item) {
					return item->GetInstanceID() == this->GetInstanceID();
				}
			));
		}

		Component::Destroy();
	}

	void Transform::RenewSelfMatrix()
	{
		localToWorldMatrix = parent.IsNull() ? Matrix4x4::identity : parent->GetLocalToWorldMatrix();

		localToWorldMatrix = localToWorldMatrix * Matrix4x4::TRS(localPosition, localEulerAngles, localScale);
		worldToLocalMatrix = localToWorldMatrix.GetInverse();
	}

	void Transform::RenewPosition()
	{
		position = parent.IsNull() ? localPosition : parent->GetLocalToWorldMatrix().MultiplyPoint(localPosition);
		RenewSelfMatrix();

		for (ObjectPtr<Transform>& child : children)
			child->RenewPosition();
	}
	void Transform::RenewEulerAngles()
	{
		Vector3 parentalEulerAngles = parent.IsNull() ? Vector3::zero : parent->GetEulerAngles();

		eulerAngles.x = (float)std::fmod(parentalEulerAngles.x + localEulerAngles.x, 360);
		eulerAngles.y = (float)std::fmod(parentalEulerAngles.y + localEulerAngles.y, 360);
		eulerAngles.z = (float)std::fmod(parentalEulerAngles.z + localEulerAngles.z, 360);

		for (ObjectPtr<Transform>& child : children)
			child->RenewEulerAngles();

		RenewPosition();
	}
	void Transform::RenewScale()
	{
		Vector3 parentalScale = parent.IsNull() ? Vector3::one : parent->GetScale();

		scale.x = parentalScale.x * localScale.x;
		scale.y = parentalScale.y * localScale.y;
		scale.z = parentalScale.z * localScale.z;

		for (ObjectPtr<Transform>& child : children)
			child->RenewScale();

		RenewPosition();
	}
	void Transform::RenewMatrix()
	{
		RenewSelfMatrix();
		for (ObjectPtr<Transform>& child : children)
			child->RenewMatrix();
	}
}


