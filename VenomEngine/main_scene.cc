///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"

void Scene()
{
    vc::Entity cubemap = vc::CreateEntity("cubemap")
//        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
            .emplace<vc::Skybox>("cubemap/billiard_hall.exr")
        .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::Skybox);

    vc::Entity balls_hd = vc::CreateEntity("balls_hd")
        .emplace<vc::Transform3D>()
//            .emplace<vc::Model>("eye/eye.obj")
//            .emplace<vc::Model>("dead_space_gun/plasmagun_txt.fbx")
        .emplace<vc::Model>("dead_space_gun/gun.glb")
        .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel);
        ;
    balls_hd.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-8.0f, 4.0f, 0.0f));

    vc::Entity face_hd = vc::CreateEntity("face_hd")
        .emplace<vc::Transform3D>()
        .emplace<vc::Model>("face/face.obj")
        .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel);
        ;
    face_hd.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(0.0f, 2.0f, 0.0f));

    vc::Entity helmet = vc::CreateEntity("helmet_hd")
    .emplace<vc::Transform3D>()
    .emplace<vc::Model>("helmet/helmet.glb")
    .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel);
    ;
    helmet.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(-4.0f, 4.0f, 0.0f));

    vc::Entity camera = vc::CreateEntity("camera")
        .emplace<vc::Transform3D>()
        .emplace<vc::Camera>();
    camera.get_mut<vc::Camera>()->SetPosition(vcm::Vec3(-5.2f, 4.8f, -4.7f));
    camera.get_mut<vc::Camera>()->LookAt(helmet.get<vc::Transform3D>()->GetPosition());

    vc::Entity light1 = vc::CreateEntity("light1")
        .emplace<vc::Light>();
    light1.get_mut<vc::Light>()->SetType(vc::LightType::Directional);
    light1.get_mut<vc::Light>()->SetPosition({6.0f, 10.0f, 2.0f});
    light1.get_mut<vc::Light>()->SetDirection({0.053f, -0.230f, 1.0f});
    light1.get_mut<vc::Light>()->SetColor({1.0f, 1.0f, 1.0f});
    light1.get_mut<vc::Light>()->SetIntensity(10.0f);

    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 1; j++) {
            auto name = "ground" + std::to_string(i) + "_" + std::to_string(j);
            vc::Entity ground = vc::CreateEntity(name.c_str())
                .emplace<vc::Transform3D>()
                .emplace<vc::Model>("ground/ground.glb")
                .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::PBRModel);
            ground.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(i * 3.5f, 0.0f,j * 3.5f));
            ground.get_mut<vc::Transform3D>()->SetScale(vcm::Vec3(1.0f, 1.0f, 1.0f));
            ground.get_mut<vc::Model>()->GetMaterials()[0].SetTextureRepeatFactor(vcm::Vec2(1.0f, 1.0f));
        }
    }

    vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::MultiSamplingCountOption::Samples4);
}
