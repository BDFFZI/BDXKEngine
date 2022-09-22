#pragma once
#include <sstream>
#include "Base/Serialization/Importer.h"
#include "Base/Object/ObjectManager.h"

namespace BDXKEngine {
	class BinaryImporter : public Importer, ObjectManager {
	public:
		BinaryImporter(std::iostream& stream);

		int TransferInt() override;
		float TransferFloat() override;
		bool TransferBool() override;

		Vector2 TransferVector2() override;
		Vector3 TransferVector3() override;
		Vector4 TransferVector4() override;
		Color TransferColor() override;
		Rect TransferRect() override;
		std::wstring TransferString() override;
		ObjectPtrBase TransferObject() override;

		void TransferBytes(void* source, int size);
	private:
		std::iostream& stream;

		template<typename TValue>
		void Read(TValue* value) {
			stream.read(reinterpret_cast<char*>(value), sizeof(TValue));
		}
	};
}
