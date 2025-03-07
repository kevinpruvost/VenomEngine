///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <unordered_set>

#include "MainScene.h"

void SceneVulkan(const vc::ScenePhase phase)
{
    static vc::Entity cubemap, balls_hd, gun_hd, eye_hd, face_hd, face_hd1, helmet, camera, light1, light2, light3, light4;
    int nbTiles = 1;
    switch (phase) {
        case vc::ScenePhase::Initialization: {
            cubemap = vc::CreateEntity("Background")
        //        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
                    .emplace<vc::Skybox>("cubemap/billiard_hall.exr");

            balls_hd = vc::CreateEntity("Onion")
                .emplace<vc::Model>("onion/onion.glb")
                ;
            ;

            gun_hd = vc::CreateEntity("Gun")
                .emplace<vc::Model>("dead_space_gun/gun.glb")
                ;

            eye_hd = vc::CreateEntity("Eye")
                .emplace<vc::Model>("eye/eye.glb")
                ;

            face_hd = vc::CreateEntity("Face")
                .emplace<vc::Model>("face/face.obj")
                ;
            ;

            face_hd1 = vc::CreateEntity("Face2")
                .emplace<vc::Model>("face/face.obj")
                ;
            ;

            helmet = vc::CreateEntity("Helmet")
            .emplace<vc::Model>("helmet/helmet.glb")
            ;
            ;

            camera = vc::CreateEntity("Camera")
                .emplace<vc::Camera>();

            light1 = vc::CreateEntity("Light1")
                .emplace<vc::Light>();

            light2 = vc::CreateEntity("Light2")
            .emplace<vc::Light>();

            light3 = vc::CreateEntity("Light3")
            .emplace<vc::Light>();

            light4 = vc::CreateEntity("Light4")
            .emplace<vc::Light>();

            for (int i = 0; i < nbTiles; i++) {
                for (int j = 0; j < nbTiles; j++) {
                    auto name = "ground" + std::to_string(i) + "_" + std::to_string(j);
                    vc::Entity ground = vc::CreateEntity(name.c_str())
                        .emplace<vc::Model>("ground/ground.glb")
                        ;
                    ground.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(i * 3.5f, 0.0f,j * 3.5f));
                    ground.get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(1.0f, 1.0f, 1.0f));
                    ground.get_mut<vc::Model>()->GetMaterials()[0].SetTextureRepeatFactor(vcm::Vec2(1.0f, 1.0f));
                }
            }

            vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::MultiSamplingCountOption::Samples2);
            vc::SceneSettings::SetTargetLuminance(10.0f);
            break;
        }
        case vc::ScenePhase::Activation: {
            gun_hd.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(6.0f, 4.0f, 0.0f));
            balls_hd.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-8.0f, 4.0f, 0.0f));
            eye_hd.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 4.0f, 6.0f));
            face_hd.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 2.0f, 0.0f));
            face_hd1.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-0.5f, 1.0f, 4.0f));
            helmet.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-4.0f, 4.0f, 0.0f));

            camera.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-5.2f, 4.8f, -4.7f));
            camera.get_mut<vc::Camera>()->LookAt(helmet.get<vc::Transform3D>()->GetPosition());

            light2.get_mut<vc::Light>()->SetType(vc::LightType::Directional);
            light2.get_mut<vc::Transform3D>()->SetRotation({0.751f, 1.762f, 2.661f});
            light2.get_mut<vc::Light>()->SetColor({0.37f, 0.0f, 0.17f});
            light2.get_mut<vc::Light>()->SetIntensity(8.0f);

            light1.get_mut<vc::Light>()->SetType(vc::LightType::Point);
            light1.get_mut<vc::Transform3D>()->SetPosition({0.984f, 2.004f, -0.211f});
            light1.get_mut<vc::Transform3D>()->SetRotation({-0.5f, 0.0f, 0.0f});
            light1.get_mut<vc::Light>()->SetColor({0.0f, 0.5f, 0.01f});
            light1.get_mut<vc::Light>()->SetIntensity(5.0f);

            light3.get_mut<vc::Light>()->SetType(vc::LightType::Spot);
            light3.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(1.0f, 3.0f, -3.0f));
            light3.get_mut<vc::Transform3D>()->SetRotation({-0.157f, 1.858f, 1.687f});
            light3.get_mut<vc::Transform3D>()->SetScale({0.1f, 0.1f, 0.1f});
            light3.get_mut<vc::Light>()->SetColor({0.0f, 0.15f, 1.0f});
            light3.get_mut<vc::Light>()->SetIntensity(1000.0f);
            light3.get_mut<vc::Light>()->SetAngle(20.0f);

            light4.get_mut<vc::Light>()->SetType(vc::LightType::Point);
            light4.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-0.5f, 1.0f, 7.0f));
            light4.get_mut<vc::Transform3D>()->SetRotation({0.3f, 0.0f, 0.6f});
            light4.get_mut<vc::Light>()->SetIntensity(50.0f);

            for (int i = 0; i < nbTiles; i++) {
                for (int j = 0; j < nbTiles; j++) {
                    auto name = "ground" + std::to_string(i) + "_" + std::to_string(j);
                    auto ground = vc::ECS::GetEntity(name.c_str());
                    ground.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(i * 3.5f, 0.0f, j * 3.5f));
                    ground.get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(1.0f, 1.0f, 1.0f));
                    ground.get_mut<vc::Model>()->GetMaterials()[0].SetTextureRepeatFactor(vcm::Vec2(1.0f, 1.0f));
                }
            }

            break;
        }
        case vc::ScenePhase::Update: break;
        case vc::ScenePhase::Destruction: break;
        default: break;
    }
}
