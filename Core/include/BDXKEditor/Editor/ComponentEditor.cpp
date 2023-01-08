#include "ComponentEditor.h"
#include "BDXKEngine/Framework/Core/Component.h"

namespace BDXKEditor
{
    void ComponentEditor::StaticConstructor()
    {
        static ComponentEditor componentEditor = {};

        AddEditorFallback([](const ObjectPtrBase& objectPtr)
        {
            if (objectPtr.ToObject<Component>() != nullptr)
                return &componentEditor;

            return static_cast<ComponentEditor*>(nullptr);
        });
    }
    void ComponentEditor::OnInspectorGUI() const
    {
        static std::vector<std::string> names = {};
        static std::vector<void*> values = {};
        static std::vector<Type> types = {};

        const Object* target = GetTarget().ToObjectBase();
        Transferer& gui = GetGUI();

        const Reflection& reflection = Reflection::GetReflection(target);
        const int count = reflection.GetFields(GetTarget().ToObjectBase(), names, values, types);
        for (int i = 3; i < count; i++)
        {
            gui.TransferField(names[i], values[i], types[i]);
        }
    }
}
