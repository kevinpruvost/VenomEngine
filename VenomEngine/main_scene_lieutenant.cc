///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"

void SceneLieutenant(const vc::ScenePhase phase)
{
    static vc::Entity cubemap, helmet, camera, light3, light4, light5;
    int nbTiles = 1;
    switch (phase) {
        case vc::ScenePhase::Initialization: {
            cubemap = vc::CreateEntity("Background")
        //        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
                    .emplace<vc::Skybox>("cubemap/billiard_hall.exr")
                .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::Skybox);

            helmet = vc::CreateEntity("Helmet")
//            .emplace<vc::Model>("eye/eye.glb")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/damaged_helmet.glb")
            .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel);
            ;

            camera = vc::CreateEntity("Camera")
                .emplace<vc::Camera>();

            light3 = vc::CreateEntity("Light3")
            .emplace<vc::Light>();

            light4 = vc::CreateEntity("Light4")
            .emplace<vc::Light>();

            light5 = vc::CreateEntity("Light5")
            .emplace<vc::Light>();

            //vc::GraphicsSettings::StartGfxSettingsChange();
            vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::MultiSamplingCountOption::Samples2);
            vc::GraphicsSettings::SetHDR(true);
            //vc::GraphicsSettings::EndGfxSettingsChange();
            vc::SceneSettings::SetTargetLuminance(100.0f);
            break;
        }
        case vc::ScenePhase::Activation: {
            cubemap.get_mut<vc::Skybox>()->ChangeBlurFactor(1.0f);

            helmet.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 2.0f, 0.0f));
            helmet.get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(3.142f, 0.149f, 3.142f));

            camera.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-1.386f, 2.457f, -4.835f));
            camera.get_mut<vc::Transform3D>()->SetRotation({-0.091f, -2.862f, 0.0f});
            //camera.get_mut<vc::Camera>()->LookAt(helmet.get<vc::Transform3D>()->GetPosition());
            camera.get_mut<vc::Camera>()->SetFieldOfView(35.0f);

            light3.get_mut<vc::Light>()->SetType(vc::LightType::Directional);
            light3.get_mut<vc::Transform3D>()->SetRotation({1.192f, 1.595f, -0.838f});
            light3.get_mut<vc::Light>()->SetColor({1.00f, 0.00f, 0.00f});
            light3.get_mut<vc::Light>()->SetIntensity(100.0f);

            light4.get_mut<vc::Light>()->SetType(vc::LightType::Spot);
            light4.get_mut<vc::Transform3D>()->SetPosition({-0.128f, 1.754f, -2.453f});
            light4.get_mut<vc::Transform3D>()->SetRotation({-1.318f, -0.001f, 0.0f});
            light4.get_mut<vc::Light>()->SetColor({0.0f, 0.0f, 1.0f});
            light4.get_mut<vc::Light>()->SetAngle(70.0f);
            light4.get_mut<vc::Light>()->SetIntensity(35.0f);

            light5.get_mut<vc::Light>()->SetType(vc::LightType::Spot);
            light5.get_mut<vc::Transform3D>()->SetPosition({1.965f, 1.722f, -0.736});
            light5.get_mut<vc::Transform3D>()->SetRotation({-2.631f, 0.436f, -1.494f});
            light5.get_mut<vc::Light>()->SetColor({0.0f, 1.0f, 0.0f});
            light5.get_mut<vc::Light>()->SetAngle(70.0f);
            light5.get_mut<vc::Light>()->SetIntensity(35.0f);

            break;
        }
        case vc::ScenePhase::Update: {
            light3.get_mut<vc::Light>()->SetIntensity(vcm::PingPongLerp(-20.0f, 30.0f, vc::Timer::GetTotalMilliseconds() * 0.0005f));
            light4.get_mut<vc::Light>()->SetIntensity(vcm::PingPongLerp(-20.0f, 30.0f, vc::Timer::GetTotalMilliseconds() * 0.0005f));
            light5.get_mut<vc::Light>()->SetIntensity(vcm::PingPongLerp(-20.0f, 30.0f, vc::Timer::GetTotalMilliseconds() * 0.0005f));

            vc::SceneSettings::SetTargetLuminance(vcm::LinearPingPongLerp(-150.0f, 350.0f, vc::Timer::GetTotalMilliseconds() * 0.001f / 8.0f));
            break;
        }
        case vc::ScenePhase::Destruction: break;
        default: break;
    }
}
