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
        vc::ECS::GetEntity("balls_hd").get_mut<vc::Transform3D>()->RotateYaw(time / 1000.0f);
        vc::ECS::GetEntity("face_hd").get_mut<vc::Transform3D>()->RotateYaw(time / 1000.0f);
    }

    if (context->IsKeyReleased(vc::KeyboardInput::KeyboardT)) {
        cameraLocked = !cameraLocked;
    }
}

void SceneGUI()
{
    vc::Entity light1 = vc::ECS::GetEntity("light1");

    vc::GUI::NewFrame();
    vc::GUI::Begin("VenomEngine");
    vc::GUI::Text("Welcome to my demo !");
    vc::GUI::Checkbox("Camera Locked", &cameraLocked);
    vc::GUI::Checkbox("Automatic Turn", &automaticTurn);
    vc::GUI::SliderFloat("Light Intensity", light1.get_mut<vc::Light>()->GetIntensityPtr(), 0.0f, 10.0f);
    vc::GUI::SliderFloat3("Light Direction", light1.get_mut<vc::Light>()->GetDirectionPtr(), -1.0f, 1.0f);
    vc::GUI::End();
}