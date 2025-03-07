///
/// Project: VenomEngineWorkspace
/// @file Light.h
/// @date Jan, 12 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/Light.h>
#include <venom/common/DeferredTrash.h>

namespace venom
{
namespace metal
{
class MetalLight : public vc::LightImpl
{
public:
    MetalLight();
    virtual ~MetalLight();

protected:
    vc::Error _SetType(const vc::LightType type) override;
    void _SetDescriptorsFromCascade(const int cascadeIndex) override;

private:
    vc::Array<int, VENOM_MAX_FRAMES_IN_FLIGHT> __lastCascades;
};
}
}
