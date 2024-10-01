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

protected:
    friend class Model;
    std::vector<vcm::VertexPos> __positions;
    std::vector<vcm::VertexNormal> __normals;
    std::vector<vcm::VertexColor> __colors;
    std::vector<vcm::VertexUV> __uvs;
    std::vector<uint32_t> __indices;
};


}
}