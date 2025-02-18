///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"

void Scene4(const vc::ScenePhase phase)
{
    static vc::Entity cubemap, camera, planet;
    static vc::Entity spaceships[50];
    int nbTiles = 1;
    switch (phase) {
        case vc::ScenePhase::Initialization: {
            cubemap = vc::CreateEntity("Background")
        //        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
                    .emplace<vc::Skybox>("cubemap/nebula.exr")
                .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::Skybox);

            for (int i = 0; i < 50; i++) {
                spaceships[i] = vc::CreateEntity(vc::format("Spaceship%d", i).c_str())
                    .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/space_fighter.glb")
                    .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel);
            }

            planet = vc::CreateEntity("Planet")
                .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/planet.glb")
                .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel);

            camera = vc::CreateEntity("Camera")
                .emplace<vc::Camera>();

            //vc::GraphicsSettings::StartGfxSettingsChange();
            vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::MultiSamplingCountOption::Samples2);
            vc::GraphicsSettings::SetHDR(true);
            //vc::GraphicsSettings::EndGfxSettingsChange();
            vc::SceneSettings::SetTargetLuminance(12.0f);
            break;
        }
        case vc::ScenePhase::Activation: {
            for (int i = 0; i < 50; i++) {
                spaceships[i].get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3((i % 5) * 3.0f, 0.0f, (i / 5) * 3.0f));
                spaceships[i].get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(2.819f, 0.591f, -3.142f));
            }
            camera.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(10.827f, 2.874f, 14.716f));
            camera.get_mut<vc::Transform3D>()->SetRotation({-0.221f, 13.141f, 0.0f});

            //camera.get_mut<vc::Camera>()->LookAt(helmet.get<vc::Transform3D>()->GetPosition());
            camera.get_mut<vc::Camera>()->SetFieldOfView(45.0f);

            planet.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 0.0f, 0.0f));
            break;
        }
        case vc::ScenePhase::Update: break;
        case vc::ScenePhase::Destruction: break;
        default: break;
    }
}
