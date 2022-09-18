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

		void TransferBytes(void* source, int size);
		void TransferObject(ObjectPtrBase& value) override;;
	private:
		std::iostream& stream;

		template<typename TValue>
		void Write(TValue value) {
			stream.write(reinterpret_cast<char*>(&value), sizeof(TValue));
		}
	};
}

//std::stringstream stream = {};
//BinaryExporter exporter = { stream };
//exporter.TransferInt(123);
//exporter.TransferVector3(Vector3{ 1,2,3 });
//exporter.TransferString(L"“ƒ¿÷ˆŒ");
//char binary[] = { 127,63,32 };
//exporter.TransferBytes(binary, 3);
//
//BinaryImporter importer = { stream };
//int intValue = importer.TransferInt();
//Vector3 vector3Value = importer.TransferVector3();
//std::wstring stringValue = importer.TransferString();
//char binaryValue[3];
//importer.TransferBytes(binaryValue, 3);