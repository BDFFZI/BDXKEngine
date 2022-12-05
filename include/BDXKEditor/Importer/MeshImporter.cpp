#include "MeshImporter.h"
#include "BDXKEngine/Platform/GL/Mesh/MeshImport.h"

namespace BDXKEditor
{
    ObjectPtrBase MeshImporter::Import(std::string filePath)
    {
        return static_cast<ObjectPtrBase>(MeshImport::Glb(filePath));
    }
}
