#pragma once
#include <string>
#include <map>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"
#include "Rect.h"

namespace BDXKEngine {
	class ObjectPtrBase;
	class Exporter {
	public:
		virtual void TransferInt(int value) = 0;
		virtual void TransferFloat(float value) = 0;
		virtual void TransferBool(bool value) = 0;
		virtual void TransferVector2(Vector2 value) = 0;
		virtual void TransferVector3(Vector3 value) = 0;
		virtual void TransferVector4(Vector4 value) = 0;
		virtual void TransferColor(Color value) = 0;
		virtual void TransferRect(Rect value) = 0;
		virtual void TransferString(std::wstring value) = 0;

		virtual void TransferBytes(void* value, int size) = 0;
		virtual void TransferObject(ObjectPtrBase& value) = 0;
	};
}