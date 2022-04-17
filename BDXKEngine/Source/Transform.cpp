#include "Transform.h"

Transform Transform::root = []() {
	Transform transform;
	transform.position = Vector3::zero;
	transform.eulerAngles = Vector3::zero;
	transform.scale = Vector3::one;
	transform.localToWorldMatrix = Matrix4x4::identity;
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
	ClearPosition();
	ClearEulerAngles();
	ClearScale();
	ClearLocalToWorldMatrix();

	//解绑旧父物体
	this->parent->children.erase(std::find(parent->children.begin(), parent->children.end(), this));
	//设置新父物体
	this->parent = (newparent == NULL ? &root : newparent);
	//绑定新父物体
	this->parent->children.push_back(this);
}
Transform* Transform::GetChild(int index)
{
	return children[index];
}
int Transform::GetChildCount()
{
	return (int)children.size();
}
#include"Debug.h"
#include"GameObject.h"
Vector3 Transform::GetPosition()
{
	if (position.IsNaN() == false)
		return position;

	position = parent->GetPosition() + localPosition;
	return position;
}
Vector3 Transform::GetEulerAngles()
{
	if (eulerAngles.IsNaN() == false)
		return eulerAngles;

	Vector3 parentalEulerAngles = parent->GetEulerAngles();
	eulerAngles.x = Math::Mod(parentalEulerAngles.x + localEulerAngles.x, 360);
	eulerAngles.y = Math::Mod(parentalEulerAngles.y + localEulerAngles.y, 360);
	eulerAngles.z = Math::Mod(parentalEulerAngles.z + localEulerAngles.z, 360);
	return eulerAngles;
}
Vector3 Transform::GetScale()
{
	if (scale.IsNaN() == false)
		return scale;

	Vector3 parentalScale = parent->GetScale();
	scale.x = parentalScale.x * localScale.x;
	scale.y = parentalScale.y * localScale.y;
	scale.z = parentalScale.z * localScale.z;
	return scale;
}
Matrix4x4 Transform::GetLocalToWorldMatrix()
{
	if (localToWorldMatrix != Matrix4x4::zero)
		return localToWorldMatrix;

	localToWorldMatrix = parent->GetLocalToWorldMatrix();
	localToWorldMatrix *= Matrix4x4::Translate(localPosition);
	localToWorldMatrix *= Matrix4x4::Rotate(localEulerAngles);
	localToWorldMatrix *= Matrix4x4::Scale(localScale);

	return localToWorldMatrix;
}

void Transform::SetLocalPosition(Vector3 value)
{
	ClearPosition();
	ClearLocalToWorldMatrix();

	localPosition = value;
}
void Transform::SetLocalEulerAngles(Vector3 value)
{
	ClearEulerAngles();
	ClearLocalToWorldMatrix();

	value.x = Math::Mod(value.x, 360);
	value.y = Math::Mod(value.y, 360);
	value.z = Math::Mod(value.z, 360);
	localEulerAngles = value;
}
void Transform::SetLocalScale(Vector3 value)
{
	ClearScale();
	ClearLocalToWorldMatrix();

	localScale = value;
}

Transform::Transform()
{
	localPosition = Vector3::zero;
	localEulerAngles = Vector3::zero;
	localScale = Vector3::one;

	position = Vector3::nan;
	eulerAngles = Vector3::nan;
	scale = Vector3::nan;
	localToWorldMatrix = Matrix4x4::zero;

	parent = &root;
	root.children.push_back(this);
}

void Transform::ClearPosition()
{
	position = Vector3::nan;
	for (Transform* child : children)
		child->ClearPosition();
}
void Transform::ClearEulerAngles()
{
	eulerAngles = Vector3::nan;
	for (Transform* child : children)
		child->ClearEulerAngles();
}
void Transform::ClearScale()
{
	scale = Vector3::nan;
	for (Transform* child : children)
		child->ClearScale();
}
void Transform::ClearLocalToWorldMatrix()
{
	localToWorldMatrix = Matrix4x4::zero;
	for (Transform* child : children)
		child->ClearLocalToWorldMatrix();
}



