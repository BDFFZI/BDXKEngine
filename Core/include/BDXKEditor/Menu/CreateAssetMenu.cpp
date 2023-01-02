#include "CreateAssetMenu.h"

#include "BDXKEngine/Function/Resources/ResourcesDefault.h"

namespace BDXKEditor
{
    ObjectPtr<Material> CreateMaterial()
    {
        return ResourcesDefault::GetStandardMaterial();
    }
    ObjectPtr<PhysicMaterial> CreatePhysicMaterial()
    {
        return Object::Create<PhysicMaterial>();
    }
}
