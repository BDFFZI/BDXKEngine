#pragma once
#include<vector>
#include "Component.h"
#include "Vector3.h"
#include "Matrix4x4.h"

class Transform :public Component
{
public:
	Transform* GetParent();
	void SetParent(Transform* parent);
	Transform* GetChild(int index);
	int GetChildCount();

	Vector3 GetPosition();
	Vector3 GetEulerAngles();
	Vector3 GetScale();

	Vector3 GetLocalPosition();
	Vector3 GetLocalEulerAngles();
	Vector3 GetLocalScale();
	void SetLocalPosition(Vector3 value);
	void SetLocalEulerAngles(Vector3 value);
	void SetLocalScale(Vector3 value);

	Matrix4x4 GetLocalToWorldMatrix();
	Matrix4x4 GetWorldToLocalMatrix();

	Vector3 GetRight();
	Vector3 GetUp();
	Vector3 GetFront();

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
	Matrix4x4 worldToLocalMatrix;

	Transform* parent;
	std::vector<Transform*> children;

	void RenewPosition();
	void RenewEulerAngles();
	void RenewScale();
	void RenewMatrix();
};