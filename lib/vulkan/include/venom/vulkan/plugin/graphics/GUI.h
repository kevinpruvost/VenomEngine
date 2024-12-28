///
/// Project: VenomEngineWorkspace
/// @file GUI.h
/// @date Nov, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/GUI.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace venom
{
namespace vulkan
{
class VulkanGUI : public vc::GUI
{
public:
    VulkanGUI();
    ~VulkanGUI() override;

    vc::Error Initialize() override;
    vc::Error Reset() override;

protected:
    void _SetNextWindowPos(const vcm::Vec2 & pos, vc::GUICond cond, const vcm::Vec2 & pivot) override;
    void _SetNextWindowSize(const vcm::Vec2& size, vc::GUICond cond) override;
    void _SetNextWindowViewport(vc::GUIViewport viewport) override;

    vcm::Vec2 _GetContentRegionAvail() override;

    vcm::Vec2 _GetWindowSize() override;
    vcm::Vec2 _GetWindowPos() override;
    vc::GUIViewport _GetMainViewport() override;

    void _NewFrame() override;
    void _Begin(const char * name, bool * p_open, vc::GUIWindowFlags flags) override;
    void _End() override;
    void _Text(const char* fmt, ...) override;
    void _TextColored(const vcm::Vec4 & col, const char* fmt, ...) override;
    void _LabelText(const char* label, const char* fmt, ...) override;

    void _Image(const vc::Texture* texture, const vcm::Vec2 & size) override;

    bool _InputText(const char* label, char* buf, size_t buf_size, vc::GUIInputTextFlags flags) override;

    bool _TreeNode(const char* label) override;
    void _TreePop() override;
    void _TreePush(const char* str_id) override;

    void _SeparatorText(const char* text) override;
    void _Separator() override;
    void _Spacing() override;

    bool _SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format) override;
    bool _SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format) override;

    bool _InputFloat(const char* label, float* v, float step, float step_fast, const char* format, vc::GUIColorEditFlags flags) override;
    bool _InputFloat3(const char* label, float v[3], const char* format, vc::GUIColorEditFlags flags) override;

    void _ColorEdit3(const char* label, float col[3], vc::GUIColorEditFlags flags) override;

    bool _CollapsingHeader(const char* label, vc::GUITreeNodeFlags flags) override;

    bool _Button(const char* label, const vcm::Vec2 & size) override;
    bool _Checkbox(const char* label, bool* v) override;
    void _ProgressBar(float fraction, const vcm::Vec2 & size_arg, const char* overlay) override;

    bool _Selectable(const char* label, bool selected, vc::GUISelectableFlags flags, const vcm::Vec2 & size) override;

    bool _BeginCombo(const char* label, const char* preview_value, vc::GUIComboFlags flags) override;
    void _EndCombo() override;

    bool _BeginMenu(const char* label, bool enabled) override;
    void _EndMenu() override;

    bool _BeginMainMenuBar() override;
    void _EndMainMenuBar() override;

    bool _BeginMenuBar() override;
    void _EndMenuBar() override;

    bool _BeginChild(const char* str_id, const vcm::Vec2 & size, vc::GUIChildFlags childFlags, vc::GUIWindowFlags extra_flags) override;
    void _EndChild() override;

    bool _MenuItem(const char* str, const char* text) override;

    void _SetNextItemWidth(float item_width) override;
    void _SetItemDefaultFocus() override;

    void _SameLine(float offset_from_start_x, float spacing) override;

    void _PushWindowPadding(const vcm::Vec2& padding) override;
    void _PopStyleVar() override;

    vc::GUIId _DockSpace(vc::GUIId id, const vcm::Vec2& size, vc::GUIDockNodeFlags flags) override;
    vc::GUIId _DockSpace(const char* id, const vcm::Vec2& size, vc::GUIDockNodeFlags flags) override;
    vc::GUIId _DockSpaceOverViewport() override;

    vc::GUIId _DockSpaceAddNode(vc::GUIId id, vc::GUIDockNodeFlags flags) override;
    void _DockSpaceRemoveNode(vc::GUIId id) override;
    void _DockSpaceSetNodeSize(vc::GUIId id, const vcm::Vec2& size) override;

    vc::GUIId _DockSpaceSplitNode(vc::GUIId id, vc::GUIDir split_dir, float size_ratio, vc::GUIId* out_id_at_dir, vc::GUIId* out_id_at_opposite_dir) override;

    void _DockWindow(const char* str_id, vc::GUIId id) override;
    void _DockFinish(vc::GUIId id) override;

    vc::GUIId _GetID(const char* str_id) override;

    vc::Error _PreUpdate() override;
    void _Render() override;

    void _Test() override;

private:
    void __SetStyle();

private:
    ImGui_ImplVulkan_InitInfo initInfo;
};
}
}
