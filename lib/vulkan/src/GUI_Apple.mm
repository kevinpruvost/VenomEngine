///
/// Project: VenomEngineWorkspace
/// @file GUI_Apple.cc
/// @date Mar, 15 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/plugin/graphics/GUI.h>
#include <venom/common/context/ContextApple.h>

#if defined(VENOM_PLATFORM_MACOS)
#include "imgui_impl_osx.h"
#endif

namespace venom
{
namespace vulkan
{
vc::Error VulkanGUI::_InitializeApple()
{
#if defined(VENOM_PLATFORM_MACOS)
    if (!ImGui_ImplOSX_Init(venom::context::apple::ContextApple::GetAppleContext()->GetAppleView())) {
        vc::Log::Error("Failed to initialize ImGui for Apple.");
        return vc::Error::Failure;
    }
#endif
    return vc::Error::Success;
}

vc::Error VulkanGUI::_NewFrameApple()
{
#if defined(VENOM_PLATFORM_MACOS)
    ImGui_ImplOSX_NewFrame(venom::context::apple::ContextApple::GetAppleContext()->GetAppleView());
#endif
    return vc::Error::Success;
}

vc::Error VulkanGUI::_DestroyApple()
{
#if defined(VENOM_PLATFORM_MACOS)
    ImGui_ImplOSX_Shutdown();
#endif
    return vc::Error::Success;
}
}
}
