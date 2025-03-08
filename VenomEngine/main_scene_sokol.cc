///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"

void SceneSokol(const vc::ScenePhase phase)
{
    static vc::Entity cubemap, helmet, camera, light3, light4, focusPoint;
    int nbTiles = 1;
    switch (phase) {
        case vc::ScenePhase::Initialization: {
            cubemap = vc::CreateEntity("Background")
        //        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
                    .emplace<vc::Skybox>("cubemap/marslike_planet.exr")
                ;

            helmet = vc::CreateEntity("Helmet")
//            .emplace<vc::Model>("eye/eye.glb")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/sokol-space-suit.glb")
            ;
            ;

            camera = vc::CreateEntity("Camera")
                .emplace<vc::Camera>();

            light3 = vc::CreateEntity("Light3")
            .emplace<vc::Light>();

            light4 = vc::CreateEntity("Light4")
            .emplace<vc::Light>();

            focusPoint = vc::CreateEntity("FocusPoint");

            //vc::GraphicsSettings::StartGfxSettingsChange();
            vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::MultiSamplingCountOption::Samples2);
            vc::GraphicsSettings::SetHDR(true);
            //vc::GraphicsSettings::EndGfxSettingsChange();
            vc::SceneSettings::SetTargetLuminance(1000.0f);
            break;
        }
        case vc::ScenePhase::Activation: {
            focusPoint.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(2.763f, 2.0f, -6.331f));

            helmet.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 2.0f, 0.0f));
            helmet.get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(-2.925f, 0.325f, 0.831f));

            camera.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-9.203f, 2.483f, -0.748f));
            camera.get_mut<vc::Transform3D>()->SetRotation({-0.037f, -1.134f, 0.0f});
            //camera.get_mut<vc::Camera>()->LookAt(helmet.get<vc::Transform3D>()->GetPosition());

            light3.get_mut<vc::Light>()->SetType(vc::LightType::Directional);
            light3.get_mut<vc::Transform3D>()->SetRotation({2.094f, 0.078f, -0.304f});
            light3.get_mut<vc::Light>()->SetColor({1.0f, 1.0f, 1.0f});
            light3.get_mut<vc::Light>()->SetIntensity(0.0f);

            light4.get_mut<vc::Light>()->SetType(vc::LightType::Spot);
            light4.get_mut<vc::Transform3D>()->SetPosition({0.0f, 1.688f, -2.581f});
            light4.get_mut<vc::Transform3D>()->SetRotation({-1.318f, -0.001f, 0.0f});
            light4.get_mut<vc::Light>()->SetColor({0.815f, 0.309f, 0.309f});
            light4.get_mut<vc::Light>()->SetAngle(130.0f);
            light4.get_mut<vc::Light>()->SetIntensity(53.0f);

            break;
        }
        case vc::ScenePhase::Update: {
            helmet.get_mut<vc::Transform3D>()->Rotate(vcm::Vec3(vc::Timer::GetLambdaMilliseconds() * 0.00001f, 0.0f, 0.0f));
            break;
        }
        case vc::ScenePhase::Destruction: break;
        default: break;
    }
}
