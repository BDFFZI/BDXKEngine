#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "List.h"

class Transform :
	public Component
{
public:
	Transform* GetParent();
	void SetParent(Transform* parent);
	Transform* GetChild(int index);
	int GetChildCount();
	Vector3 GetPosition();
	Vector3 GetEulerAngles();
	Vector3 GetScale();
	Matrix4x4 GetLocalToWorldMatrix();
	void SetLocalPosition(Vector3 value);
	void SetLocalEulerAngles(Vector3 value);
	void SetLocalScale(Vector3 value);

	Transform();
private:
	static Transform root;

	Vector3 localPosition;
	Vector3 localEulerAngles;
	Vector3 localScale;

	Vector3 position;
	Vector3 eulerAngles;
	Vector3 scale;
	Matrix4x4 localToWorldMatrix;

	Transform* parent;
	List<Transform*> children;

	void RenewPosition();
	void RenewEulerAngles();
	void RenewScale();
	void RenewLocalToWorldMatrix();
};
