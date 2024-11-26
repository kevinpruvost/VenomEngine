///
/// Project: VenomEngine
/// @file Material.cc
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/Material.h>

#include <venom/common/Log.h>

namespace venom
{
namespace common
{
MaterialImpl::MaterialImpl()
    : __components{
    MaterialComponentType::AMBIENT,
    MaterialComponentType::DIFFUSE,
    MaterialComponentType::SPECULAR,
    MaterialComponentType::EMISSIVE,
    MaterialComponentType::SHININESS,
    MaterialComponentType::OPACITY,
    MaterialComponentType::NORMAL,
    MaterialComponentType::HEIGHT,
    MaterialComponentType::REFLECTION,
    MaterialComponentType::REFLECTIVITY,
    MaterialComponentType::REFRACTION,
    MaterialComponentType::TRANSPARENT,
    MaterialComponentType::ANISOTROPY,
    MaterialComponentType::BASE_COLOR,
    MaterialComponentType::METALLIC,
    MaterialComponentType::ROUGHNESS,
    MaterialComponentType::AMBIENT_OCCLUSION,
    MaterialComponentType::EMISSION_COLOR,
    MaterialComponentType::TRANSMISSION,
    MaterialComponentType::SHEEN,
    MaterialComponentType::CLEARCOAT
    }
    , __textureRepeatingFactor(1.0f, 1.0f)
    , __resourceTableDirty(true)
{
}

MaterialImpl::~MaterialImpl()
{
}

void MaterialImpl::SetComponent(const MaterialComponentType type, const MaterialComponent& comp)
{
    __components[type] = comp;
}

Material::Material()
    : PluginObjectImplWrapper(GraphicsPlugin::Get()->CreateMaterial())
{
}

Material::~Material()
{
}

void MaterialImpl::SetComponent(const MaterialComponentType type, const vcm::Vec3& value)
{
    __components[type].SetValue(value);
    __resourceTableDirty = true;
}

void MaterialImpl::SetComponent(const MaterialComponentType type, const vcm::Vec4& value)
{
    __components[type].SetValue(value);
    __resourceTableDirty = true;
}

void MaterialImpl::SetComponent(const MaterialComponentType type, const float value)
{
    __components[type].SetValue(value);
    __resourceTableDirty = true;
}

void MaterialImpl::SetComponent(const MaterialComponentType type, const Texture & texture)
{
    __components[type].SetValue(texture);
    __resourceTableDirty = true;
}

void MaterialImpl::SetComponentChannels(const MaterialComponentType type, const MaterialComponentValueChannels channels)
{
    __components[type].SetChannels(channels);
    __resourceTableDirty = true;
}

void MaterialImpl::SetComponentChannelsFromIndex(const MaterialComponentType type, const int index)
{
    __components[type].SetChannelsFromIndex(index);
    __resourceTableDirty = true;
}

const MaterialComponent& MaterialImpl::GetComponent(const MaterialComponentType type) const
{
    venom_assert(type < MaterialComponentType::MAX_COMPONENT, "MaterialComponentType out of range");
    return __components[type];
}

const std::string& MaterialImpl::GetName() const
{
    return __name;
}

void MaterialImpl::SetName(const std::string& name)
{
    __name = name;
    __resourceTableDirty = true;
}

const vcm::Vec2& MaterialImpl::GetTextureRepeatFactor() const
{
    return __textureRepeatingFactor;
}

void MaterialImpl::SetTextureRepeatFactor(const vcm::Vec2& factor)
{
    __textureRepeatingFactor = factor;
    __resourceTableDirty = true;
}

MaterialImpl::MaterialComponentResourceTable::MaterialComponentResourceTable()
    : value(vcm::Vec4(0.0f, 0.0f, 0.0f, 1.0f))
    , valueType(MaterialComponentValueType::NONE)
{
}

const MaterialImpl::MaterialResourceTable& MaterialImpl::_GetResourceTable(bool& wasDirty)
{
    wasDirty = __resourceTableDirty;
    if (__resourceTableDirty)
    {
        for (int i = 0; i < MaterialComponentType::MAX_COMPONENT; i++)
        {
            __resourceTable.components[i].valueType = static_cast<int>(__components[i].GetValueType());
            switch (__resourceTable.components[i].valueType) {
                case MaterialComponentValueType::COLOR3D:
                    memcpy(&__resourceTable.components[i].value, &__components[i].GetColor3D(), sizeof(vcm::Vec3));
                    break;
                case MaterialComponentValueType::COLOR4D:
                    __resourceTable.components[i].value = __components[i].GetColor4D();
                    break;
                case MaterialComponentValueType::VALUE:
                    float val = __components[i].GetFloatValue();
                    memcpy(&__resourceTable.components[i].value, &val, sizeof(float));
                    break;
            }
            __resourceTable.components[i].channels = static_cast<int>(__components[i].GetChannels());
            __resourceTable.textureRepeatingFactor = __textureRepeatingFactor;
        }
        __resourceTableDirty = false;
    }
    return __resourceTable;
}

}
}
