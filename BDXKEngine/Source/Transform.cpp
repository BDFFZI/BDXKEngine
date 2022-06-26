#include "Transform.h"
#include<cmath>

Transform Transform::root{};

Transform* Transform::GetParent()
{
	if (parent == &root)
		return NULL;
	return parent;
}
void Transform::SetParent(Transform* newparent)
{
	//解绑旧父物体
	this->parent->children.erase(std::find(parent->children.begin(), parent->children.end(), this));
	//设置新父物体
	this->parent = (newparent == NULL ? &root : newparent);
	//绑定新父物体
	this->parent->children.push_back(this);

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
	return localToWorldMatrix.MultiplyVector(Vector3::right).GetNormalized();
}
Vector3 Transform::GetUp()
{
	return localToWorldMatrix.MultiplyVector(Vector3::up).GetNormalized();
}
Vector3 Transform::GetFront()
{
	return localToWorldMatrix.MultiplyVector(Vector3::front).GetNormalized();
}

Transform::Transform()
{
	parent = &root;
	root.children.push_back(this);

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
	position = parent->GetPosition() + localPosition;
	for (Transform* child : children)
		child->RenewPosition();
}
void Transform::RenewEulerAngles()
{
	Vector3 parentalEulerAngles = parent->GetEulerAngles();
	eulerAngles.x = (float)std::fmod(parentalEulerAngles.x + localEulerAngles.x, 360);
	eulerAngles.y = (float)std::fmod(parentalEulerAngles.y + localEulerAngles.y, 360);
	eulerAngles.z = (float)std::fmod(parentalEulerAngles.z + localEulerAngles.z, 360);
	for (Transform* child : children)
		child->RenewEulerAngles();
}
void Transform::RenewScale()
{
	Vector3 parentalScale = parent->GetScale();
	scale.x = parentalScale.x * localScale.x;
	scale.y = parentalScale.y * localScale.y;
	scale.z = parentalScale.z * localScale.z;
	for (Transform* child : children)
		child->RenewScale();
}
void Transform::RenewMatrix()
{
	localToWorldMatrix = parent->localToWorldMatrix;
	localToWorldMatrix *= Matrix4x4::Translate(localPosition);
	localToWorldMatrix *= Matrix4x4::Rotate(localEulerAngles);
	localToWorldMatrix *= Matrix4x4::Scale(localScale);

	worldToLocalMatrix = localToWorldMatrix.GetInverse();

	for (Transform* child : children)
		child->RenewMatrix();
}



