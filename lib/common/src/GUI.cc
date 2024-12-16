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

#include <nfd.h>

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

bool GUI::EditableTexture(vc::Texture * texture, vc::String & path)
{
    bool ret = false;
    // Can see list but should filter out later
    // const auto & graphicObjects = GraphicsPluginObject::GetCachedObjects();
    // if (vc::GUI::BeginCombo("Texture", texture.GetName().c_str())) {
    //     for (const auto & [name, graphicsObject] : graphicObjects) {
    //         if (graphicsObject->As<TextureResource>()) {
    //             bool isSelected = texture == graphicsObject;
    //             if (vc::GUI::Selectable(name.c_str(), isSelected)) {
    //                 texture.LoadImageFromCachedResource(graphicsObject);
    //                 ret = true;
    //             }
    //             if (isSelected) {
    //                 vc::GUI::SetItemDefaultFocus();
    //             }
    //         }
    //     }
    //     vc::GUI::EndCombo();
    // }

    if (vc::GUI::Button(texture->GetShortName().c_str())) {
        nfdchar_t *outPath = nullptr;
        const char * filterTextures = "png;jpg;tga;bmp;gif;psd;hdr;exr;jpeg";
        nfdresult_t result = NFD_OpenDialog(filterTextures, nullptr, &outPath);
        if (result == NFD_OKAY) {
            path = outPath;
            ret = true;
            free(outPath);
        }
    }
    vc::GUI::SameLine();
    vc::GUI::Text("Texture");
    return ret;
}

bool GUI::EditableModel(vc::Model * model, vc::String& path)
{
    bool ret = false;

    if (vc::GUI::Button(model->GetShortName().c_str())) {
        nfdchar_t *outPath = nullptr;
        const char * filterModels = "obj;glb;fbx;gltf";
        nfdresult_t result = NFD_OpenDialog(filterModels, nullptr, &outPath);
        if (result == NFD_OKAY) {
            path = outPath;
            ret = true;
            free(outPath);
        }
    }
    vc::GUI::SameLine();
    vc::GUI::Text("Model");
    return ret;
}

void GUI::_EntityPropertiesWindow()
{
    vc::GUI::SetNextWindowPos(vcm::Vec2{vc::Context::GetWindowWidth(), 20}, vc::GUICondBits::GUICond_Always, vcm::Vec2(1.0f, 0));
    vc::GUI::Begin("Entity Properties");
    {
        char buffer[128] = {0};
        vc::String entityName(selectedEntity.name().c_str());
        memcpy(buffer, entityName.c_str(), entityName.size());
        if (vc::GUI::InputText("Name", buffer, 128 - 1)) {
            selectedEntity.set_name(buffer);
        }
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
