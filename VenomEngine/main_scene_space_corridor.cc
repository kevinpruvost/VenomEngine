///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"

void SceneSpaceCorridor(const vc::ScenePhase phase)
{
    static vc::Entity cubemap, corridor, camera, light3, light4, focusPoint, monster, astronaut1, astronaut2;
    int nbTiles = 1;
    switch (phase) {
        case vc::ScenePhase::Initialization: {
            cubemap = vc::CreateEntity("Background")
        //        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
                    .emplace<vc::Skybox>("cubemap/marslike_planet.exr")
                ;

            corridor = vc::CreateEntity("Helmet")
//            .emplace<vc::Model>("eye/eye.glb")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/corridor_space.glb")
            ;
            ;

            monster = vc::CreateEntity("Monster")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/infector_nf-75.glb")

            ;

            astronaut1 = vc::CreateEntity("Astronaut1")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/Astronaut_rust.glb")

            ;

            astronaut2 = vc::CreateEntity("Astronaut2")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/Astronaut_rust.glb")

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
            focusPoint.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(1.208f, 1.866f, -0.039f));

            monster.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(1.009f, 1.889f, 0.006f));
            monster.get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(0.0f, 1.121f, 0.0f));
            monster.get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(0.133f, 0.133f, 0.133f));

            astronaut1.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-0.276f, 1.845f, -0.025f));
            astronaut1.get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(0.0f, 1.478f, 0.0f));
            astronaut1.get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(0.104f, 0.104f, 0.104f));

            astronaut2.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-0.279f, 1.851f, 0.159f));
            astronaut2.get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(0.0f, 1.417f, 0.0f));
            astronaut2.get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(0.104f, 0.104f, 0.104f));

            corridor.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 2.0f, 0.0f));
            corridor.get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(0.0f, 0.0f, 0.0f));

            camera.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-0.446f, 2.023f, 0.191f));
            camera.get_mut<vc::Transform3D>()->SetRotation({-0.094f, -1.433f, 0.0f});
            //camera.get_mut<vc::Camera>()->LookAt(helmet.get<vc::Transform3D>()->GetPosition());

            light3.get_mut<vc::Light>()->SetType(vc::LightType::Point);
            light3.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.118f, 2.324f, 0.066f));
            light3.get_mut<vc::Transform3D>()->SetRotation({0.0f, 0.0f, 0.0f});
            light3.get_mut<vc::Light>()->SetColor({0.474f, 0.847f, 1.0f});
            light3.get_mut<vc::Light>()->SetIntensity(0.2f);

            light4.get_mut<vc::Light>()->SetType(vc::LightType::Spot);
            light4.get_mut<vc::Transform3D>()->SetPosition({-0.172f, 1.948f, 0.031f});
            light4.get_mut<vc::Transform3D>()->SetRotation({0.0f, 0.000f, 1.533f});
            light4.get_mut<vc::Light>()->SetColor({1.0f, 0.258f, 0.047f});
            light4.get_mut<vc::Light>()->SetAngle(23.906f);
            light4.get_mut<vc::Light>()->SetIntensity(50.0f);

            break;
        }
        case vc::ScenePhase::Update: {
            // After 6 seconds, make light 4 flicker for 1.5 seconds and turn off for 3 seconds
            float totalMs = std::fmod(vc::Timer::GetTotalMilliseconds(), 12000.0f);
            if (totalMs >= 6000.0f && totalMs < 7500.0f) {
                light4.get_mut<vc::Light>()->SetIntensity(vcm::PingPongLerp(-10.0f, 50.0f, (totalMs - 6000.0f) * 0.003f));
                light3.get_mut<vc::Light>()->SetIntensity(vcm::PingPongLerp(-10.0f, 0.2f, (totalMs - 6000.0f) * 0.003f));
            } else if (totalMs >= 7500.0f && totalMs < 12000.0f) {
                light4.get_mut<vc::Light>()->SetIntensity(0.0f);
                light3.get_mut<vc::Light>()->SetIntensity(0.0f);
            } else {
                light4.get_mut<vc::Light>()->SetIntensity(50.0f);
                light3.get_mut<vc::Light>()->SetIntensity(0.2f);
            }
            break;
        }
        case vc::ScenePhase::Destruction: break;
        default: break;
    }
}
