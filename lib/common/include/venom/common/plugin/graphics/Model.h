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

#include <venom/common/ECS.h>

namespace venom
{
namespace common
{
class Model;

class VENOM_COMMON_API ModelResource : public GraphicsCachedResource
{
public:
    vc::Vector<vc::Mesh> meshes;
    vc::Vector<vc::Material> materials;
};

class VENOM_COMMON_API ModelImpl : public PluginObjectImpl, public GraphicsPluginObject, public GraphicsCachedResourceHolder
{
public:
    ModelImpl();
    virtual ~ModelImpl() = default;

    vc::Error ImportModel(const char * path);
    virtual void Draw() = 0;

    const vc::Vector<vc::Mesh> & GetMeshes() const;
    const vc::Vector<vc::Material> & GetMaterials() const;
    vc::Vector<vc::Material> & GetMaterials();

private:
    friend class Model;
};

/// @brief Contains all the mesh's data and is the
/// main high-level interface for the user
class VENOM_COMMON_API Model : public PluginObjectImplWrapper, public Component
{
public:
    Model();
    Model(const char * path);
    ~Model();

    void _GUI() override;
    vc::String _GetComponentTitle() override;
    inline void Draw() { _impl->As<ModelImpl>()->Draw(); }
    inline vc::Error ImportModel(const char * path) { return _impl->As<ModelImpl>()->ImportModel(path); }
    inline const vc::Vector<vc::Mesh> & GetMeshes() const { return _impl->As<ModelImpl>()->GetMeshes(); }
    inline const vc::Vector<vc::Material> & GetMaterials() const { return _impl->As<ModelImpl>()->GetMaterials(); }
    inline vc::Vector<vc::Material> & GetMaterials() { return _impl->As<ModelImpl>()->GetMaterials(); }
    inline const vc::String & GetName() { return _impl->As<ModelImpl>()->GetResourceName(); }
    inline const vc::String & GetShortName() { return _impl->As<TextureImpl>()->GetResourceShortName(); }
};


}
}
