///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <unordered_set>

#include "MainScene.h"

void Scene(const vc::ScenePhase phase)
{
    static vc::Entity cubemap, camera, corridor;
    static vc::Entity lights[4];
    int nbTiles = 1;
    switch (phase) {
        case vc::ScenePhase::Initialization: {
            cubemap = vc::CreateEntity("Background")
        //        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
                    .emplace<vc::Skybox>("cubemap/billiard_hall.exr")
            ;
            
            corridor = vc::CreateEntity("Helmet")
//            .emplace<vc::Model>("eye/eye.glb")
            .emplace<vc::Model>("chess/chess.glb")
            ;

            camera = vc::CreateEntity("Camera").emplace<vc::Camera>();

            for (int i = 0; i < 4; ++i) {
                lights[i] = vc::CreateEntity(vc::format("Light%d", i).c_str()).emplace<vc::Light>();
            }

            vc::GraphicsSettings::StartGfxSettingsChange();
            vc::GraphicsSettings::SetHDR(false);
            vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::MultiSamplingCountOption::Samples2);
            vc::GraphicsSettings::EndGfxSettingsChange();
            vc::SceneSettings::SetTargetLuminance(500.0f);
            break;
        }
        case vc::ScenePhase::Activation: {
            camera.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-1.262f, 1.348f, -1.515f));
            camera.get_mut<vc::Camera>()->LookAt(corridor.get<vc::Transform3D>()->GetPosition());

            lights[0].get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 0.0f, 0.0f));
            lights[0].get_mut<vc::Light>()->SetColor(vcm::Vec3(1.0f, 1.0f, 1.0f));
            lights[0].get_mut<vc::Light>()->SetIntensity(2.0f);
            lights[0].get_mut<vc::Light>()->SetType(vc::LightType::Directional);
            lights[0].get_mut<vc::Transform3D>()->SetRotation({2.094f, 0.078f, -0.304f});

            lights[1].get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 0.0f, 0.0f));
            lights[1].get_mut<vc::Light>()->SetColor(vcm::Vec3(0.0f, 0.0f, 1.0f));
            lights[1].get_mut<vc::Light>()->SetIntensity(2.0f);
            lights[1].get_mut<vc::Light>()->SetType(vc::LightType::Point);
            lights[1].get_mut<vc::Light>()->SetAngle(0.0f);

            lights[2].get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 0.0f, 0.0f));
            lights[2].get_mut<vc::Light>()->SetColor(vcm::Vec3(1.0f, 0.0f, 0.0f));
            lights[2].get_mut<vc::Light>()->SetIntensity(2.0f);
            lights[2].get_mut<vc::Light>()->SetType(vc::LightType::Point);
            lights[2].get_mut<vc::Light>()->SetAngle(0.0f);

            lights[3].get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 0.0f, 0.0f));
            lights[3].get_mut<vc::Light>()->SetColor(vcm::Vec3(0.0f, 0.0f, 1.0f));
            lights[3].get_mut<vc::Light>()->SetIntensity(2.0f);
            lights[3].get_mut<vc::Light>()->SetType(vc::LightType::Point);
            lights[3].get_mut<vc::Light>()->SetAngle(0.0f);
            break;
        }
        case vc::ScenePhase::Update: break;
        case vc::ScenePhase::Destruction: break;
        default: break;
    }
}
