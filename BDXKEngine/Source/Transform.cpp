#include "Transform.h"

Transform Transform::root = []() {
	Transform transform;
	transform.name = "[root] : class Transform";
	return transform;
}();

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
	RenewLocalToWorldMatrix();
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
Matrix4x4 Transform::GetLocalToWorldMatrix()
{
	return localToWorldMatrix;
}

void Transform::SetLocalPosition(Vector3 value)
{
	localPosition = value;

	RenewPosition();
	RenewLocalToWorldMatrix();
}
void Transform::SetLocalEulerAngles(Vector3 value)
{
	value.x = Math::Mod(value.x, 360);
	value.y = Math::Mod(value.y, 360);
	value.z = Math::Mod(value.z, 360);
	localEulerAngles = value;

	RenewEulerAngles();
	RenewLocalToWorldMatrix();
}
void Transform::SetLocalScale(Vector3 value)
{
	localScale = value;

	RenewScale();
	RenewLocalToWorldMatrix();
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
	eulerAngles.x = Math::Mod(parentalEulerAngles.x + localEulerAngles.x, 360);
	eulerAngles.y = Math::Mod(parentalEulerAngles.y + localEulerAngles.y, 360);
	eulerAngles.z = Math::Mod(parentalEulerAngles.z + localEulerAngles.z, 360);
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
void Transform::RenewLocalToWorldMatrix()
{
	localToWorldMatrix = parent->GetLocalToWorldMatrix();
	localToWorldMatrix *= Matrix4x4::Translate(localPosition);
	localToWorldMatrix *= Matrix4x4::Rotate(localEulerAngles);
	localToWorldMatrix *= Matrix4x4::Scale(localScale);
	for (Transform* child : children)
		child->RenewLocalToWorldMatrix();
}



