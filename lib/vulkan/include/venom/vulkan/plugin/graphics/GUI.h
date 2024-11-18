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
    void _NewFrame() override;
    void _Begin(const char * name, bool * p_open, vc::GUIWindowFlags flags) override;
    void _End() override;
    void _Text(const char* fmt, ...) override;
    void _TextColored(const vcm::Vec4 & col, const char* fmt, ...) override;
    void _LabelText(const char* label, const char* fmt, ...) override;

    void _SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float power) override;
    void _SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format, float power);

    void _ColorEdit3(const char* label, float col[3], vc::GUIColorEditFlags flags) override;

    void _Button(const char* label, const vcm::Vec2 & size) override;
    void _Checkbox(const char* label, bool* v) override;
    void _ProgressBar(float fraction, const vcm::Vec2 & size_arg, const char* overlay) override;

    void _SameLine(float offset_from_start_x, float spacing) override;

    void _Render() override;

private:
    void __SetStyle();

private:
    ImGui_ImplVulkan_InitInfo initInfo;
};
}
}