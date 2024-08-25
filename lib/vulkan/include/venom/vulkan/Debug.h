///
/// Project: VenomEngine
/// @file Debug.h
/// @date Aug, 19 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Instance.h>

namespace venom
{
namespace vulkan
{
class DebugApplication : public Instance
{
public:
    DebugApplication();
    ~DebugApplication();

    vc::Error InitDebug();
    void DestroyDebug();

protected:
    void _SetInstanceCreateInfoValidationLayers(VkInstanceCreateInfo * createInfos);
    template<typename CreateInfo>
    void _SetCreateInfoValidationLayers(CreateInfo * createInfos);
    vc::Error _PostInstance_SetDebugParameters();

private:
    vc::Error __InitValidationLayers();

#ifdef VENOM_DEBUG
    std::vector<const char *>      __validationLayersInUse;
    std::vector<VkLayerProperties> __validationLayersAvailable;

    VkDebugUtilsMessengerEXT __debugMessenger;
    VkDebugUtilsMessengerCreateInfoEXT __debugMessengerCreateInfo;
#endif
};

}
}