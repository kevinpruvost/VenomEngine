///
/// Project: VenomEngine
/// @file Mesh.h
/// @date Aug, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/math/Vector.h>
#include <venom/common/plugin/graphics/GraphicsPlugin.h>

#include <vector>

namespace venom
{
namespace common
{
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

protected:
    std::vector<vc::VertexPos> __positions;
    std::vector<vc::VertexNormal> __normals;
    std::vector<vc::VertexColor> __colors;
    std::vector<vc::VertexUV> __uvs;
    std::vector<uint32_t> __indices;
};


}
}