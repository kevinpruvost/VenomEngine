///
/// Project: VenomEngineWorkspace
/// @file GUI.cc
/// @date Nov, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/GUI.h>

#include <venom/common/ECS.h>
#include <venom/common/plugin/graphics/Light.h>
#include <venom/common/SceneSettings.h>
#include <venom/common/Transform3D.h>
#include <venom/common/plugin/graphics/Camera.h>
#include <venom/common/plugin/graphics/GraphicsApplication.h>
#include <venom/common/plugin/graphics/Model.h>
#include <venom/common/plugin/graphics/RenderingPipeline.h>

// Disable NFD on Mobile
#if !defined(VENOM_PLATFORM_IOS)
#include <nfd.h>
#endif

#include <venom/common/Resources.h>
namespace venom
{
namespace common
{
GUI * GUI::s_gui = nullptr;
GUIDrawCallback GUI::s_guiDrawCallback = nullptr;
bool GUI::s_guiDraw = true;

GUI::GUI()
    : _app(nullptr)
    , __firstFrame(true)
    , _firstInit(true)
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

vc::Error GUI::Initialize()
{
    vc::Error err = _Initialize();
    if (err != vc::Error::Success) return err;
    return vc::Error::Success;
}

vc::Error GUI::Reset()
{
    vc::Error err = _Reset();
    if (err != vc::Error::Success) return err;

    __firstFrame = true;
    return vc::Error::Success;
}

void GUI::Render()
{
    DrawCallback();
    s_gui->_Render();
    __firstFrame = false;
    _firstInit = false;
}

void GUI::AddFont(const char* fontPath, float fontSize, const uint16_t* glyphRanges)
{
    auto realPath = Resources::GetFontsResourcePath(fontPath);
    s_gui->_AddFont(realPath.c_str(), fontSize, glyphRanges);
}

void GUI::AddFont(const char* fontPath, float fontSize)
{
    auto realPath = Resources::GetFontsResourcePath(fontPath);
    s_gui->_AddFont(realPath.c_str(), fontSize);
}

bool GUI::isFirstFrame()
{
    return s_gui->__firstFrame;
}

bool GUI::isFirstInitialization()
{
    return s_gui->_firstInit;
}

void GUI::GraphicsSettingsCollaspingHeader()
{
    if (vc::GUI::CollapsingHeader("Graphics Settings", GUITreeNodeFlagsBits::GUITreeNodeFlags_None)) {
        // Graphics Settings
        vc::GUI::SeparatorText("General");
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

        // Shadow Bias

        // Scene Graphics Settings
        vc::GUI::SeparatorText("Scene-Related");
        // Background Target Luminance
        float targetLuminance = vc::SceneSettings::GetTargetLuminance();
        vc::GUI::SetNextItemWidth(100.0f);
        if (vc::GUI::SliderFloat("Target Luminance", &targetLuminance, 0.0f, 1000.0f)) {
            vc::SceneSettings::SetTargetLuminance(targetLuminance);
        }

        // Debug Graphics Settings
        vc::GUI::SeparatorText("Debug");
        const vc::Vector<vc::String> & debugVisualizerModes = vc::GraphicsSettings::GetDebugVisualizerStrings();
        int debugVisualizerMode = static_cast<int>(vc::GraphicsSettings::GetDebugVisualizationMode());
        if (vc::GUI::BeginCombo("Visualizer", debugVisualizerModes[debugVisualizerMode].c_str())) {
            for (int i = 0; i < debugVisualizerModes.size(); i++) {
                bool isSelected = (debugVisualizerMode == i);
                if (vc::GUI::Selectable(debugVisualizerModes[i].c_str(), isSelected)) {
                    debugVisualizerMode = i;
                    vc::GraphicsSettings::SetDebugVisualizationMode(static_cast<vc::GraphicsSettings::DebugVisualizationMode>(i));
                }
                if (isSelected) {
                    vc::GUI::SetItemDefaultFocus();
                }
            }
            vc::GUI::EndCombo();
        }
    }
}

static vc::Entity selectedEntity;
void GUI::EntitiesListCollapsingHeader()
{
    if (vc::GUI::CollapsingHeader("Entities", GUITreeNodeFlagsBits::GUITreeNodeFlags_DefaultOpen)) {
        vc::GUI::SeparatorText("Hierarchy");
        int selected = -1;
        if (vc::GUI::BeginChild("##EntitiesList", vcm::Vec2(0, 300), GUIChildFlagsBits::GUIChildFlags_FrameStyle | GUIChildFlagsBits::GUIChildFlags_ResizeY))
        {
            int n = 0;
            vc::ECS::ForEach<vc::Transform3D>([&](vc::Entity entity, vc::Transform3D & cm) {
                vc::String name = ICON_MS_DEPLOYED_CODE " ";
                name += entity.name();
                if (selectedEntity == entity) {
                    selected = n;
                }
                if (vc::GUI::Selectable(name.c_str(), selected == n)) {
                    selectedEntity = entity;
                    selected = n;
                }
                ++n;
            });
        }
        vc::GUI::EndChild();
        // Adding entities context menu
        if (vc::GUI::BeginPopupContextItem("EntityCreationPopup", GUIPopupFlagsBits::GUIPopupFlags_MouseButtonRight)) {
            if (selectedEntity.is_valid()) {
                if (vc::GUI::MenuItem(ICON_MS_DELETE " Delete")) {
                    selectedEntity.destruct();
                }
                if (vc::Camera::GetMainCamera()->GetFocusEntity() == selectedEntity) {
                    if (vc::GUI::MenuItem(ICON_MS_VISIBILITY_LOCK " Remove Focus")) {
                        vc::Camera::GetMainCamera()->RemoveFocusEntity();
                    }
                } else {
                    if (vc::GUI::MenuItem(ICON_MS_VISIBILITY_LOCK " Focus Entity")) {
                        vc::Camera::GetMainCamera()->SetFocusEntity(selectedEntity);
                    }
                }
            }
            if (vc::GUI::MenuItem(ICON_MS_DEPLOYED_CODE " Create New Entity")) {
                vc::CreateEntity();
            }
            vc::GUI::EndPopup();
        }
    }
    __EntityPropertiesWindow();
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

    float totalWidth = vc::GUI::GetContentRegionAvail().x;

#if !defined(VENOM_PLATFORM_MOBILE)
    vc::GUI::PushButtonTextAlign({0.0f, 0.5f});
    if (vc::GUI::Button(texture->GetShortName().c_str(), {totalWidth * 2.0f / 3.0f, 0.0f})) {
        nfdchar_t *outPath = nullptr;
        const char * filterTextures = "png;jpg;tga;bmp;gif;psd;hdr;exr;jpeg";
        nfdresult_t result = NFD_OpenDialog(filterTextures, nullptr, &outPath);
        if (result == NFD_OKAY) {
            path = outPath;
            ret = true;
            free(outPath);
        }
    }
    vc::GUI::PopStyleVar();
    vc::GUI::SameLine();
    vc::GUI::Text("Texture File");
#endif
    return ret;
}

bool GUI::EditableModel(vc::Model * model, vc::String& path)
{
    bool ret = false;
#if !defined(VENOM_PLATFORM_MOBILE)

    float totalWidth = vc::GUI::GetContentRegionAvail().x;

    vc::GUI::PushButtonTextAlign({0.0f, 0.5f});
    if (vc::GUI::Button(model->GetShortName().c_str(), {totalWidth * 2.0f / 3.0f, 0.0f})) {
        nfdchar_t *outPath = nullptr;
        const char * filterModels = "obj;glb;fbx;gltf";
        nfdresult_t result = NFD_OpenDialog(filterModels, nullptr, &outPath);
        if (result == NFD_OKAY) {
            path = outPath;
            ret = true;
            free(outPath);
        }
    }
    vc::GUI::PopStyleVar();
    vc::GUI::SameLine();
    vc::GUI::Text("File");
#endif
    return ret;
}

void GUI::EntityGuizmo(const vcm::Vec2 & renderingSize)
{
    __EntityGuizmo(renderingSize);
}

void GUI::__EntityPropertiesWindow()
{
    vc::GUI::Begin(ICON_MS_INFO " Inspector");
    {
        if (selectedEntity) {
            vc::GUI::SeparatorText("Entity");
            char buffer[128] = {0};
            vc::String entityName(selectedEntity.name().c_str());
            memcpy(buffer, entityName.c_str(), entityName.size());
            if (vc::GUI::InputText("Name", buffer, 128 - 1)) {
                if (vc::ECS::IsNameAvailable(buffer)) {
                    selectedEntity.set_name(buffer);
                }
            }
            int id = static_cast<uint32_t>(selectedEntity.id());
            vc::GUI::Text("ID: %d", id);
            vc::GUI::SeparatorText("Components");

            // GUI for every component
            selectedEntity.each([&](flecs::id componentID)
            {
                void * component = selectedEntity.get_mut(componentID);
                vc::VenomComponent::GUIComponentAction action = reinterpret_cast<Component*>(component)->__GUI(selectedEntity);
                switch (action)
                {
                    case vc::VenomComponent::GUIComponentAction::Remove:
                        selectedEntity.remove(componentID);
                        break;
                    default:
                        break;
                }
                vc::GUI::Dummy(vcm::Vec2(0, 2));
            });

            // Padding
            vc::GUI::Separator();
            vc::GUI::Dummy(vcm::Vec2(0, 2));

            // Add Component Button
            if (vc::GUI::Button("+ Add Component", vcm::Vec2(-1, 0)))
            {
                // Show all components
                vc::GUI::OpenPopup("add_component_popup");
            }
            if (vc::GUI::BeginPopup("add_component_popup"))
            {
                vc::GUI::SeparatorText("Components");

                const auto & componentCreateFuncs = ECS::s_ecs->__componentsCreateAndHasFuncs;
                const auto & componentCanCreateFuncs = ECS::s_ecs->__GetComponentCanCreateFuncs();
                for (const auto & [name, displayName, funcCreate, funcHas] : componentCreateFuncs)
                {
                    if (funcHas(selectedEntity))
                        continue;
                    const auto & canCreateFunc = componentCanCreateFuncs.find(name);
                    if (canCreateFunc != componentCanCreateFuncs.end() && !canCreateFunc->second(selectedEntity))
                        continue;
                    if (vc::GUI::Button(displayName.c_str(), vcm::Vec2(-1, 0)))
                    {
                        funcCreate(selectedEntity);
                        vc::GUI::CloseCurrentPopup();
                    }
                }

                vc::GUI::EndPopup();
            }
        }
    }
    vc::GUI::End();
}

void GUI::__EntityGuizmo(const vcm::Vec2 & renderingSize)
{
    vc::Transform3D * transform = selectedEntity.is_valid() ? selectedEntity.get_mut<vc::Transform3D>() : nullptr;
    s_gui->_EntityGuizmo(transform, renderingSize);
}

vc::Error GUI::__PreUpdate()
{
    return _PreUpdate();
}
}
}
