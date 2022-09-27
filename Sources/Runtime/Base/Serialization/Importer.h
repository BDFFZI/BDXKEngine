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
		virtual int TransferInt(std::wstring key) = 0;
		virtual float TransferFloat(std::wstring key) = 0;
		virtual bool TransferBool(std::wstring key) = 0;
		virtual Vector2 TransferVector2(std::wstring key) = 0;
		virtual Vector3 TransferVector3(std::wstring key) = 0;
		virtual Vector4 TransferVector4(std::wstring key) = 0;
		virtual Color TransferColor(std::wstring key) = 0;
		virtual Rect TransferRect(std::wstring key) = 0;
		virtual std::wstring TransferString(std::wstring key) = 0;
		virtual ObjectPtrBase TransferObjectPtr(std::wstring key) = 0;
		virtual void TransferBytes(std::wstring key, void* source, int size) = 0;

		virtual ~Importer() = default;
	};
}