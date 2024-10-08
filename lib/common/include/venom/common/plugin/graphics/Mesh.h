///
/// Project: VenomEngine
/// @file Mesh.h
/// @date Oct, 01 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/math/Vector.h>
#include <venom/common/plugin/graphics/GraphicsPlugin.h>

#include <venom/common/plugin/graphics/Material.h>

#include <vector>

namespace venom
{
namespace common
{
class Model;
/// @brief Contains all the mesh's data and is the
/// main high-level interface for the user
class VENOM_COMMON_API Mesh : public GraphicsPluginObject
{
protected:
    Mesh();
public:
    static Mesh * Create();
    ~Mesh();

    virtual void Draw() = 0;

    /**
     * @brief Assigns Material to mesh
     * @param material
     */
    void SetMaterial(Material * material);


    const Material * GetMaterial() const;

private:
    /**
     * @brief Loads Mesh into the Graphics API from the current data
     * So it is expected that vertices, faces, ... are loaded in the Mesh beforehand
     * @return vc::Error::Failure if the loading failed, vc::Error::Success otherwise
     */
    virtual vc::Error __LoadMeshFromCurrentData() = 0;

protected:
    friend class Model;
    std::vector<vcm::VertexPos> __positions;
    std::vector<vcm::VertexNormal> __normals;
    std::vector<vcm::VertexColor> __colors[8];
    std::vector<vcm::VertexUV> __uvs[8];
    std::vector<uint32_t> __indices;
    std::vector<vcm::VertexTangent> __tangents;
    std::vector<vcm::VertexBitangent> __bitangents;
    Material * __material;
};


}
}