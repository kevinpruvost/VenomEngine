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
    std::vector<vcm::VertexPos> __positions;
    std::vector<vcm::VertexNormal> __normals;
    std::vector<vcm::VertexColor> __colors;
    std::vector<vcm::VertexUV> __uvs;
    std::vector<uint32_t> __indices;
};


}
}