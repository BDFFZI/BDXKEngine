#pragma once
#include <sstream>
#include "TransferBase.h"

namespace BDXKEngine {
	class BinaryWriter :public TransferBase {
	public:
		BinaryWriter(std::iostream& stream) :stream(stream) {
		}

		void TransferData(char* value, int size) {
			stream.write(value, size);
		};
		void TransferInt(int* value) override {
			Write(*value);
		};
		void TransferFloat(float* value) override {
			Write(*value);
		};
		void TransferBool(bool* value) override {
			Write(*value);
		};

		void TransferVector2(Vector2* value) override {
			Write(*value);
		};
		void TransferVector3(Vector3* value) override {
			Write(*value);
		};
		void TransferVector4(Vector4* value) override {
			Write(*value);
		};
		void TransferColor(Color* value) override {
			Write(*value);
		};
		void TransferRect(Rect* value) override {
			Write(*value);
		};

		void TransferObject(ObjectPtrBase* value) override {
			Object* object = *value;
			std::wstring guid = InstanceIDToGuid(object == nullptr ? 0 : object->GetInstanceID());
			TransferString(&guid);
		};
		void TransferString(std::wstring* value) override {
			int size = value->length() * sizeof(wchar_t);
			char* data = reinterpret_cast<char*>(value->data());

			TransferInt(&size);
			TransferData(data, size);
		};
		void TransferBytes(std::unique_ptr<char>* value, int size) {
			TransferData(value->get(), size);
		}
	private:
		std::iostream& stream;

		template<typename TValue>
		void Write(TValue value) {
			stream.write(reinterpret_cast<char*>(&value), sizeof(TValue));
		}
	};
}