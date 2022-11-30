#include "ComponentEditor.h"
#include "BDXKEngine/Framework/Core/Component.h"

namespace BDXKEditor
{
    ComponentEditor componentEditor;
    std::vector<std::string> names;
    std::vector<void*> values;
    std::vector<Type> types;

    void ComponentEditor::StaticConstructor()
    {
        AddEditorFallback([](const Reflective& reflective)
        {
            if (dynamic_cast<const Component*>(&reflective) != nullptr)
                return &componentEditor;

            return static_cast<ComponentEditor*>(nullptr);
        });
    }
    void ComponentEditor::OnInspectorGUI()
    {
        const Object* target = GetTarget().ToObjectBase();
        Transferer& gui = GetGUITransferer();

        const Reflection& reflection = Reflection::GetReflection(target);
        const int count = reflection.GetFields(GetTarget().ToObjectBase(), names, values, types);
        for (int i = 3; i < count; i++)
        {
            gui.TransferField(names[i], values[i], types[i]);
        }
    }
}
