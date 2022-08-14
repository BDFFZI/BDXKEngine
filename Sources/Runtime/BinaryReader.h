#pragma once
#include <sstream>
#include "TransferBase.h"

namespace BDXKEngine {
	class BinaryReader : public TransferBase {
	public:
		BinaryReader(std::iostream& stream) :stream(stream) {
		}

		void TransferData(char* value, int size) {
			stream.read(value, size);
		};
		void TransferInt(int* value) override {
			Read(value);
		};
		void TransferFloat(float* value) override {
			Read(value);
		};
		void TransferBool(bool* value) override {
			Read(value);
		};

		void TransferVector2(Vector2* value) override {
			Read(value);
		};
		void TransferVector3(Vector3* value) override {
			Read(value);
		};
		void TransferVector4(Vector4* value) override {
			Read(value);
		};
		void TransferColor(Color* value) override {
			Read(value);
		};
		void TransferRect(Rect* value) override {
			Read(value);
		};

		void TransferObject(ObjectPtrBase* value) override {
			std::wstring guid = {};
			TransferString(&guid);
			*value = Object::FindObjectOfInstanceID(GuidToInstanceID(guid));
		};
		void TransferString(std::wstring* value) override {
			int size = 0;
			TransferInt(&size);

			char* data = new char[size];
			TransferData(data, size);

			*value = reinterpret_cast<wchar_t*>(data);
			delete[] data;
		};
		void TransferBytes(std::unique_ptr<char>* value, int size) {
			*value = std::unique_ptr<char>(new char[size]);
			TransferData(value->get(), size);
		}
	private:
		std::iostream& stream;

		template<typename TValue>
		void Read(TValue* value) {
			read(reinterpret_cast<char*>(&value), sizeof(TValue));
		}
	};
}
