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
    _style->Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 0.00f, 0.00f, 0.00f);
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

    _style->Colors[ImGuiCol_Tab] = ImVec4(0.0f, 0.3f, 0.0f, 1.0f);           // Dark green for an inactive tab
    _style->Colors[ImGuiCol_TabHovered] = ImVec4(0.0f, 0.5f, 0.0f, 1.0f);    // Brighter green when the tab is hovered
    _style->Colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.4f, 0.0f, 1.0f);     // Brighter green for the active tab
    _style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.3f, 0.0f, 0.8f);  // Dark green for an unfocused inactive tab
    _style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.4f, 0.0f, 0.9f); // Green for an active tab in an unfocused window
    _style->Colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.0f, 0.4f, 0.0f, 0.9f); // Green for an active tab in an unfocused window
}

vc::Error VulkanGUI::Initialize()
{
    const VulkanApplication * const app = static_cast<const VulkanApplication * const>(_app);

    // Setup Platform/Renderer backends
    ImGui::CreateContext();

    ImGui::StyleColorsLight();

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
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    __SetStyle();
    return vc::Error::Success;
}

vc::Error VulkanGUI::Reset()
{
    const VulkanApplication * const app = static_cast<const VulkanApplication * const>(_app);

    vc::Texture::UnloadAllGuiTextures();
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    return Initialize();
}

void VulkanGUI::_SetNextWindowPos(const vcm::Vec2& pos, vc::GUICond cond, const vcm::Vec2& pivot)
{
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y), static_cast<ImGuiCond>(cond), ImVec2(pivot.x, pivot.y));
}

void VulkanGUI::_SetNextWindowSize(const vcm::Vec2& size, vc::GUICond cond)
{
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y), static_cast<ImGuiCond>(cond));
}

void VulkanGUI::_SetNextWindowViewport(vc::GUIViewport viewport)
{
    ImGui::SetNextWindowViewport(reinterpret_cast<ImGuiViewport*>(viewport)->ID);
}

vcm::Vec2 VulkanGUI::_GetContentRegionAvail()
{
    const auto& size = ImGui::GetContentRegionAvail();
    return {size.x, size.y};
}

vcm::Vec2 VulkanGUI::_GetWindowSize()
{
    const auto& size = ImGui::GetWindowSize();
    return {size.x, size.y};
}

vcm::Vec2 VulkanGUI::_GetWindowPos()
{
    const auto& pos = ImGui::GetWindowPos();
    return {pos.x, pos.y};
}

vc::GUIViewport VulkanGUI::_GetMainViewport()
{
    return ImGui::GetMainViewport();
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

void VulkanGUI::_Image(vc::Texture* texture, const vcm::Vec2 & size)
{
    void * textureId;
    if (texture->GetGUITextureID(&textureId) != vc::Error::Success) {
        vc::Log::Error("Failed to get GUI texture ID");
        return;
    }
    // Remove padding from the window
    ImGui::Image(reinterpret_cast<ImTextureID>(textureId), ImVec2(size.x, size.y));
}

bool VulkanGUI::_InputText(const char* label, char* buf, size_t buf_size, vc::GUIInputTextFlags flags)
{
    return ImGui::InputText(label, buf, buf_size, static_cast<ImGuiInputTextFlags>(flags));
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

void VulkanGUI::_SeparatorText(const char* text)
{
    ImGui::SeparatorText(text);
}

void VulkanGUI::_Separator()
{
    ImGui::Separator();
}

void VulkanGUI::_Spacing()
{
    ImGui::Spacing();
}

bool VulkanGUI::_SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format)
{
    return ImGui::SliderFloat(label, v, v_min, v_max, format);
}

bool VulkanGUI::_SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format)
{
    return ImGui::SliderFloat3(label, v, v_min, v_max, format, ImGuiSliderFlags_Logarithmic);
}

bool VulkanGUI::_InputFloat(const char* label, float* v, float step, float step_fast, const char* format,
    vc::GUIColorEditFlags flags)
{
    return ImGui::InputFloat(label, v, step, step_fast, format, static_cast<ImGuiColorEditFlags>(flags));
}

bool VulkanGUI::_InputFloat3(const char* label, float v[3], const char* format, vc::GUIColorEditFlags flags)
{
    return ImGui::InputFloat3(label, v, format, static_cast<ImGuiColorEditFlags>(flags));
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

bool VulkanGUI::_BeginChild(const char* str_id, const vcm::Vec2& size, vc::GUIChildFlags childFlags, vc::GUIWindowFlags extra_flags)
{
    return ImGui::BeginChild(str_id, ImVec2(size.x, size.y), static_cast<ImGuiChildFlags>(childFlags),
        static_cast<ImGuiWindowFlags>(extra_flags));
}

void VulkanGUI::_EndChild()
{
    ImGui::EndChild();
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

void VulkanGUI::_PushWindowPadding(const vcm::Vec2& padding)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding.x, padding.y));
}

void VulkanGUI::_PopStyleVar()
{
    ImGui::PopStyleVar();
}

vc::GUIId VulkanGUI::_DockSpace(vc::GUIId id, const vcm::Vec2& size, vc::GUIDockNodeFlags flags)
{
    return ImGui::DockSpace(id, ImVec2(size.x, size.y), static_cast<ImGuiDockNodeFlags>(flags));
}

vc::GUIId VulkanGUI::_DockSpace(const char* id, const vcm::Vec2& size, vc::GUIDockNodeFlags flags)
{
    vc::GUIId guiId = ImGui::GetID(id);
    return ImGui::DockSpace(guiId, ImVec2(size.x, size.y), static_cast<ImGuiDockNodeFlags>(flags));
}

vc::GUIId VulkanGUI::_DockSpaceOverViewport()
{
    return ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
}

vc::GUIId VulkanGUI::_DockSpaceAddNode(vc::GUIId id, vc::GUIDockNodeFlags flags)
{
    return ImGui::DockBuilderAddNode(id, static_cast<ImGuiDockNodeFlags>(flags));
}

void VulkanGUI::_DockSpaceRemoveNode(vc::GUIId id)
{
    ImGui::DockBuilderRemoveNode(id);
}

void VulkanGUI::_DockSpaceSetNodeSize(vc::GUIId id, const vcm::Vec2& size)
{
    ImGui::DockBuilderSetNodeSize(id, ImVec2(size.x, size.y));
}

vc::GUIId VulkanGUI::_DockSpaceSplitNode(vc::GUIId id, vc::GUIDir split_dir, float size_ratio, vc::GUIId* out_id_at_dir,
    vc::GUIId* out_id_at_opposite_dir)
{
    return ImGui::DockBuilderSplitNode(id, static_cast<ImGuiDir>(split_dir), size_ratio, out_id_at_dir, out_id_at_opposite_dir);
}

void VulkanGUI::_DockWindow(const char* str_id, vc::GUIId id)
{
    ImGui::DockBuilderDockWindow(str_id, id);
}

void VulkanGUI::_DockFinish(vc::GUIId id)
{
    ImGui::DockBuilderFinish(id);
}

vc::GUIId VulkanGUI::_GetID(const char* str_id)
{
    return ImGui::GetID(str_id);
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

void VulkanGUI::_Test()
{
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoCloseButton;
    static bool dockspaceOpen = true;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoMove;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("MainWindow", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    ImGuiID mainDockSpaceId = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(mainDockSpaceId, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::Begin("AAA", nullptr, ImGuiWindowFlags_NoCollapse);
    {

    }
    ImGui::End();

    ImGui::Begin("BBB", nullptr, ImGuiWindowFlags_NoCollapse);
    {

    }
    ImGui::End();

    static bool sFirstFrame = true;
    if (sFirstFrame)
    {
        sFirstFrame = false;

        const ImVec2 dockspace_size = ImGui::GetContentRegionAvail();
        ImGui::DockBuilderRemoveNode(mainDockSpaceId);
        ImGui::DockBuilderAddNode(mainDockSpaceId, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(mainDockSpaceId, dockspace_size);

        ImGuiID dock_id_left;
        ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(mainDockSpaceId, ImGuiDir_Right, 0.5f, NULL, &dock_id_left);

        ImGui::DockBuilderDockWindow("AAA", dock_id_left);
        ImGui::DockBuilderDockWindow("BBB", dock_id_right);

        ImGui::DockBuilderFinish(mainDockSpaceId);
    }

    ImGui::End();
}
}
}
