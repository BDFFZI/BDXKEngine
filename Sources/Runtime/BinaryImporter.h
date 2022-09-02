#pragma once
#include <sstream>
#include "Object.h"
#include "Importer.h"

namespace BDXKEngine {
	class BinaryImporter : public Importer, ObjectManager {
	public:
		BinaryImporter(std::iostream& stream) :stream(stream) {
		}

		int TransferInt() override {
			int value = {};
			Read(&value);
			return value;
		};
		float TransferFloat() override {
			float value = {};
			Read(&value);
			return value;
		};
		bool TransferBool() override {
			bool value = {};
			Read(&value);
			return value;
		};

		Vector2 TransferVector2() override {
			Vector2 value = {};
			Read(&value);
			return value;
		};
		Vector3 TransferVector3() override {
			Vector3 value = {};
			Read(&value);
			return value;
		};
		Vector4 TransferVector4() override {
			Vector4 value = {};
			Read(&value);
			return value;
		};
		Color TransferColor() override {
			Color value = {};
			Read(&value);
			return value;
		};
		Rect TransferRect() override {
			Rect value = {};
			Read(&value);
			return value;
		};
		std::wstring TransferString() override {
			int size = TransferInt();
			char* data = new char[size];
			TransferBytes(data, size);

			std::wstring value = reinterpret_cast<wchar_t*>(data);
			delete[] data;
			return value;
		};

		void TransferBytes(void* value, int size) {
			stream.read(reinterpret_cast<char*>(value), size);
		}
		void TransferObject(ObjectPtrBase& value) override {
			std::wstring guid = TransferString();
			value = Object::FindObjectOfInstanceID(ObjectManager::GuidToInstanceID(guid));
		};
	private:
		std::iostream& stream;

		template<typename TValue>
		void Read(TValue* value) {
			read(reinterpret_cast<char*>(&value), sizeof(TValue));
		}
	};
}
