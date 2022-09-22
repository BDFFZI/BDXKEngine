#include "BinaryExporter.h"
#include "Base/Object/ObjectPtrBase.h"

namespace BDXKEngine {
	BinaryExporter::BinaryExporter(std::iostream& stream) :stream(stream) {
	}

	void BinaryExporter::TransferInt(int value) {
		Write(value);
	}
	void BinaryExporter::TransferFloat(float value) {
		Write(value);
	}
	void BinaryExporter::TransferBool(bool value) {
		Write(value);
	}

	void BinaryExporter::TransferVector2(Vector2 value) {
		Write(value);
	}
	void BinaryExporter::TransferVector3(Vector3 value) {
		Write(value);
	}
	void BinaryExporter::TransferVector4(Vector4 value) {
		Write(value);
	}
	void BinaryExporter::TransferColor(Color value) {
		Write(value);
	}
	void BinaryExporter::TransferRect(Rect value) {
		Write(value);
	}
	void BinaryExporter::TransferString(std::wstring value) {
		int size = value.length() * sizeof(wchar_t);
		char* data = reinterpret_cast<char*>(value.data());

		TransferInt(size);
		TransferBytes(data, size);
	}

	void BinaryExporter::TransferBytes(void* source, int size) {
		stream.write(reinterpret_cast<char*>(source), size);
	}
	void BinaryExporter::TransferObject(ObjectPtrBase& value) {
		TransferInt(value.GetInstanceID());
	}
}