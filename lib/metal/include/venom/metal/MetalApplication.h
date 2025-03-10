///
/// Project: VenomEngine
/// @file GraphicsApplication.h
/// @date Aug, 18 2024
/// @brief Metal Encapsulation of the MetalApplication class.
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#ifdef __OBJC__
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>
#import <Foundation/Foundation.h>
#endif

#include <venom/metal/Debug.h>

#include <venom/common/plugin/graphics/GraphicsApplication.h>
#include <venom/common/Context.h>

#include <venom/common/VenomSettings.h>
#include <venom/common/math/Vector.h>

#include "venom/common/Thread.h"
#include "venom/common/plugin/graphics/Light.h"

namespace venom
{
/// @brief Encapsulation of Metal for the front end of VenomEngine.
namespace metal
{
class MetalShaderResourceTable;
class MetalLight;

class MetalApplication
    : public vc::GraphicsApplication
    , public DebugApplication
{
public:
    MetalApplication();
    ~MetalApplication() override;
    vc::Error __Init() override;
    vc::Error __PostInit() override;
    vc::Error __Loop() override;
    bool ShouldClose() override;
public:
    void WaitForDraws() override;

protected:
    vc::Error _OnGfxSettingsChange() override;
    vc::Error _OnGfxConstantsChange() override;

    vc::Error _SetMultiSampling(const MultiSamplingModeOption mode, const MultiSamplingCountOption samples) override;
    vc::Vector<MultiSamplingCountOption> _GetAvailableMultisamplingOptions() override;

    vc::Error _SetHDR(bool enable) override;

private:
    int __shadowMapIndices[VENOM_MAX_LIGHTS];
    vcm::Mat4 __shadowMapDirectionalLightSpaceMatrices[VENOM_CSM_TOTAL_CASCADES * VENOM_MAX_DIRECTIONAL_LIGHTS];
    vcm::Mat4 __shadowMapPointLightSpaceMatrices[6 * VENOM_MAX_POINT_LIGHTS];
    vcm::Mat4 __shadowMapSpotLightSpaceMatrices[VENOM_MAX_SPOT_LIGHTS];

    bool __shouldClose;

#ifdef __OBJC__
public:

private:
#endif
    friend class MetalShaderResourceTable;
    friend class MetalLight;
};
}
}
