///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"

void SceneLarge(const vc::ScenePhase phase)
{
    static vc::Entity cubemap, car, camera, light3, focusPoint;
    int nbTiles = 1;
    switch (phase) {
        case vc::ScenePhase::Initialization: {
            cubemap = vc::CreateEntity("Background")
        //        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
                    .emplace<vc::Skybox>("cubemap/cloudy_sky.exr")
                .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::Skybox);

            car = vc::CreateEntity("Helmet")
//            .emplace<vc::Model>("eye/eye.glb")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/low_poly_forest.glb")
            .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel);
            ;

            camera = vc::CreateEntity("Camera")
                .emplace<vc::Camera>();

            light3 = vc::CreateEntity("Light3")
            .emplace<vc::Light>();

            focusPoint = vc::CreateEntity("FocusPoint");

            //vc::GraphicsSettings::StartGfxSettingsChange();
            vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::MultiSamplingCountOption::Samples2);
            vc::GraphicsSettings::SetHDR(true);
            //vc::GraphicsSettings::EndGfxSettingsChange();
            vc::SceneSettings::SetTargetLuminance(85.0f);
            break;
        }
        case vc::ScenePhase::Activation: {
            focusPoint.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(8.381f, -7.866f, -4.423f));

            car.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 2.0f, 0.0f));
            car.get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(0.0f, 0.0f, 0.0f));
            car.get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(50.0f, 50.0f, 50.0f));

            camera.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(38.454f, 8.019f, -30.579f));
            camera.get_mut<vc::Transform3D>()->SetRotation({-0.126f, -3.800f, 0.0f});
            //camera.get_mut<vc::Camera>()->LookAt(helmet.get<vc::Transform3D>()->GetPosition());

            light3.get_mut<vc::Light>()->SetType(vc::LightType::Directional);
            light3.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.118f, 2.324f, 0.066f));
            light3.get_mut<vc::Transform3D>()->SetRotation({-1.408f, -0.867f, -1.676f});
            light3.get_mut<vc::Light>()->SetColor({1.0f, 1.0f, 1.0f});
            light3.get_mut<vc::Light>()->SetIntensity(12.0f);

            break;
        }
        case vc::ScenePhase::Update: {
            light3.get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(vcm::LinearPingPongLerp(-2.058f, 0.0f, vc::Timer::GetTotalMilliseconds() * 0.0001f), -0.867f, -1.676f));
            break;
        }
        case vc::ScenePhase::Destruction: break;
        default: break;
    }
}