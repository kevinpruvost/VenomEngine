///
/// Project: VenomEngine
/// @file Mesh.h
/// @date Oct, 01 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/String.h>
#include <venom/common/math/Vector.h>
#include <venom/common/plugin/graphics/GraphicsPlugin.h>

#include <venom/common/plugin/graphics/Material.h>

#include <vector>

namespace venom
{
namespace common
{
class ModelImpl;
class VENOM_COMMON_API MeshImpl : public GraphicsPluginObject
{
public:
    MeshImpl();
    virtual ~MeshImpl() = default;

    void SetMaterial(const Material & material);
    bool HasMaterial() const;
    const Material & GetMaterial() const;
    virtual void Draw() = 0;

private:
    /**
     * @brief Loads Mesh into the Graphics API from the current data
     * So it is expected that vertices, faces, ... are loaded in the Mesh beforehand
     * @return vc::Error::Failure if the loading failed, vc::Error::Success otherwise
     */
    virtual vc::Error __LoadMeshFromCurrentData() = 0;

protected:
    friend class ModelImpl;
    vc::Vector<vcm::VertexPos> _positions;
    vc::Vector<vcm::VertexNormal> _normals;
    vc::Vector<vcm::VertexColor> _colors[8];
    vc::Vector<vcm::VertexUV> _uvs[8];
    vc::Vector<uint32_t> _indices;
    vc::Vector<vcm::VertexTangent> _tangents;
    vc::Vector<vcm::VertexBitangent> _bitangents;
    PluginObjectOptional<Material> _material;
};

/// @brief Contains all the mesh's data and is the
/// main high-level interface for the user
class VENOM_COMMON_API Mesh : public PluginObjectWrapper
{
public:
    Mesh();
    ~Mesh();

    /**
     * @brief Assigns Material to mesh
     * @param material
     */
    inline void SetMaterial(const Material & material) {
        _impl->As<MeshImpl>()->SetMaterial(material);
    }

    /**
     * @brief Checks if the mesh has a Material assigned
     * @return true if a Material is assigned, false otherwise
     */
    inline bool HasMaterial() const {
        return _impl->As<MeshImpl>()->HasMaterial();
    }

    /**
     * @brief Returns the Material assigned to the mesh
     * @return Material
     */
    inline const Material & GetMaterial() const {
        return _impl->As<MeshImpl>()->GetMaterial();
    }

    /**
     * @brief Draws the mesh
    */
    inline void Draw() const {
        _impl->As<MeshImpl>()->Draw();
    }

private:
    friend class ModelImpl;
};


}
}