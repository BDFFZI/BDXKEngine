#include "CreateAssetMenu.h"

#include "BDXKEngine/Function/Resources/ResourcesDefault.h"

namespace BDXKEditor
{
    ObjectPtr<Material> CreateStandardMaterial()
    {
        return ResourcesDefault::GetStandardMaterial();
    }
    ObjectPtr<Material> CreateSkyboxMaterial()
    {
        return ResourcesDefault::GetSkyboxMaterial();
    }
    ObjectPtr<PhysicMaterial> CreatePhysicMaterial()
    {
        return Object::Create<PhysicMaterial>();
    }
}
