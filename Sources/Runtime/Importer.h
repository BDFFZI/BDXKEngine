#pragma once
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
	class Importer {
	public:
		virtual int TransferInt() = 0;
		virtual float TransferFloat() = 0;
		virtual bool TransferBool() = 0;
		virtual Vector2 TransferVector2() = 0;
		virtual Vector3 TransferVector3() = 0;
		virtual Vector4 TransferVector4() = 0;
		virtual Color TransferColor() = 0;
		virtual Rect TransferRect() = 0;
		virtual std::wstring TransferString() = 0;

		virtual void TransferBytes(void* value, int size) = 0;
		virtual void TransferObject(ObjectPtrBase& value) = 0;
	};
}