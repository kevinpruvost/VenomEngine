
///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"

void ScenePeach(const vc::ScenePhase phase)
{
    static vc::Entity cubemap, corridor, camera, light3, focusPoint, monster;
    static vc::Entity mobs[4];
    int nbTiles = 1;
    switch (phase) {
        case vc::ScenePhase::Initialization: {
            cubemap = vc::CreateEntity("Background")
        //        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
                    .emplace<vc::Skybox>("cubemap/cloudy_sky.exr")
                .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::Skybox);

            corridor = vc::CreateEntity("Helmet")
//            .emplace<vc::Model>("eye/eye.glb")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/peach_castle.glb")
            .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel);
            ;

            monster = vc::CreateEntity("Monster")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/mighty_doom_guy.glb")
            .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel)
            ;

            mobs[0] = vc::CreateEntity("Astronaut1")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/goomba.glb")
            .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel)
            ;

            mobs[1] = vc::CreateEntity("Astronaut2")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/goomba.glb")
            .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel)
            ;

            mobs[2] = vc::CreateEntity("Astronaut3")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/goomba.glb")
            .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel)
            ;

            mobs[3] = vc::CreateEntity("Astronaut4")
            .emplace<vc::Model>("/Volumes/Kevin潘凯文/Youtube/Video1/Resources/goomba.glb")
            .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel)
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
            vc::SceneSettings::SetTargetLuminance(150.0f);
            break;
        }
        case vc::ScenePhase::Activation: {
            focusPoint.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(8.381f, -7.866f, -4.423f));

            monster.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(3.689f, -7.823f, -7.950f));
            monster.get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(-0.104f, -0.063f, 0.0f));
            monster.get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(2.0f, 2.0f, 2.0f));

            mobs[0].get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(2.258f, -9.388f, -1.606f));
            mobs[0].get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(0.0f, -0.002f, 0.0f));
            mobs[0].get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(0.8f, 0.8f, 0.8f));

            mobs[1].get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(5.000f, -9.388f, -1.617f));
            mobs[1].get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(0.0f, -0.002f, 0.0f));
            mobs[1].get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(0.8f, 0.8f, 0.8f));

            mobs[2].get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(5.000f, -9.558f, 0.785f));
            mobs[2].get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(0.0f, -0.002f, 0.0f));
            mobs[2].get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(0.8f, 0.8f, 0.8f));

            mobs[3].get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(2.352f, -9.558f, 0.789f));
            mobs[3].get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(0.0f, -0.002f, 0.0f));
            mobs[3].get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(0.8f, 0.8f, 0.8f));

            for (int i = 0; i < 4; ++i) {
                mobs[i].get_mut<vc::Transform3D>()->LookAt(monster.get<vc::Transform3D>()->GetPosition());
                mobs[i].get_mut<vc::Transform3D>()->RotateYaw(3.142f);
            }

            corridor.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 2.0f, 0.0f));
            corridor.get_mut<vc::Transform3D>()->SetRotation(vcm::Vec3(0.0f, 0.0f, 0.0f));
            corridor.get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(50.0f, 50.0f, 50.0f));

            camera.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-0.382f, -7.041f, 12.973f));
            camera.get_mut<vc::Transform3D>()->SetRotation({-0.043f, -0.471f, 0.0f});
            //camera.get_mut<vc::Camera>()->LookAt(helmet.get<vc::Transform3D>()->GetPosition());

            light3.get_mut<vc::Light>()->SetType(vc::LightType::Directional);
            light3.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.118f, 2.324f, 0.066f));
            light3.get_mut<vc::Transform3D>()->SetRotation({-1.625f, -0.867f, -1.676f});
            light3.get_mut<vc::Light>()->SetColor({1.0f, 0.0f, 0.0f});
            light3.get_mut<vc::Light>()->SetIntensity(75.0f);

            break;
        }
        case vc::ScenePhase::Update: {
            break;
        }
        case vc::ScenePhase::Destruction: break;
        default: break;
    }
}
