///
/// Project: VenomEngine
/// @file Material.h
/// @date Oct, 02 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/Texture.h>
#include <venom/common/math/Matrix.h>

namespace venom
{
namespace common
{

/**
 * @brief MaterialComponentType
 * Contains all the possible components of a material that
 * VenomEngine can handle.
 */
enum MaterialComponentType
{
    AMBIENT,
    DIFFUSE,
    SPECULAR,
    EMISSIVE,
    SHININESS,
    OPACITY,
    NORMAL,
    HEIGHT,
    REFLECTION,
    REFLECTIVITY,
    REFRACTION,
    TRANSPARENT,
    ANISOTROPY,
    // PBR Materials
    BASE_COLOR,
    METALLIC,
    ROUGHNESS,
    AMBIENT_OCCLUSION,
    EMISSION_COLOR,
    TRANSMISSION,
    // For cloth
    SHEEN,
    CLEARCOAT,
    MAX_COMPONENT
};

/**
 * @brief MaterialComponentValueType
 * Contains all the possible value types of a material component.
 */
enum MaterialComponentValueType
{
    COLOR3D,
    COLOR4D,
    VALUE,
    TEXTURE,
    NONE
};

/**
 * @brief MaterialComponent
 * Contains all the data of a material component.
 */
class VENOM_COMMON_API MaterialComponent
{
public:
    MaterialComponent(const MaterialComponentType type);
    ~MaterialComponent();

    /**
     * @brief Sets Material component value
     * @param value can be a color3D/4D, a value or a texture
     */
    void SetValue(const vcm::Vec3 & value);
    /**
     * @brief Sets Material component value
     * @param value can be a color3D/4D, a value or a texture
     */
    void SetValue(const vcm::Vec4 & value);
    /**
     * @brief Sets Material component value
     * @param value can be a color3D/4D, a value or a texture
     */
    void SetValue(const float value);
    /**
     * @brief Sets Material component value
     * @param value can be a color3D/4D, a value or a texture
     */
    void SetValue(const Texture & texture);

    inline MaterialComponentType GetType() const { return __type; }
    inline MaterialComponentValueType GetValueType() const { return __valueType; }
    const Texture & GetTexture() const;
private:
    const MaterialComponentType __type;
    MaterialComponentValueType __valueType;
    std::variant<vcm::Vec3, vcm::Vec4, float, Texture> __value;
};



}
}