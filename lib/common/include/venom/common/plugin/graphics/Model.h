///
/// Project: VenomEngine
/// @file Mesh.h
/// @date Aug, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/math/Vector.h>
#include <venom/common/plugin/graphics/Mesh.h>
#include <venom/common/plugin/graphics/GraphicsPluginObject.h>

#include <vector>

namespace venom
{
namespace common
{
class VENOM_COMMON_API ModelImpl : public PluginObjectImpl
{
public:
    ModelImpl();
    virtual ~ModelImpl() = default;

    vc::Error ImportModel(const std::string & path);
    virtual void Draw() = 0;

    const std::vector<vc::Mesh *> & GetMeshes() const;

protected:
    std::vector<vc::Mesh *> __meshes;
    std::vector<vc::Material *> __materials;
};

/// @brief Contains all the mesh's data and is the
/// main high-level interface for the user
class VENOM_COMMON_API Model : public GraphicsPluginObject
{
protected:
    Model();
public:
    static Model * Create(const std::string & path);
    ~Model() override;

    void Draw() { _impl->As<ModelImpl>()->Draw(); }
    inline vc::Error ImportModel(const std::string & path) { return _impl->As<ModelImpl>()->ImportModel(path); }
    inline const std::vector<vc::Mesh *> & GetMeshes() const { return _impl->As<ModelImpl>()->GetMeshes(); }
};


}
}