#pragma once
#pragma once
#include <string>
#include <map>
#include "Base/Vector/Vector2.h"
#include "Base/Vector/Vector3.h"
#include "Base/Vector/Vector4.h"
#include "Base/Color.h"
#include "Base/Rect.h"

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
		virtual ObjectPtrBase TransferObjectPtr() = 0;

		virtual void TransferBytes(void* source, int size) = 0;

		virtual ~Importer() = default;
	};
}