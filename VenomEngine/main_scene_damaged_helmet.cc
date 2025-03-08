///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"

void Scene2z(const vc::ScenePhase phase)
{
    static vc::Entity cubemap, helmet, camera, light3, light4;
    int nbTiles = 1;
    switch (phase) {
        case vc::ScenePhase::Initialization: {
            cubemap = vc::CreateEntity("Background")
        //        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
                    .emplace<vc::Skybox>("cubemap/billiard_hall.exr");

            helmet = vc::CreateEntity("Helmet")
//            .emplace<vc::Model>("eye/eye.glb")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/lieutenant_head.glb")
            ;

            camera = vc::CreateEntity("Camera")
                .emplace<vc::Camera>();

            light3 = vc::CreateEntity("Light3")
            .emplace<vc::Light>();

            light4 = vc::CreateEntity("Light4")
            .emplace<vc::Light>();

            //vc::GraphicsSettings::StartGfxSettingsChange();
            vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::MultiSamplingCountOption::Samples2);
            vc::GraphicsSettings::SetHDR(true);
            //vc::GraphicsSettings::EndGfxSettingsChange();
            vc::SceneSettings::SetTargetLuminance(0.0f);
            break;
        }
        case vc::ScenePhase::Activation: {
            helmet.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 2.0f, 0.0f));

            camera.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-0.680f, 2.370f, -3.758f));
            camera.get_mut<vc::Transform3D>()->SetRotation({-0.079f, -2.751f, 0.0f});
            //camera.get_mut<vc::Camera>()->LookAt(helmet.get<vc::Transform3D>()->GetPosition());
            camera.get_mut<vc::Camera>()->SetFieldOfView(35.0f);

            light3.get_mut<vc::Light>()->SetType(vc::LightType::Directional);
            light3.get_mut<vc::Transform3D>()->SetRotation({2.094f, 0.078f, -0.304f});
            light3.get_mut<vc::Light>()->SetColor({0.05f, 0.05f, 0.05f});
            light3.get_mut<vc::Light>()->SetIntensity(100.0f);

            light4.get_mut<vc::Light>()->SetType(vc::LightType::Spot);
            light4.get_mut<vc::Transform3D>()->SetPosition({0.0f, 1.688f, -2.581f});
            light4.get_mut<vc::Transform3D>()->SetRotation({-1.318f, -0.001f, 0.0f});
            light4.get_mut<vc::Light>()->SetColor({1.0f, 0.0f, 0.0f});
            light4.get_mut<vc::Light>()->SetAngle(35.0f);
            light4.get_mut<vc::Light>()->SetIntensity(35.0f);

            break;
        }
        case vc::ScenePhase::Update: {
            static bool cinematicState = false;
            static float intensityStart = 0.0f, intensityStart2 = 0.0f;
            static vc::Timer endTimer;
            if (!cinematicState) {
                light3.get_mut<vc::Transform3D>()->RotateYaw(0.001f * vc::Timer::GetLambdaMicroseconds());
                light4.get_mut<vc::Light>()->SetIntensity(vcm::PingPongLerp(0.0f, 25.0f, vc::Timer::GetTotalMilliseconds() * 0.0005f));
            } else {
                light3.get_mut<vc::Transform3D>()->RotateYaw(0.001f * vc::Timer::GetLambdaMicroseconds());
                light3.get_mut<vc::Light>()->SetIntensity(vcm::LinearPingPongLerp(intensityStart, -100.0f, endTimer.GetMilliSeconds() * 0.0005f));
                light4.get_mut<vc::Light>()->SetIntensity(vcm::LinearPingPongLerp(intensityStart2, -100.0f, endTimer.GetMilliSeconds() * 0.0005f));
            }

            if (vc::Context::Get()->IsKeyReleased(vc::KeyboardInput::KeyboardP)) {
                cinematicState = !cinematicState;
                if (cinematicState) {
                    intensityStart = light3.get<vc::Light>()->GetIntensity();
                    intensityStart2 = light4.get<vc::Light>()->GetIntensity();
                    endTimer.Reset();
                } else {
                    light3.get_mut<vc::Light>()->SetIntensity(100.0f);
                }
            }
            break;
        }
        case vc::ScenePhase::Destruction: break;
        default: break;
    }
}
