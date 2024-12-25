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
bool automaticTurn = true;

static vc::Timer timer_uni;

void SceneInput(vc::Context * context)
{
    float time = timer_uni.GetMilliSeconds();
    timer_uni.Reset();
    if (cameraLocked == false) {
        // Camera controls
        const float speed = 5.0f;
        if (context->IsKeyPressed(vc::KeyboardInput::KeyboardW)) {
            vc::Camera::GetMainCamera()->MoveForward(speed * vc::Timer::GetLambdaSeconds());
        }
        if (context->IsKeyPressed(vc::KeyboardInput::KeyboardS)) {
            vc::Camera::GetMainCamera()->MoveForward(-speed * vc::Timer::GetLambdaSeconds());
        }
        if (context->IsKeyPressed(vc::KeyboardInput::KeyboardA)) {
            vc::Camera::GetMainCamera()->MoveRight(-speed * vc::Timer::GetLambdaSeconds());
        }
        if (context->IsKeyPressed(vc::KeyboardInput::KeyboardD)) {
            vc::Camera::GetMainCamera()->MoveRight(speed * vc::Timer::GetLambdaSeconds());
        }
        if (context->IsKeyPressed(vc::KeyboardInput::KeyboardQ)) {
            vc::Camera::GetMainCamera()->MoveUp(-speed * vc::Timer::GetLambdaSeconds());
        }
        if (context->IsKeyPressed(vc::KeyboardInput::KeyboardE)) {
            vc::Camera::GetMainCamera()->MoveUp(speed * vc::Timer::GetLambdaSeconds());
        }
        vcm::Vec2 mouseMov = context->GetMouseMove();
        vc::Camera::GetMainCamera()->RotateYaw( -mouseMov.x * 0.0025f);
        vc::Camera::GetMainCamera()->RotatePitch(-mouseMov.y * 0.0025f);
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
}

void SceneGUI()
{
    vc::Entity light1 = vc::ECS::GetEntity("light1");

    vc::GUI::NewFrame();

    if (vc::GUI::BeginMainMenuBar()) {
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
        vc::GUI::EndMainMenuBar();
    }

    static vc::GUIDockNodeFlags dockspaceFlags = vc::GUIDockNodeFlags_NoCloseButton | vc::GUIDockNodeFlags_NoWindowMenuButton;
    static bool dockspaceOpen = true;

    vc::GUIWindowFlags window_flags = vc::GUIWindowFlags_MenuBar | vc::GUIWindowFlags_NoDocking | vc::GUIWindowFlags_NoCollapse | vc::GUIWindowFlags_NoTitleBar | vc::GUIWindowFlags_NoDecoration | vc::GUIWindowFlags_NoResize | vc::GUIWindowFlags_NoNavFocus | vc::GUIWindowFlags_NoMove;

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
        vc::GUI::Begin("Settings & Objects", nullptr, vc::GUIWindowFlags_NoCollapse);
        {
            if (vc::GUI::CollapsingHeader("Scene Settings:", vc::GUITreeNodeFlagsBits::GUITreeNodeFlags_DefaultOpen)) {
                vc::GUI::Checkbox("Camera Locked", &cameraLocked);
                vcm::Vec3 cameraPos = vc::Camera::GetMainCamera()->GetPosition();
                // if (vc::GUI::SliderFloat3("Camera Position", &cameraPos[0], -100.0f, 100.0f)) {
                //     vc::Camera::GetMainCamera()->SetPosition(cameraPos);
                // }
                vc::GUI::Checkbox("Automatic Turn", &automaticTurn);
                // vc::GUI::SliderFloat("Light Intensity", light1.get_mut<vc::Light>()->GetIntensityPtr(), 0.0f, 100.0f);
                // vc::GUI::SliderFloat3("Light Direction", light1.get_mut<vc::Light>()->GetDirectionPtr(), -1.0f, 1.0f);
            }
            vc::GUI::GraphicsSettingsCollaspingHeader();
            vc::GUI::EntitiesListCollapsingHeader();
        }
        vc::GUI::End();

        vc::GUI::PushWindowPadding({0, 0});
        vc::GUI::Begin("Test", nullptr, vc::GUIWindowFlags_NoSavedSettings);
        {
            static vc::Texture textureTest("hank_happy.png");
            vc::GUI::Image(&textureTest, vc::GUI::GetContentRegionAvail());
        }
        vc::GUI::End();
        vc::GUI::PopStyleVar();

        static bool sFirstFrame = true;
        if (sFirstFrame)
        {
            sFirstFrame = false;

            const vcm::Vec2 dockspaceSize = vc::GUI::GetContentRegionAvail();
            vc::GUI::DockSpaceRemoveNode(mainDockSpaceId);
            vc::GUI::DockSpaceAddNode(mainDockSpaceId, vc::GUIDockNodeFlags_DockSpace | vc::GUIDockNodeFlags_NoWindowMenuButton);
            vc::GUI::DockSpaceSetNodeSize(mainDockSpaceId, dockspaceSize);

            vc::GUIId dock_id_left;
            vc::GUIId dock_id_right = vc::GUI::DockSpaceSplitNode(mainDockSpaceId, vc::GUIDir_Right, 0.25f, NULL, &dock_id_left);

            vc::GUI::DockWindow("Settings & Objects", dock_id_left);
            vc::GUI::DockWindow("Test", dock_id_right);

            vc::GUI::DockFinish(mainDockSpaceId);
        }
    }
    vc::GUI::End();
}
