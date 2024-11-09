///
/// Project: VenomEngineWorkspace
/// @file main_input.cc
/// @date Nov, 08 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"

bool cameraLocked = true;

void SceneInput(vc::Context * context)
{
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

    if (context->IsKeyReleased(vc::KeyboardInput::KeyboardT)) {
        cameraLocked = !cameraLocked;
    }
}

void SceneGUI()
{
    vc::GUI::NewFrame();
    vc::GUI::Begin("VenomEngine");
    vc::GUI::Text("Welcome to my demo !");
    vc::GUI::Checkbox("Camera Locked", &cameraLocked);
    vc::GUI::End();
}