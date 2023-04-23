#include "ComponentEditor.h"
#include "Core/Runtime/Framework/Actor/Component.h"

namespace BDXKEditor
{
    void ComponentEditor::StaticConstructor()
    {
        AddEditorFallback([](const ObjectPtrBase& objectPtr)
        {
            if (objectPtr.ToObject<Component>() != nullptr)
                return new ComponentEditor{};

            return static_cast<ComponentEditor*>(nullptr);
        });
    }
    void ComponentEditor::OnInspectorGUI() const
    {
        static std::vector<std::string> names = {};
        static std::vector<void*> values = {};
        static std::vector<Type> types = {};

        const Object* target = GetTarget().ToObject();
        Transferer& gui = GetGUI();

        const Reflection& reflection = Reflection::GetReflection(target);
        const int count = reflection.GetFields(GetTarget().ToObject(), names, values, types);
        for (int i = 3; i < count; i++)
        {
            gui.TransferFieldNative(names[i], values[i], types[i]);
        }
    }
}
