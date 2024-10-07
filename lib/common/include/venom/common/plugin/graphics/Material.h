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

class VENOM_COMMON_API Material : public GraphicsPluginObject
{
protected:
    Material();
public:
    ~Material();

    static Material * Create();
public:
    void SetComponent(const MaterialComponentType type, const vcm::Vec3& value);
    void SetComponent(const MaterialComponentType type, const vcm::Vec4& value);
    void SetComponent(const MaterialComponentType type, const float value);
    void SetComponent(const MaterialComponentType type, const Texture* texture);
    const MaterialComponent & GetComponent(const MaterialComponentType type) const;
    const std::string & GetName() const;
    void SetName(const std::string & name);
private:
    MaterialComponent __components[MaterialComponentType::MAX_COMPONENT];
    std::string __name;
};

}
}