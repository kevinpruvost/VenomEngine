///
/// Project: VenomEngineWorkspace
/// @file main_input.cc
/// @date Nov, 08 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"
#include "../lib/external/imgui/imgui.h"

bool cameraLocked = true;
#if defined(VENOM_PLATFORM_MOBILE)
bool automaticTurn = true;
#else
bool automaticTurn = false;
#endif

static vc::Timer timer_uni;
static float cameraSpeed = 5.0f;

void SceneInput(vc::Context * context)
{
    float time = timer_uni.GetMilliSeconds();
    timer_uni.Reset();
    if (cameraLocked == false) {
        // Camera controls
        if (context->IsKeyPressed(vc::KeyboardInput::KeyboardW)) {
            vc::Camera::GetMainCamera()->MoveForward(cameraSpeed * vc::Timer::GetLambdaSeconds());
        }
        if (context->IsKeyPressed(vc::KeyboardInput::KeyboardS)) {
            vc::Camera::GetMainCamera()->MoveForward(-cameraSpeed * vc::Timer::GetLambdaSeconds());
        }
        if (context->IsKeyPressed(vc::KeyboardInput::KeyboardA)) {
            vc::Camera::GetMainCamera()->MoveRight(-cameraSpeed * vc::Timer::GetLambdaSeconds());
        }
        if (context->IsKeyPressed(vc::KeyboardInput::KeyboardD)) {
            vc::Camera::GetMainCamera()->MoveRight(cameraSpeed * vc::Timer::GetLambdaSeconds());
        }
        if (context->IsKeyPressed(vc::KeyboardInput::KeyboardQ)) {
            vc::Camera::GetMainCamera()->MoveUp(-cameraSpeed * vc::Timer::GetLambdaSeconds());
        }
        if (context->IsKeyPressed(vc::KeyboardInput::KeyboardE)) {
            vc::Camera::GetMainCamera()->MoveUp(cameraSpeed * vc::Timer::GetLambdaSeconds());
        }
        if (!vc::Camera::GetMainCamera()->GetFocusEntity().is_valid()) {
            vcm::Vec2 mouseMov = context->GetMouseMove();
            vc::Camera::GetMainCamera()->RotateYaw( -mouseMov.x * 0.0025f);
            vc::Camera::GetMainCamera()->RotatePitch(-mouseMov.y * 0.0025f);
        }
    }

    if (automaticTurn) {
        vc::ECS::ForEach<vc::Transform3D, vc::Model>([&](vc::Entity entity, vc::Transform3D & transform, vc::Model & model) {
            transform.RotateYaw(time / 1000.0f);
        });
        // vc::ECS::GetEntity("balls_hd").get_mut<vc::Transform3D>()->RotateYaw(time / 1000.0f);
        // vc::ECS::GetEntity("face_hd").get_mut<vc::Transform3D>()->RotateYaw(time / 1000.0f);
    }

    if (context->IsKeyReleased(vc::KeyboardInput::KeyboardT)) {
        cameraLocked = !cameraLocked;
    }

    if (context->IsKeyReleased(vc::KeyboardInput::KeyboardC) && context->IsKeyModifierPressed(vc::KeyboardModifier::KeyboardModShift | vc::KeyboardModifier::KeyboardModControl)) {
        vc::ShaderPipeline::RecompileAllShaders();
        vc::ShaderPipeline::ReloadAllShaders();
    }

    // Fullscreen mode (no GUI)
    if (context->IsKeyReleased(vc::KeyboardInput::KeyboardF)) {
        vc::GUI::ToggleGUIDraw();
    }
}

void SceneGUI()
{
    if (vc::GUI::isFirstFrame())
    {
        vc::GUI::AddFont("NotoSans-Regular.ttf", 16.0f);
        uint16_t ranges[] = {ICON_MIN_MS, ICON_MAX_16_MS, 0};
        vc::GUI::AddFont("MaterialSymbolsOutlined.ttf", 16.0f, ranges);
    }

    vc::Entity light1 = vc::ECS::GetEntity("light1");

    vc::GUI::NewFrame();

    static vc::GUIDockNodeFlags dockspaceFlags = vc::GUIDockNodeFlags_NoCloseButton | vc::GUIDockNodeFlags_NoWindowMenuButton;
    static bool dockspaceOpen = true;

    vc::GUIWindowFlags window_flags = vc::GUIWindowFlags_MenuBar | vc::GUIWindowFlags_NoDocking |  vc::GUIWindowFlags_NoCollapse | vc::GUIWindowFlags_NoTitleBar | vc::GUIWindowFlags_NoDecoration | vc::GUIWindowFlags_NoResize | vc::GUIWindowFlags_NoNavFocus | vc::GUIWindowFlags_NoMove;

    vc::GUIViewport mainViewport = vc::GUI::GetMainViewport();
    vc::GUI::SetNextWindowViewport(mainViewport);
    vc::GUI::SetNextWindowPos(vcm::Vec2(0, 0), vc::GUICondBits::GUICond_Always);
    vc::GUI::SetNextWindowSize(vcm::Vec2(vc::Context::GetWindowWidth(), vc::Context::GetWindowHeight()), vc::GUICondBits::GUICond_Always);
    vc::GUI::PushWindowPadding({0, 0});
    vc::GUI::Begin("DockSpace", &dockspaceOpen, window_flags);
    vc::GUI::PopStyleVar();

    vc::GUIId mainDockSpaceId = vc::GUI::GetID("MainViewport");
    vc::GUI::DockSpace(mainDockSpaceId, vcm::Vec2(0.0f, 0.0f), dockspaceFlags);
    {
        if (vc::GUI::BeginMenuBar()) {
            static vc::Texture venomIcon("VenomEngineLogo_Small.png");
            vc::GUI::Image(&venomIcon, {16, 16}, false);
            if (vc::GUI::BeginMenu("Shaders")) {
                if (vc::GUI::MenuItem("Recompile & Reload Shaders", "Ctrl+Shift+C")) {
                    vc::ShaderPipeline::RecompileAllShaders();
                    vc::ShaderPipeline::ReloadAllShaders();
                }
                if (vc::GUI::MenuItem("Reload Shaders")) {
                    vc::ShaderPipeline::ReloadAllShaders();
                }
                vc::GUI::EndMenu();
            }
            vc::GUI::EndMenuBar();
        }

        vc::GUI::Begin(ICON_MS_SETTINGS " Settings & Objects", nullptr, vc::GUIWindowFlags_NoCollapse);
        {
            if (vc::GUI::CollapsingHeader("Scene Settings", vc::GUITreeNodeFlagsBits::GUITreeNodeFlags_DefaultOpen)) {
                vc::GUI::Checkbox("Draw GUI (press F to toggle)", &vc::GUI::IsGUIDrawRef());
                vc::GUI::Checkbox("Camera Locked", &cameraLocked);
                vcm::Vec3 cameraPos = vc::Camera::GetMainCamera()->GetPosition();
                // Camera speed
                vc::GUI::SliderFloat("Camera Speed", &cameraSpeed, 0.1f, 20.0f);
                vc::GUI::Checkbox("Automatic Turn", &automaticTurn);
                // vc::GUI::SliderFloat("Light Intensity", light1.get_mut<vc::Light>()->GetIntensityPtr(), 0.0f, 100.0f);
                // vc::GUI::SliderFloat3("Light Direction", light1.get_mut<vc::Light>()->GetDirectionPtr(), -1.0f, 1.0f);
            }
            vc::GUI::Dummy(vcm::Vec2(0, 2));
            vc::GUI::GraphicsSettingsCollaspingHeader();
            vc::GUI::Dummy(vcm::Vec2(0, 2));
            vc::GUI::EntitiesListCollapsingHeader();
        }
        vc::GUI::End();

        vc::GUI::PushWindowPadding({0, 0});
        vc::GUI::Begin(ICON_MS_FOREST " Scene", nullptr, vc::GUIWindowFlags_NoSavedSettings);
        {
            static vc::RenderTarget renderTarget(vc::RenderingPipelineType::PBRModel, vc::ShaderVertexFormat::Vec4);
            // 16/9 size
            vcm::Vec2 renderingSize = vc::GUI::GetContentRegionAvail();
            renderingSize.y = std::min(std::floor(renderingSize.x * 9.0f / 16.0f), renderingSize.y);
            vc::GUI::Image(&renderTarget, renderingSize);
            vc::GUI::EntityGuizmo(renderingSize);
            //static vc::Texture textureTest("hank_happy.png");
            //vc::GUI::Image(&textureTest, vc::GUI::GetContentRegionAvail());
        }
        vc::GUI::End();
        vc::GUI::PopStyleVar();

        if (vc::GUI::isFirstInitialization())
        {
            const vcm::Vec2 dockspaceSize = vc::GUI::GetWindowSize();
            vc::GUI::DockSpaceRemoveNode(mainDockSpaceId);
            vc::GUI::DockSpaceAddNode(mainDockSpaceId, vc::GUIDockNodeFlags_DockSpace | vc::GUIDockNodeFlags_NoWindowMenuButton);
            vc::GUI::DockSpaceSetNodeSize(mainDockSpaceId, dockspaceSize);

            vc::GUIId dockIdSettingsObjects;
            vc::GUIId dockIdSceneRendering;
            vc::GUIId dockIdEntitiesProps;

            dockIdSettingsObjects = vc::GUI::DockSpaceSplitNode(mainDockSpaceId, vc::GUIDir_Left, 0.25f, NULL, &dockIdSceneRendering);
            dockIdEntitiesProps = vc::GUI::DockSpaceSplitNode(dockIdSceneRendering, vc::GUIDir_Right, 0.3f, NULL, &dockIdEntitiesProps);

            vc::GUI::DockWindow(ICON_MS_SETTINGS " Settings & Objects", dockIdSettingsObjects);
            vc::GUI::DockWindow(ICON_MS_FOREST " Scene", dockIdSceneRendering);
            vc::GUI::DockWindow(ICON_MS_INFO " Inspector", dockIdEntitiesProps);

            vc::GUI::DockFinish(mainDockSpaceId);
        }
    }
    vc::GUI::End();
}
