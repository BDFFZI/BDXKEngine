#pragma once
#include <sstream>
#include "Object.h"
#include "Exporter.h"
#include "ObjectPtr.h"

namespace BDXKEngine {
	class BinaryExporter :public Exporter, ObjectManager {
	public:
		BinaryExporter(std::iostream& stream) :stream(stream) {
		}

		void TransferInt(int value) override {
			Write(value);
		};
		void TransferFloat(float value) override {
			Write(value);
		};
		void TransferBool(bool value) override {
			Write(value);
		};

		void TransferVector2(Vector2 value) override {
			Write(value);
		};
		void TransferVector3(Vector3 value) override {
			Write(value);
		};
		void TransferVector4(Vector4 value) override {
			Write(value);
		};
		void TransferColor(Color value) override {
			Write(value);
		};
		void TransferRect(Rect value) override {
			Write(value);
		};
		void TransferString(std::wstring value) override {
			int size = value.length() * sizeof(wchar_t);
			char* data = reinterpret_cast<char*>(value.data());

			TransferInt(size);
			TransferBytes(data, size);
		};

		void TransferBytes(void* value, int size) {
			stream.write(reinterpret_cast<char*>(value), size);
		}
		void TransferObject(ObjectPtrBase& value) override {
			Object* object = value;
			std::wstring guid = ObjectManager::InstanceIDToGuid(object == nullptr ? 0 : object->GetInstanceID());
			TransferString(guid);
		};
	private:
		std::iostream& stream;

		template<typename TValue>
		void Write(TValue value) {
			stream.write(reinterpret_cast<char*>(&value), sizeof(TValue));
		}
	};
}