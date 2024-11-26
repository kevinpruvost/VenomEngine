///
/// Project: VenomEngine
/// @file Material.h
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPluginObject.h>
#include <venom/common/plugin/graphics/MaterialComponent.h>

#include <venom/common/math/Matrix.h>

namespace venom
{
namespace common
{
class Model;

class VENOM_COMMON_API MaterialImpl : public PluginObjectImpl, public GraphicsPluginObject
{
public:
    MaterialImpl();
    virtual ~MaterialImpl();

    void SetComponent(const MaterialComponentType type, const MaterialComponent & comp);
    void SetComponent(const MaterialComponentType type, const vcm::Vec3& value);
    void SetComponent(const MaterialComponentType type, const vcm::Vec4& value);
    void SetComponent(const MaterialComponentType type, const float value);
    void SetComponent(const MaterialComponentType type, const Texture & texture);
    void SetComponentChannels(const MaterialComponentType type, const MaterialComponentValueChannels channels);
    void SetComponentChannelsFromIndex(const MaterialComponentType type, const int index);
    const MaterialComponent & GetComponent(const MaterialComponentType type) const;
    const std::string & GetName() const;
    void SetName(const std::string & name);
    const vcm::Vec2 & GetTextureRepeatFactor() const;
    void SetTextureRepeatFactor(const vcm::Vec2 & factor);

    /**
     * Material Component Resource Table (size: 32 bytes)
     */
    struct VENOM_COMMON_API MaterialComponentResourceTable {
        MaterialComponentResourceTable();
        vcm::Vec4 value; // 16 bytes
        int valueType; // 4 bytes
        int channels; // 4 bytes
        char padding[8]; // 8 bytes
    };
    struct VENOM_COMMON_API MaterialResourceTable {
        MaterialComponentResourceTable components[MaterialComponentType::MAX_COMPONENT];
        vcm::Vec2 textureRepeatingFactor;
    };
protected:
    const MaterialResourceTable & _GetResourceTable(bool & wasDirty);

private:
    MaterialComponent __components[MaterialComponentType::MAX_COMPONENT];
    vcm::Vec2 __textureRepeatingFactor;
    MaterialResourceTable __resourceTable;
    bool __resourceTableDirty;
    std::string __name;
};

class VENOM_COMMON_API Material : public PluginObjectImplWrapper
{
public:
    Material();
    ~Material();
public:
    inline void SetComponent(const MaterialComponentType type, const MaterialComponent & comp) {
        _impl->As<MaterialImpl>()->SetComponent(type, comp);
    }
    inline void SetComponent(const MaterialComponentType type, const vcm::Vec3& value) {
        _impl->As<MaterialImpl>()->SetComponent(type, value);
    }
    inline void SetComponent(const MaterialComponentType type, const vcm::Vec4& value) {
        _impl->As<MaterialImpl>()->SetComponent(type, value);
    }
    inline void SetComponent(const MaterialComponentType type, const float value) {
        _impl->As<MaterialImpl>()->SetComponent(type, value);
    }
    inline void SetComponent(const MaterialComponentType type, const Texture & texture) {
        _impl->As<MaterialImpl>()->SetComponent(type, texture);
    }
    inline const MaterialComponent & GetComponent(const MaterialComponentType type) const {
        return _impl->As<MaterialImpl>()->GetComponent(type);
    }
    inline const std::string & GetName() const {
        return _impl->As<MaterialImpl>()->GetName();
    }
    inline void SetName(const std::string & name) {
        _impl->As<MaterialImpl>()->SetName(name);
    }
    inline const vcm::Vec2 & GetTextureRepeatFactor() const {
        return _impl->As<MaterialImpl>()->GetTextureRepeatFactor();
    }
    inline void SetTextureRepeatFactor(const vcm::Vec2 & factor) {
        _impl->As<MaterialImpl>()->SetTextureRepeatFactor(factor);
    }
};

}
}