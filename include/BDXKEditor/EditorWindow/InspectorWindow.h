#pragma once
#include <BDXKEngine/Engine.h>
#include "EditorWindow.h"

namespace BDXKEditor
{
    class Editor;
    using namespace BDXKEngine;

    class Inspector : public transferer
    {
    public:
        TransferDirection GetTransferDirection() override { return TransferDirection::Inspect; }
        std::string GetFieldPath() const { return fieldPath; }
        std::string GetFieldName() const { return fieldName; }
        std::string GetFieldID() const { return GetFieldName() + "##" + GetFieldPath(); }
        std::string GetFieldID(const std::string& name) const { return name + "##" + GetFieldPath() + name; }
        void PushPath(const std::string& key) override
        {
            fieldName = key;
            fieldPath.append(key);
        }
        void PopPath(std::string& key) override
        {
            fieldPath.resize(fieldPath.size() - key.size());
        }

        void TransferValue(int& value) override;
        void TransferValue(float& value) override;
        void TransferValue(bool& value) override;
        void TransferValue(Vector2& value) override;
        void TransferValue(Vector3& value) override;
        void TransferValue(Vector4& value) override;
        void TransferValue(Color& value) override;
        void TransferValue(Rect& value) override;
        void TransferValue(std::string& value) override;
        void TransferValue(ObjectPtrBase& value) override;
        void TransferValue(Serialization& value) override;
        void TransferValue(char* source, int size) override;
    private:
        std::string fieldPath;
        std::string fieldName;
    };

    class InspectorWindow : public EditorWindow
    {
        ObjectPtrBase lastTarget;
        ObjectPtr<Editor> editor;
        Inspector inspector;

        void OnGUI() override;
    };
}
