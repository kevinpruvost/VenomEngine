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
    IOR,
    MAX_COMPONENT
};

/**
 * @brief MaterialComponentValueType
 * Contains all the possible value types of a material component.
 */
enum MaterialComponentValueType
{
    NONE,
    FLOAT3D = 1,
    FLOAT4D = 2,
    FLOAT1D = 4,
    TEXTURE = 8,
};

enum MaterialComponentValueChannels
{
    R = 0b1,
    G = 0b10,
    B = 0b100,
    A = 0b1000,
    RG = R | G,
    GB = G | B,
    BA = B | A,
    RGB = R | G | B,
    RGBA = R | G | B | A
};

/**
 * @brief MaterialComponent
 * Contains all the data of a material component.
 */
class VENOM_COMMON_API MaterialComponent
{
public:
    typedef std::variant<vcm::Vec3, vcm::Vec4, float> ComponentValue;
    MaterialComponent(const MaterialComponentType type);
    MaterialComponent(const MaterialComponent & other);
    MaterialComponent(MaterialComponent && other) noexcept;
    MaterialComponent & operator=(const MaterialComponent & other);
    MaterialComponent & operator=(MaterialComponent && other) noexcept;
    ~MaterialComponent();

    /**
     * @brief Sets Material component value
     * @param value can be a color3D/4D, a value or a texture
     */
    inline void SetValue(const vcm::Vec3 & value)
    {
        __valueType = MaterialComponentValueType::FLOAT3D | (__valueType & MaterialComponentValueType::TEXTURE);
        __value.emplace<vcm::Vec3>(value);
    }
    /**
     * @brief Sets Material component value
     * @param value can be a color3D/4D, a value or a texture
     */
    inline void SetValue(const vcm::Vec4 & value)
    {
        __valueType = MaterialComponentValueType::FLOAT4D | (__valueType & MaterialComponentValueType::TEXTURE);
        __value.emplace<vcm::Vec4>(value);
    }
    /**
     * @brief Sets Material component value
     * @param value can be a color3D/4D, a value or a texture
     */
    inline void SetValue(const float value)
    {
        __valueType = MaterialComponentValueType::FLOAT1D | (__valueType & MaterialComponentValueType::TEXTURE);
        __value.emplace<float>(value);
    }
    /**
     * @brief Removes the value of the material component
     */
    inline void RemoveValue()
    {
        __value = ComponentValue();
        __valueType = __valueType & MaterialComponentValueType::TEXTURE;
    }
    /**
     * @brief Sets Material component value
     * @param value can be a color3D/4D, a value or a texture
     */
    inline void SetTexture(const Texture & texture)
    {
        __texture.reset(new Texture(texture));
        __channels = MaterialComponentValueChannels::RGBA;
        __valueType |= MaterialComponentValueType::TEXTURE;
    }
    /**
     * @brief Removes the texture value of the material component
     */
    inline void RemoveTexture()
    {
        __texture.reset();
        __valueType &= ~MaterialComponentValueType::TEXTURE;
    }
    /**
     * @brief Gets the texture value of the material component
     * @return Texture
     */
    inline const Texture * GetTexture() const { return __texture.get(); }
    /**
     * @brief Gets the color value of the material component
     * @return Vec4
     */
    inline const vcm::Vec4 & GetColor4D() const { return std::get<vcm::Vec4>(__value); }
    /**
     * @brief Gets the color value of the material component
     * @return Vec3
     */
    inline const vcm::Vec3 & GetColor3D() const { return std::get<vcm::Vec3>(__value); }
    /**
     * @brief Gets the value of the material component
     * @return float
     */
    inline float GetFloatValue() const { return std::get<float>(__value); }
    inline ComponentValue & GetValueRef() { return __value; }
    inline const ComponentValue & GetValueRef() const { return __value; }

    inline void SetChannels(const MaterialComponentValueChannels channels) { __channels = channels; }
    inline void SetChannelsFromIndex(const int index) { __channels = GetComponentValueChannelsFromIndex(index); }

    inline bool HasTexture() const { return __valueType & MaterialComponentValueType::TEXTURE; }

    inline int GetValueType() const { return __valueType; }
    inline int & GetValueTypeRef() { return __valueType; }
    inline MaterialComponentValueChannels GetChannels() const { return __channels; }
    inline MaterialComponentValueChannels & GetChannelsRef() { return __channels; }

    static inline constexpr vc::Array<const char *, 9> GetChannelsViews()
    {
        constexpr vc::Array<const char *, 9> channels = {
            "R",
            "G",
            "B",
            "A",
            "RG",
            "GB",
            "BA",
            "RGB",
            "RGBA"
        };
        return channels;
    }
    static inline MaterialComponentValueChannels GetComponentValueChannelsFromIndex(const int index)
    {
        switch (index)
        {
        case 0:
            return MaterialComponentValueChannels::R;
        case 1:
            return MaterialComponentValueChannels::G;
        case 2:
            return MaterialComponentValueChannels::B;
        case 3:
            return MaterialComponentValueChannels::A;
        case 4:
            return MaterialComponentValueChannels::RG;
        case 5:
            return MaterialComponentValueChannels::GB;
        case 6:
            return MaterialComponentValueChannels::BA;
        case 7:
            return MaterialComponentValueChannels::RGB;
        case 8:
            return MaterialComponentValueChannels::RGBA;
        default:
            return MaterialComponentValueChannels::RGBA;
        }
    }
private:
    int __valueType;
    MaterialComponentValueChannels __channels;
    UPtr<Texture> __texture;
    ComponentValue __value;
};



}
}