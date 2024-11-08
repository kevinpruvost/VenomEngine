///
/// Project: VenomEngineWorkspace
/// @file GUI.cc
/// @date Nov, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Allocator.h>
#include <venom/vulkan/LogicalDevice.h>
#include <venom/vulkan/plugin/graphics/GUI.h>

#include <venom/vulkan/VulkanApplication.h>

namespace venom
{
namespace vulkan
{
VulkanGUI::VulkanGUI()
    : initInfo{}
{
}

VulkanGUI::~VulkanGUI()
{
    ImGui::NewFrame();
    ImGui_ImplVulkan_Shutdown();
}

vc::Error VulkanGUI::Initialize()
{
    const VulkanApplication * const app = static_cast<const VulkanApplication * const>(_app);

    // Setup Platform/Renderer backends
    ImGui::CreateContext();

    ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForVulkan(vc::Context::Get()->GetWindow(), true);
    initInfo.Instance = Instance::GetVkInstance();
    initInfo.PhysicalDevice = PhysicalDevice::GetUsedVkPhysicalDevice();
    initInfo.Device = LogicalDevice::GetInstance().GetVkDevice();
    initInfo.QueueFamily = QueueManager::GetGraphicsQueue().GetQueueFamilyIndex();
    initInfo.Queue = QueueManager::GetGraphicsQueue().GetVkQueue();
    //initInfo.PipelineCache = g_PipelineCache;
    initInfo.DescriptorPool = app->GetDescriptorPool()->GetVkDescriptorPool();
    initInfo.RenderPass = app->GetRenderPass()->GetVkRenderPass();
    initInfo.Subpass = 0;
    initInfo.MinImageCount = app->GetSwapChain()->surface->GetCapabilities().minImageCount;
    initInfo.ImageCount = VENOM_MAX_FRAMES_IN_FLIGHT;
    initInfo.MSAASamples = static_cast<VkSampleCountFlagBits>(app->GetSwapChain()->GetSamples());
    initInfo.Allocator = Allocator::GetVKAllocationCallbacks();
#ifdef VENOM_DEBUG
    initInfo.CheckVkResultFn = [](VkResult err)
    {
        if (err != 0)
            vc::Log::Error("ImGui_ImplVulkan_Init failed with error code %d", err);
    };
#endif
    if (!ImGui_ImplVulkan_Init(&initInfo))
        return vc::Error::Failure;
    return vc::Error::Success;
}

vc::Error VulkanGUI::Reset()
{
    const VulkanApplication * const app = static_cast<const VulkanApplication * const>(_app);

    initInfo.Instance = Instance::GetVkInstance();
    initInfo.PhysicalDevice = PhysicalDevice::GetUsedVkPhysicalDevice();
    initInfo.Device = LogicalDevice::GetInstance().GetVkDevice();
    initInfo.QueueFamily = QueueManager::GetGraphicsQueue().GetQueueFamilyIndex();
    initInfo.Queue = QueueManager::GetGraphicsQueue().GetVkQueue();
    //initInfo.PipelineCache = g_PipelineCache;
    initInfo.DescriptorPool = app->GetDescriptorPool()->GetVkDescriptorPool();
    initInfo.RenderPass = app->GetRenderPass()->GetVkRenderPass();
    initInfo.Subpass = 0;
    initInfo.MinImageCount = app->GetSwapChain()->surface->GetCapabilities().minImageCount;
    initInfo.ImageCount = VENOM_MAX_FRAMES_IN_FLIGHT;
    initInfo.MSAASamples = static_cast<VkSampleCountFlagBits>(app->GetSwapChain()->GetSamples());
    initInfo.Allocator = Allocator::GetVKAllocationCallbacks();
#ifdef VENOM_DEBUG
    initInfo.CheckVkResultFn = [](VkResult err)
    {
        if (err != 0)
            vc::Log::Error("ImGui_ImplVulkan_Init failed with error code %d", err);
    };
#endif
    ImGui_ImplVulkan_Shutdown();
    if (!ImGui_ImplVulkan_Init(&initInfo))
        return vc::Error::Failure;
    return vc::Error::Success;
}

void VulkanGUI::_NewFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void VulkanGUI::_Begin(const char* name, bool* p_open, vc::GUIWindowFlags flags)
{
    ImGui::Begin(name, p_open, static_cast<ImGuiWindowFlags>(flags));
}

void VulkanGUI::_End()
{
    ImGui::End();
}

void VulkanGUI::_Text(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    ImGui::TextV(fmt, args);
    va_end(args);
}

void VulkanGUI::_TextColored(const vcm::Vec4& col, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    ImGui::TextColoredV(ImVec4(col.x, col.y, col.z, col.w), fmt, args);
    va_end(args);
}

void VulkanGUI::_LabelText(const char* label, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    ImGui::LabelTextV(label, fmt, args);
    va_end(args);
}

void VulkanGUI::_SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float power)
{
    ImGui::SliderFloat(label, v, v_min, v_max, format, power);
}

void VulkanGUI::_ColorEdit3(const char* label, float col[3], vc::GUIColorEditFlags flags)
{
    ImGui::ColorEdit3(label, col, static_cast<ImGuiColorEditFlags>(flags));
}

void VulkanGUI::_Button(const char* label, const vcm::Vec2& size)
{
    ImGui::Button(label, ImVec2(size.x, size.y));
}

void VulkanGUI::_Checkbox(const char* label, bool* v)
{
    ImGui::Checkbox(label, v);
}

void VulkanGUI::_ProgressBar(float fraction, const vcm::Vec2& size_arg, const char* overlay)
{
    ImGui::ProgressBar(fraction, ImVec2(size_arg.x, size_arg.y), overlay);
}

void VulkanGUI::_SameLine(float offset_from_start_x, float spacing)
{
    ImGui::SameLine(offset_from_start_x, spacing);
}

void VulkanGUI::_Render()
{
    const VulkanApplication * const app = static_cast<const VulkanApplication * const>(_app);

    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(draw_data, app->GetCurrentCommandBuffer()->GetVkCommandBuffer());
}

}
}