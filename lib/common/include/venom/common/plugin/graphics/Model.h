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

#include <vector>

namespace venom
{
namespace common
{
/// @brief Contains all the mesh's data and is the
/// main high-level interface for the user
class VENOM_COMMON_API Model : public GraphicsPluginObject
{
protected:
    Model();
public:
    static Model * Create();
    ~Model();

    virtual void Draw() = 0;
    vc::Error ImportModel(const std::string & path);
    //virtual vc::Error __ImportMesh() = 0;

protected:
    std::vector<vc::Mesh *> __meshes;
};


}
}