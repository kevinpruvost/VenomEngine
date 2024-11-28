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
    void _SetNextWindowPos(const vcm::Vec2 & pos, vc::GUICond cond, const vcm::Vec2 & pivot);

    const vcm::Vec2 & _GetWindowSize() override;
    const vcm::Vec2 & _GetWindowPos() override;

    void _NewFrame() override;
    void _Begin(const char * name, bool * p_open, vc::GUIWindowFlags flags) override;
    void _End() override;
    void _Text(const char* fmt, ...) override;
    void _TextColored(const vcm::Vec4 & col, const char* fmt, ...) override;
    void _LabelText(const char* label, const char* fmt, ...) override;

    bool _SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float power) override;
    bool _SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format, float power);

    void _ColorEdit3(const char* label, float col[3], vc::GUIColorEditFlags flags) override;

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

    bool _MenuItem(const char* str, const char* text) override;

    void _SetItemDefaultFocus() override;

    void _SameLine(float offset_from_start_x, float spacing) override;

    vc::Error _PreUpdate() override;
    void _Render() override;

private:
    void __SetStyle();

private:
    ImGui_ImplVulkan_InitInfo initInfo;
};
}
}