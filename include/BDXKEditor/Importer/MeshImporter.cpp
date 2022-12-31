#include "MeshImporter.h"
#include "BDXKEngine/Platform/GL/Mesh/MeshImport.h"

namespace BDXKEditor
{
    ObjectPtrBase MeshImporter::ImportObject(std::string filePath)
    {
        return static_cast<ObjectPtrBase>(MeshImport::Glb(filePath));
    }
}
