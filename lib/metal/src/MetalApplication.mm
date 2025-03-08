///
/// Project: VenomEngine
/// @file GraphicsApplication.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/MetalApplication.h>

#include <array>
#include <thread>
#include <vector>

#include <venom/metal/plugin/graphics/ShaderResourceTable.h>

#include <venom/common/FpsCounter.h>

#include <venom/metal/plugin/graphics/Texture.h>

#include <venom/common/VenomSettings.h>
#include <venom/metal/plugin/graphics/Light.h>
#include <venom/metal/plugin/graphics/Material.h>

#include "venom/common/ECS.h"
#include <venom/metal/plugin/graphics/Skybox.h>

#include "venom/common/plugin/graphics/Light.h"
#include "venom/common/SceneSettings.h"
#include "venom/common/plugin/graphics/GUI.h"
#include "venom/common/plugin/graphics/RenderingPipeline.h"
#include "venom/common/plugin/graphics/RenderTarget.h"

#include <venom/metal/Device.h>
#include <venom/metal/CommandQueue.h>
#include <venom/metal/Layer.h>

namespace venom::metal
{
MetalApplication::MetalApplication()
    : vc::GraphicsApplication()
    , DebugApplication()
    , __shouldClose(false)
{
    NSApplication * app = [NSApplication sharedApplication];
    InitializeDevice();
    InitializeCommandQueue();
    InitializeLayer();
}

MetalApplication::~MetalApplication()
{
    vc::Log::Print("Destroying Metal app...");
    DestroyLayer();
    DestroyCommandQueue();
    DestroyDevice();
#ifdef VENOM_DEBUG
#endif
    vc::Log::Print("Metal app succesfully destroyed.");
}

bool MetalApplication::ShouldClose() { return vc::Context::Get()->ShouldClose(); }

void MetalApplication::WaitForDraws()
{
}

vc::Error MetalApplication::__Init()
{
    return vc::Error::Success;
}

vc::Error MetalApplication::__PostInit()
{
    return vc::Error::Success;
}

vc::Error MetalApplication::__Loop()
{
    vc::Error err = vc::Error::Success;
    __shouldClose = vc::Context::Get()->ShouldClose();
    return err;
}

vc::Error MetalApplication::_LoadGfxSettings() {
    return vc::Error::Success;
}

vc::Error MetalApplication::_SetHDR(bool enable) {
    return vc::Error::Success;
}


vc::Vector<vc::GraphicsSettings::MultiSamplingCountOption> MetalApplication::_GetAvailableMultisamplingOptions() {
    return vc::Vector<vc::GraphicsSettings::MultiSamplingCountOption>();
}


vc::Error MetalApplication::_SetMultiSampling(const MultiSamplingModeOption mode, const MultiSamplingCountOption samples) {
    return vc::Error::Success;
}
}

