///
/// Project: VenomEngine
/// @file Material.h
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPlugin.h>

#include <venom/common/math/Matrix.h>

namespace venom
{
namespace common
{
class Model;

class VENOM_COMMON_API Material : public GraphicsPluginObject
{
protected:
    Material();
public:
    ~Material();

    static Material * Create();

public:
    // Material properties
    vcm::Vec3 ambientColor;   // Ka
    vcm::Vec3 diffuseColor;   // Kd
    vcm::Vec3 specularColor;  // Ks
    vcm::Vec3 emissiveColor;  // Ke
    float shininess;          // Ns (Specular exponent)
    float opacity;            // d (Alpha value)
    float refractionIndex;    // Ni
};

}
}