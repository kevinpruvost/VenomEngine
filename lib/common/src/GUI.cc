///
/// Project: VenomEngineWorkspace
/// @file GUI.cc
/// @date Nov, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/GUI.h>

#include <venom/common/ComponentManager.h>
#include <venom/common/ECS.h>
#include <venom/common/Light.h>
#include <venom/common/SceneSettings.h>
#include <venom/common/Transform3D.h>
#include <venom/common/plugin/graphics/Camera.h>
#include <venom/common/plugin/graphics/GraphicsApplication.h>
#include <venom/common/plugin/graphics/Model.h>
#include <venom/common/plugin/graphics/RenderingPipeline.h>

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

void GUI::GraphicsSettingsCollaspingHeader()
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
        // Background Target Luminance
        static float targetLuminance;
        targetLuminance = vc::SceneSettings::GetTargetLuminance();
        if (vc::GUI::SliderFloat("Target Luminance", &targetLuminance, 0.0f, 1000.0f)) {
            vc::SceneSettings::SetTargetLuminance(targetLuminance);
        }
    }
}

static vc::Entity selectedEntity;
void GUI::EntitiesListCollapsingHeader()
{
    if (vc::GUI::CollapsingHeader("Entities", GUITreeNodeFlagsBits::GUITreeNodeFlags_DefaultOpen)) {
        static int selected = -1;
        if (vc::GUI::BeginChild("##EntitiesList", vcm::Vec2(0, 300), GUIChildFlagsBits::GUIChildFlags_FrameStyle | GUIChildFlagsBits::GUIChildFlags_ResizeY))
        {
            int n = 0;
            vc::ECS::ForEach<vc::ComponentManager>([&](vc::Entity entity, vc::ComponentManager & cm) {
                if (vc::GUI::Selectable(entity.name().c_str(), selected == n)) {
                    selected = n;
                    selectedEntity = entity;
                }
                ++n;
            });
        }
        vc::GUI::EndChild();
    }
    if (selectedEntity.is_valid() && selectedEntity.is_alive()) {
        _EntityPropertiesWindow();
    }
}

void GUI::_EntityPropertiesWindow()
{
    vc::GUI::SetNextWindowPos(vcm::Vec2{vc::Context::GetWindowWidth(), 20}, vc::GUICondBits::GUICond_Always, vcm::Vec2(1.0f, 0));
    vc::GUI::Begin("Entity Properties");
    {
        vc::GUI::Text(selectedEntity.name().c_str());
        vc::GUI::Text("ID: %d", selectedEntity.id());
        vc::GUI::SeparatorText("Components");

        // GUI for every component
        selectedEntity.each([&](flecs::id componentID)
        {
            void * component = selectedEntity.get_mut(componentID);
            int idTest = selectedEntity.raw_id();
            reinterpret_cast<Component*>(component)->GUI();
        });
    }
    vc::GUI::End();
}

vc::Error GUI::__PreUpdate()
{
    return _PreUpdate();
}
}
}
