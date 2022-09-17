#pragma once
#include <sstream>
#include "Exporter.h"
#include "ObjectManager.h"

namespace BDXKEngine {
	class BinaryExporter :public Exporter, ObjectManager {
	public:
		BinaryExporter(std::iostream& stream);

		void TransferInt(int value) override;;
		void TransferFloat(float value) override;;
		void TransferBool(bool value) override;;

		void TransferVector2(Vector2 value) override;;
		void TransferVector3(Vector3 value) override;;
		void TransferVector4(Vector4 value) override;;
		void TransferColor(Color value) override;;
		void TransferRect(Rect value) override;;
		void TransferString(std::wstring value) override;;

		void TransferBytes(void* value, int size);
		void TransferObject(ObjectPtrBase& value) override;;
	private:
		std::iostream& stream;

		template<typename TValue>
		void Write(TValue value) {
			stream.write(reinterpret_cast<char*>(&value), sizeof(TValue));
		}
	};
}