///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <unordered_set>

#include "MainScene.h"

void SceneV(const vc::ScenePhase phase)
{
    static vc::Entity cubemap, camera;
    int nbTiles = 1;
    switch (phase) {
        case vc::ScenePhase::Initialization: {
            DEBUG_PRINT("Hello -1");
            cubemap = vc::CreateEntity("Background")
        //        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
                    .emplace<vc::Skybox>("cubemap/billiard_hall.exr")
            ;
            DEBUG_PRINT("Hello 0");


            camera = vc::CreateEntity("Camera")
                .emplace<vc::Camera>();

            DEBUG_PRINT("Hello 1");
            vc::GraphicsSettings::StartGfxSettingsChange();
            vc::GraphicsSettings::SetHDR(true);
            DEBUG_PRINT("Hello 2");
            vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::MultiSamplingCountOption::Samples2);
            vc::GraphicsSettings::EndGfxSettingsChange();
            DEBUG_PRINT("Hello 3");
            vc::SceneSettings::SetTargetLuminance(10.0f);
            DEBUG_PRINT("Hello 4");
            break;
        }
        case vc::ScenePhase::Activation: {
            camera.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-5.2f, 4.8f, -4.7f));
            //camera.get_mut<vc::Camera>()->LookAt(helmet.get<vc::Transform3D>()->GetPosition());

            break;
        }
        case vc::ScenePhase::Update: break;
        case vc::ScenePhase::Destruction: break;
        default: break;
    }
}
