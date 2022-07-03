#include "Transform.h"
#include<cmath>
#include<exception>
#include "GameObject.h"

std::vector<Transform*> Transform::rootTransforms{};

Transform* Transform::GetParent()
{
	return parent;
}
void Transform::SetParent(Transform* newparent)
{
	if (newparent != nullptr)
	{
		Transform* newUpLayer = newparent;
		do {
			if (this == newUpLayer)
			{
				throw std::exception("你在试图让一个父物体或自身成为其的孩子，这会导致嵌套循环，是不允许的。");
			}
			newUpLayer = newUpLayer->parent;
		} while (newUpLayer != nullptr);
	}

	//解绑旧父物体
	if (parent != nullptr)parent->children.erase(std::find(parent->children.begin(), parent->children.end(), this));
	else rootTransforms.erase(std::find(rootTransforms.begin(), rootTransforms.end(), this));
	//设置新父物体
	this->parent = newparent;
	//绑定新父物体
	if (parent != nullptr)parent->children.push_back(this);
	else rootTransforms.push_back(this);

	RenewPosition();
	RenewEulerAngles();
	RenewScale();
	RenewMatrix();
}
Transform* Transform::GetChild(int index)
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
void Transform::SetLocalPosition(Vector3 value)
{
	localPosition = value;

	RenewPosition();
	RenewMatrix();
}
void Transform::SetLocalEulerAngles(Vector3 value)
{
	value.x = (float)std::fmod(value.x, 360);
	value.y = (float)std::fmod(value.y, 360);
	value.z = (float)std::fmod(value.z, 360);
	localEulerAngles = value;

	RenewEulerAngles();
	RenewMatrix();
}
void Transform::SetLocalScale(Vector3 value)
{
	localScale = value;

	RenewScale();
	RenewMatrix();
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

inline std::wstring Transform::ToString() {
	std::wstringstream stream;
	stream << L"位置：" << localPosition.ToString() << std::endl;
	stream << L"旋转：" << localEulerAngles.ToString() << std::endl;
	stream << L"缩放：" << localScale.ToString() << std::endl;
	stream << L"父亲：" << (parent != nullptr ? parent->GetGameObject()->GetName() : L"nullptr") << std::endl;
	stream << L"孩子数量：" << std::to_wstring(GetChildCount()) << std::endl;
	return stream.str();
}

Transform::Transform()
{
	parent = nullptr;
	rootTransforms.push_back(this);

	localPosition = Vector3::zero;
	localEulerAngles = Vector3::zero;
	localScale = Vector3::one;

	position = Vector3::zero;
	eulerAngles = Vector3::zero;
	scale = Vector3::one;

	localToWorldMatrix = Matrix4x4::identity;
	worldToLocalMatrix = Matrix4x4::identity;
}

void Transform::RenewPosition()
{
	Vector3 parentalPosition = parent == nullptr ? Vector3::zero : parent->GetPosition();

	position = parentalPosition + localPosition;

	for (Transform* child : children)
		child->RenewPosition();
}
void Transform::RenewEulerAngles()
{
	Vector3 parentalEulerAngles = parent == nullptr ? Vector3::zero : parent->GetEulerAngles();

	eulerAngles.x = (float)std::fmod(parentalEulerAngles.x + localEulerAngles.x, 360);
	eulerAngles.y = (float)std::fmod(parentalEulerAngles.y + localEulerAngles.y, 360);
	eulerAngles.z = (float)std::fmod(parentalEulerAngles.z + localEulerAngles.z, 360);
	for (Transform* child : children)
		child->RenewEulerAngles();
}
void Transform::RenewScale()
{
	Vector3 parentalScale = parent == nullptr ? Vector3::one : parent->GetScale();

	scale.x = parentalScale.x * localScale.x;
	scale.y = parentalScale.y * localScale.y;
	scale.z = parentalScale.z * localScale.z;

	for (Transform* child : children)
		child->RenewScale();
}
void Transform::RenewMatrix()
{
	localToWorldMatrix = parent == nullptr ? Matrix4x4::identity : parent->GetLocalToWorldMatrix();
	localToWorldMatrix *= Matrix4x4::Translate(localPosition);
	localToWorldMatrix *= Matrix4x4::Rotate(localEulerAngles);
	localToWorldMatrix *= Matrix4x4::Scale(localScale);

	worldToLocalMatrix = localToWorldMatrix.GetInverse();

	for (Transform* child : children)
		child->RenewMatrix();
}



