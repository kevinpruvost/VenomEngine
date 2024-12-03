///
/// Project: VenomEngineWorkspace
/// @file GUI.cc
/// @date Nov, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <imgui_internal.h>
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
    ImGui_ImplVulkan_Shutdown();
}

void VulkanGUI::__SetStyle()
{
    auto _style = &ImGui::GetStyle();

    //_style->WindowPadding = ImVec2(15, 15);
    //_style->WindowRounding = 5.0f;
    //_style->FramePadding = ImVec2(5, 5);
    //_style->FrameRounding = 4.0f;
    //_style->ItemSpacing = ImVec2(12, 8);
    //_style->ItemInnerSpacing = ImVec2(8, 6);
    //_style->IndentSpacing = 25.0f;
    //_style->ScrollbarSize = 15.0f;
    //_style->ScrollbarRounding = 9.0f;
    //_style->GrabMinSize = 5.0f;
    //_style->GrabRounding = 3.0f;

    //_style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    //_style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    //_style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //_style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    //_style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    //_style->Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    //_style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    //_style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    //_style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    //_style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    //_style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    //_style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    //_style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    //_style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    //_style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    //_style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    //_style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    //_style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //_style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    //_style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    //_style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //_style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    //_style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    //_style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    //_style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    //_style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    //_style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //_style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    //_style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    //_style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //_style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    //_style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    //_style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    //_style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    //_style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

    _style->WindowMinSize = ImVec2(160, 20);
    _style->FramePadding = ImVec2(4, 2);
    _style->ItemSpacing = ImVec2(6, 2);
    _style->ItemInnerSpacing = ImVec2(6, 4);
    _style->Alpha = 1.0f;
    _style->WindowRounding = 4.0f;
    _style->FrameRounding = 2.0f;
    _style->IndentSpacing = 6.0f;
    _style->ItemInnerSpacing = ImVec2(2, 4);
    _style->ColumnsMinSpacing = 50.0f;
    _style->GrabMinSize = 14.0f;
    _style->GrabRounding = 16.0f;
    _style->ScrollbarSize = 12.0f;
    _style->ScrollbarRounding = 16.0f;
    _style->Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
    _style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
    _style->Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    _style->Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
    _style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    _style->Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    _style->Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    _style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
    _style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
    _style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    _style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    _style->Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
    _style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    _style->Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    _style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    _style->Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);

    // Colored ones
    _style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.59f, 0.18f, 0.78f);
    _style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.12f, 0.59f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.59f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.12f, 0.59f, 0.18f, 0.78f);
    _style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.12f, 0.59f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_CheckMark] = ImVec4(0.14f, 0.45f, 0.17f, 1.00f);
    _style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.09f, 0.53f, 0.30f, 0.14f);
    _style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.12f, 0.59f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_Button] = ImVec4(0.09f, 0.53f, 0.30f, 0.14f);
    _style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.12f, 0.59f, 0.18f, 0.86f);
    _style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.12f, 0.59f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_Header] = ImVec4(0.12f, 0.59f, 0.18f, 0.76f);
    _style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.12f, 0.59f, 0.18f, 0.86f);
    _style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.12f, 0.59f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.12f, 0.59f, 0.18f, 0.78f);
    _style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.12f, 0.59f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.12f, 0.59f, 0.18f, 0.78f);
    _style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.12f, 0.59f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.12f, 0.59f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.12f, 0.59f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.12f, 0.59f, 0.18f, 0.43f);
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
    initInfo.RenderPass = app->GetGuiRenderPass()->GetVkRenderPass();
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

    __SetStyle();
    return vc::Error::Success;
}

vc::Error VulkanGUI::Reset()
{
    const VulkanApplication * const app = static_cast<const VulkanApplication * const>(_app);

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    // Setup Platform/Renderer backends

    ImGui_ImplGlfw_InitForVulkan(vc::Context::Get()->GetWindow(), true);
    initInfo.Instance = Instance::GetVkInstance();
    initInfo.PhysicalDevice = PhysicalDevice::GetUsedVkPhysicalDevice();
    initInfo.Device = LogicalDevice::GetInstance().GetVkDevice();
    initInfo.QueueFamily = QueueManager::GetGraphicsQueue().GetQueueFamilyIndex();
    initInfo.Queue = QueueManager::GetGraphicsQueue().GetVkQueue();
    //initInfo.PipelineCache = g_PipelineCache;
    initInfo.DescriptorPool = app->GetDescriptorPool()->GetVkDescriptorPool();
    initInfo.RenderPass = app->GetGuiRenderPass()->GetVkRenderPass();
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
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(vc::Context::GetWindowWidth()), static_cast<float>(vc::Context::GetWindowHeight()));

    return vc::Error::Success;
}

void VulkanGUI::_SetNextWindowPos(const vcm::Vec2& pos, vc::GUICond cond, const vcm::Vec2& pivot)
{
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y), static_cast<ImGuiCond>(cond), ImVec2(pivot.x, pivot.y));
}

const vcm::Vec2 & VulkanGUI::_GetWindowSize()
{
    const auto& size = ImGui::GetWindowSize();
    return {size.x, size.y};
}

const vcm::Vec2 & VulkanGUI::_GetWindowPos()
{
    const auto& pos = ImGui::GetWindowPos();
    return {pos.x, pos.y};
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

bool VulkanGUI::_TreeNode(const char* label)
{
    return ImGui::TreeNode(label);
}

void VulkanGUI::_TreePop()
{
    ImGui::TreePop();
}

void VulkanGUI::_TreePush(const char* str_id)
{
    ImGui::TreePush(str_id);
}

void VulkanGUI::_Spacing()
{
    ImGui::Spacing();
}

bool VulkanGUI::_SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float power)
{
    return ImGui::SliderFloat(label, v, v_min, v_max, format, power);
}

bool VulkanGUI::_SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format, float power)
{
    return ImGui::SliderFloat3(label, v, v_min, v_max, format, power);
}

void VulkanGUI::_ColorEdit3(const char* label, float col[3], vc::GUIColorEditFlags flags)
{
    ImGui::ColorEdit3(label, col, static_cast<ImGuiColorEditFlags>(flags));
}

bool VulkanGUI::_CollapsingHeader(const char* label, vc::GUITreeNodeFlags flags)
{
    return ImGui::CollapsingHeader(label, static_cast<ImGuiTreeNodeFlags>(flags));
}

bool VulkanGUI::_Button(const char* label, const vcm::Vec2& size)
{
    return ImGui::Button(label, ImVec2(size.x, size.y));
}

bool VulkanGUI::_Checkbox(const char* label, bool* v)
{
    return ImGui::Checkbox(label, v);
}

void VulkanGUI::_ProgressBar(float fraction, const vcm::Vec2& size_arg, const char* overlay)
{
    ImGui::ProgressBar(fraction, ImVec2(size_arg.x, size_arg.y), overlay);
}

bool VulkanGUI::_Selectable(const char* label, bool selected, vc::GUISelectableFlags flags, const vcm::Vec2& size)
{
    return ImGui::Selectable(label, selected, static_cast<ImGuiSelectableFlags>(flags), ImVec2(size.x, size.y));
}

bool VulkanGUI::_BeginCombo(const char* label, const char* preview_value, vc::GUIComboFlags flags)
{
    return ImGui::BeginCombo(label, preview_value, static_cast<ImGuiComboFlags>(flags));
}

void VulkanGUI::_EndCombo()
{
    ImGui::EndCombo();
}

bool VulkanGUI::_BeginMenu(const char* label, bool enabled)
{
    return ImGui::BeginMenu(label, enabled);
}

void VulkanGUI::_EndMenu()
{
    ImGui::EndMenu();
}

bool VulkanGUI::_BeginMainMenuBar()
{
    return ImGui::BeginMainMenuBar();
}

void VulkanGUI::_EndMainMenuBar()
{
    ImGui::EndMainMenuBar();
}

bool VulkanGUI::_BeginMenuBar()
{
    return ImGui::BeginMenuBar();
}

void VulkanGUI::_EndMenuBar()
{
    ImGui::EndMenuBar();
}

bool VulkanGUI::_MenuItem(const char* str, const char* text)
{
    return ImGui::MenuItem(str, text);
}

void VulkanGUI::_SetItemDefaultFocus()
{
    ImGui::SetItemDefaultFocus();
}

void VulkanGUI::_SameLine(float offset_from_start_x, float spacing)
{
    ImGui::SameLine(offset_from_start_x, spacing);
}

vc::Error VulkanGUI::_PreUpdate()
{
    return vc::Error::Success;
}

void VulkanGUI::_Render()
{
    const VulkanApplication * const app = static_cast<const VulkanApplication * const>(_app);

    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(draw_data, app->GetCurrentGraphicsCommandBuffer()->GetVkCommandBuffer());
}

}
}