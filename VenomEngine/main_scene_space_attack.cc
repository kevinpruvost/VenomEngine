///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"

void SceneSpace(const vc::ScenePhase phase)
{
    static vc::Entity cubemap, camera, planet, adam, light1, light2;
    static vc::Entity spaceships[10];
    static float spaceshipsSpeed[10];
    int nbTiles = 1;
    switch (phase) {
        case vc::ScenePhase::Initialization: {
            cubemap = vc::CreateEntity("Background")
        //        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
                    .emplace<vc::Skybox>("cubemap/nebula.exr")
                .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::Skybox);

            for (int i = 0; i < 10; i++) {
                spaceships[i] = vc::CreateEntity(vc::format("Spaceship%d", i).c_str())
                    .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/space_fighter.glb")
                    .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel);
                spaceshipsSpeed[i] = rand() % 1000;
                spaceshipsSpeed[i] = spaceshipsSpeed[i] * 0.004f + 0.3f;
            }

            planet = vc::CreateEntity("Planet")
                .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/planet.glb")
                .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel);

            camera = vc::CreateEntity("Camera")
                .emplace<vc::Camera>();

            adam = vc::CreateEntity("Adam")
                .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/adam_head.glb")
                .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel);

            light1 = vc::CreateEntity("Light1")
                .emplace<vc::Light>();

            light2 = vc::CreateEntity("Light2")
                .emplace<vc::Light>();

            //vc::GraphicsSettings::StartGfxSettingsChange();
            vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::MultiSamplingCountOption::Samples2);
            vc::GraphicsSettings::SetHDR(true);
            //vc::GraphicsSettings::EndGfxSettingsChange();
            vc::SceneSettings::SetTargetLuminance(1.0f);
            break;
        }
        case vc::ScenePhase::Activation: {
            planet.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-62.377f, 0.0f, -51.608f));
            planet.get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(9.202f, 9.202f, 9.202f));

            light1.get_mut<vc::Light>()->SetType(vc::LightType::Point);
            light1.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-57.903f, 11.947f, -64.082f));
            light1.get_mut<vc::Light>()->SetIntensity(4000.0f);
            light1.get_mut<vc::Light>()->SetColor({1.0f, 0.388f, 0.0f});

            light2.get_mut<vc::Light>()->SetType(vc::LightType::Point);
            light2.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(11.128f, 3.904f, 1.649f));
            light2.get_mut<vc::Light>()->SetIntensity(1000.0f);
            light2.get_mut<vc::Light>()->SetColor({1.0f, 0.524f, 0.0f});

            for (int i = 0; i < 10; i++) {
                spaceships[i].get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3((i % 5) * 5.0f + 0.0f, (float)(rand() % 6) - 3.0f, (i / 5) * 5.0f + 0.0f));
                spaceships[i].get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(2.819f, 0.591f, -3.142f));
                spaceships[i].get_mut<vc::Transform3D>()->LookAt(planet.get<vc::Transform3D>()->GetPosition());
                // Turn them back 180
                spaceships[i].get_mut<vc::Transform3D>()->RotateYaw(3.142f);
            }
            camera.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(16.445f, 2.853f, 7.530f));
            camera.get_mut<vc::Transform3D>()->SetRotation({-0.138f, 13.616f, 0.0f});

            adam.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(13.004f, 2.268f, 7.574f));
            adam.get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(0.0f, 0.733f, 0.0f));

            //camera.get_mut<vc::Camera>()->LookAt(helmet.get<vc::Transform3D>()->GetPosition());
            camera.get_mut<vc::Camera>()->SetFieldOfView(45.0f);
            break;
        }
        case vc::ScenePhase::Update:
        {
            // Move the spaceships towards the planet with varying speeds
            for (int i = 0; i < 10; i++) {
                vcm::Vec3 pos = spaceships[i].get<vc::Transform3D>()->GetPosition();
                vcm::Vec3 planetPos = planet.get<vc::Transform3D>()->GetPosition();
                vcm::Vec3 dir = planetPos - pos;
                vcm::Normalize(dir);
                const float lambdaMs = vc::Timer::GetLambdaMilliseconds() * 0.001f;
                pos += dir * lambdaMs * spaceshipsSpeed[i];
                spaceships[i].get_mut<vc::Transform3D>()->SetPosition(pos);
            }
            camera.get_mut<vc::Camera>()->SetFieldOfView(vcm::LinearPingPongLerp(45.0f, 48.0f, vc::Timer::GetTotalMilliseconds() * 0.00005f));

            // If type P, then reset positions
            if (vc::Context::Get()->IsKeyReleased(vc::KeyboardInput::KeyboardP)) {
                for (int i = 0; i < 10; i++) {
                    spaceships[i].get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3((i % 5) * 5.0f + 0.0f, (float)(rand() % 6) - 3.0f, (i / 5) * 5.0f + 0.0f));
                    spaceships[i].get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(2.819f, 0.591f, -3.142f));
                    spaceships[i].get_mut<vc::Transform3D>()->LookAt(planet.get<vc::Transform3D>()->GetPosition());
                    // Turn them back 180
                    spaceships[i].get_mut<vc::Transform3D>()->RotateYaw(3.142f);
                }
            }
            break;
        }
        case vc::ScenePhase::Destruction: break;
        default: break;
    }
}
