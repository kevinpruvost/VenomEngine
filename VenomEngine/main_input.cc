///
/// Project: VenomEngineWorkspace
/// @file main_input.cc
/// @date Nov, 08 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"

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

int drawNormalMap = false;
int disableMetallic = false;
float metallic = 0.0f;
int disableRoughness = false;
float roughness = 0.0f;

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


    vc::GUI::SetNextWindowPos(vcm::Vec2(0, 20), vc::GUICondBits::GUICond_Always);
    vc::GUI::Begin("VenomEngine");
    vc::GUI::Checkbox("Camera Locked", &cameraLocked);
    vc::GUI::Checkbox("Automatic Turn", &automaticTurn);
    vc::GUI::SliderFloat("Light Intensity", light1.get_mut<vc::Light>()->GetIntensityPtr(), 0.0f, 100.0f);
    vc::GUI::SliderFloat3("Light Direction", light1.get_mut<vc::Light>()->GetDirectionPtr(), -1.0f, 1.0f);
    if (vc::GUI::Checkbox("Draw Normal Map", (bool *)&drawNormalMap)) {
        vc::ShaderResourceTable::UpdateDescriptor(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SCENE, 1, &drawNormalMap, sizeof(int), sizeof(vcm::Vec2));
    }
    if (vc::GUI::Checkbox("Disable Metallic", (bool *)&disableMetallic)) {
        vc::ShaderResourceTable::UpdateDescriptor(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SCENE, 1, &disableMetallic, sizeof(int), sizeof(vcm::Vec2) + sizeof(int));
    }
    if (vc::GUI::SliderFloat("Metallic", &metallic, 0.0f, 1.0f)) {
        vc::ShaderResourceTable::UpdateDescriptor(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SCENE, 1, &metallic, sizeof(float), sizeof(vcm::Vec2) + 3 * sizeof(int));
    }
    if (vc::GUI::Checkbox("Disable Roughness", (bool *)&disableRoughness)) {
        vc::ShaderResourceTable::UpdateDescriptor(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SCENE, 1, &disableRoughness, sizeof(int), sizeof(vcm::Vec2) + 2 * sizeof(int));
    }
    if (vc::GUI::SliderFloat("Roughness", &roughness, 0.0f, 1.0f)) {
        vc::ShaderResourceTable::UpdateDescriptor(vc::ShaderResourceTable::SetsIndex::SETS_INDEX_SCENE, 1, &roughness, sizeof(float), sizeof(vcm::Vec2) + 3 * sizeof(int) + sizeof(float));
    }
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
    vc::GUI::End();

    vc::GUI::SetNextWindowPos(vcm::Vec2{vc::Context::GetWindowWidth(), 20}, vc::GUICondBits::GUICond_Always, vcm::Vec2(1.0f, 0));
    vc::GUI::Begin("TestWindow");
    vc::GUI::Text("Hello, world!");
    vc::GUI::End();
}
