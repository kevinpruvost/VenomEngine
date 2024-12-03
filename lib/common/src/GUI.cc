///
/// Project: VenomEngineWorkspace
/// @file GUI.cc
/// @date Nov, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/GUI.h>

#include "venom/common/plugin/graphics/GraphicsApplication.h"

namespace venom
{
namespace common
{
GUI * GUI::s_gui = nullptr;
GUIDrawCallback GUI::s_guiDrawCallback = nullptr;

GUI::GUI()
    : _app(nullptr)
{
    venom_assert(s_gui == nullptr, "GUI::GUI() : s_gui is not nullptr");
    s_gui = this;
}

GUI::~GUI()
{
    s_gui = nullptr;
}

void GUI::SetGraphicsApplication(GraphicsApplication* app)
{
    _app = app;
}

void GUI::GraphicsSettingsWindow()
{
    if (vc::GUI::CollapsingHeader("Graphics Settings", GUITreeNodeFlagsBits::GUITreeNodeFlags_None)) {
        // Multisampling
        const vc::Vector<vc::String> & msaaModes = vc::GraphicsSettings::GetAvailableMultisamplingCountOptionsStrings();
        int msaaMode = vc::GraphicsSettings::GetActiveMultisamplingCountIndex();
        if (vc::GUI::BeginCombo("MSAA:", msaaModes[msaaMode].c_str())) {
            for (int i = 0; i < vc::GraphicsSettings::GetAvailableMultisamplingCountOptions().size(); i++) {
                bool isSelected = (msaaMode == i);
                if (vc::GUI::Selectable(msaaModes[i].c_str(), isSelected)) {
                    msaaMode = i;
                    vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::GetAvailableMultisamplingCountOptions()[i]);
                }
                if (isSelected) {
                    vc::GUI::SetItemDefaultFocus();
                }
            }
            vc::GUI::EndCombo();
        }
        // HDR
        bool hdrEnabled = vc::GraphicsSettings::IsHDREnabled();
        if (vc::GUI::Checkbox("HDR (High Dynamic Range)", &hdrEnabled)) {
            vc::GraphicsSettings::SetHDR(hdrEnabled);
        }
    }
}

vc::Error GUI::__PreUpdate()
{
    return _PreUpdate();
}
}
}
