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
    NONE,
    COLOR3D,
    COLOR4D,
    VALUE,
    TEXTURE,
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
    inline void SetValue(const vcm::Vec3 & value)
    {
        __valueType = MaterialComponentValueType::COLOR3D;
        __value.emplace<vcm::Vec3>(value);
    }
    /**
     * @brief Sets Material component value
     * @param value can be a color3D/4D, a value or a texture
     */
    inline void SetValue(const vcm::Vec4 & value)
    {
        __valueType = MaterialComponentValueType::COLOR4D;
        __value.emplace<vcm::Vec4>(value);
    }
    /**
     * @brief Sets Material component value
     * @param value can be a color3D/4D, a value or a texture
     */
    inline void SetValue(const float value)
    {
        __valueType = MaterialComponentValueType::VALUE;
        __value.emplace<float>(value);
    }
    /**
     * @brief Sets Material component value
     * @param value can be a color3D/4D, a value or a texture
     */
    inline void SetValue(const Texture & texture)
    {
        __value.emplace<Texture>(texture);
        __valueType = MaterialComponentValueType::TEXTURE;
    }
    /**
     * @brief Gets the texture value of the material component
     * @return Texture
     */
    inline const Texture & GetTexture() const { return std::get<Texture>(__value); }
    inline const vcm::Vec4 & GetColor4D() const { return std::get<vcm::Vec4>(__value); }
    inline const vcm::Vec3 & GetColor3D() const { return std::get<vcm::Vec3>(__value); }
    inline float GetValue() const { return std::get<float>(__value); }

    inline MaterialComponentValueType GetValueType() const { return __valueType; }
private:
    MaterialComponentValueType __valueType;
    std::variant<vcm::Vec3, vcm::Vec4, float, Texture> __value;
};



}
}