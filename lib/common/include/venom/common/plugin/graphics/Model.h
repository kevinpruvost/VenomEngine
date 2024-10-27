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
class Model;

class VENOM_COMMON_API ModelResource : public GraphicsCachedResource
{
public:
    std::vector<vc::Mesh> meshes;
    std::vector<vc::Material> materials;
};

class VENOM_COMMON_API ModelImpl : public PluginObjectImpl, public GraphicsPluginObject, public GraphicsCachedResourceHolder
{
public:
    ModelImpl();
    virtual ~ModelImpl() = default;

    vc::Error ImportModel(const char * path);
    virtual void Draw() = 0;

    const std::vector<vc::Mesh> & GetMeshes() const;

private:
    friend class Model;
};

/// @brief Contains all the mesh's data and is the
/// main high-level interface for the user
class VENOM_COMMON_API Model : public PluginObjectImplWrapper
{
public:
    Model();
    Model(const char * path);
    ~Model();

    inline void Draw() { _impl->As<ModelImpl>()->Draw(); }
    inline vc::Error ImportModel(const char * path) { return _impl->As<ModelImpl>()->ImportModel(path); }
    inline const std::vector<vc::Mesh> & GetMeshes() const { return _impl->As<ModelImpl>()->GetMeshes(); }
};


}
}