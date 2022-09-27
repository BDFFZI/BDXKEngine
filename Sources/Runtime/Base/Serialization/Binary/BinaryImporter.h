#pragma once
#include <sstream>
#include "Base/Serialization/Importer.h"
#include "Base/Object/ObjectManager.h"

namespace BDXKEngine {
	class BinaryImporter : public Importer, ObjectManager {
	public:
		BinaryImporter(std::iostream& stream);

		int TransferInt(std::wstring key) override;
		float TransferFloat(std::wstring key) override;
		bool TransferBool(std::wstring key) override;

		Vector2 TransferVector2(std::wstring key) override;
		Vector3 TransferVector3(std::wstring key) override;
		Vector4 TransferVector4(std::wstring key) override;
		Color TransferColor(std::wstring key) override;
		Rect TransferRect(std::wstring key) override;
		std::wstring TransferString(std::wstring key) override;
		ObjectPtrBase TransferObjectPtr(std::wstring key) override;

		void TransferBytes(std::wstring key, void* source, int size);
	private:
		std::iostream& stream;

		template<typename TValue>
		void Read(TValue* value) {
			stream.read(reinterpret_cast<char*>(value), sizeof(TValue));
		}
	};
}
